#include "shader_program.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader_program::shader_program():
	m_linked( false ),
	m_id( 0 ),
	m_errorLog( "No Error: Empty shader program." )
{
}

shader_program::shader_program( const shader_program& other ):
	m_linked( other.m_linked ),
	m_id( other.m_id ),
	m_errorLog( other.m_errorLog ),
	m_store( other.m_store )
{
	if( !other.is_linked() )
		throw std::runtime_error( "shader_program: Failed to copy shader_program because it has not been linked." );

	if( m_id != 0 ) {
		assert( shader_prog_id_ref_count[other.m_id] > 0 );

		shader_prog_id_ref_count[other.m_id] += 1;
	}
}

shader_program::shader_program( const std::vector< const boost::shared_ptr<const shader> >& shaders ):
	m_linked( false ),
	m_id( 0 ),
	m_errorLog()
{
	init_shader_program( shaders );
	link_shaders();
}

shader_program::~shader_program()
{
	if( m_id != 0 ) {
		// The ref count of a shader program id should exist in the map and not be zero if the destructor is called
		assert( shader_prog_id_ref_count.find( m_id ) != shader_prog_id_ref_count.end() && shader_prog_id_ref_count[m_id] != 0 );
		
		shader_prog_id_ref_count[m_id] -= 1;

		// Free the OpenGL shader id since the shader program is no longer in use
		if( shader_prog_id_ref_count[m_id] == 0 )
			glDeleteProgram( m_id );
	}
}

void shader_program::use_program() const {
	if( !m_linked ) {
		throw std::runtime_error( "shader_program.use_program: Failed to use program because the program has not been properly linked." );
	}

	glUseProgram( m_id );
}

const GLuint shader_program::get_id() const {
	if( !m_linked ) {
		throw std::runtime_error( "shader_program.get_id: Failed to get shader program id because shaders have not been linked." );
	}

	return m_id;
}

shader_uniform_store& shader_program::get_uniform_store() const {
	if( !m_linked ) {
		throw std::runtime_error( "shader_program.get_uniform_store: Failed to get shader program's uniform store because shaders have not been linked." );
	}

	return m_store;
}

const bool shader_program::is_linked() const {
	return m_linked;
}

const std::string& shader_program::get_error_log() const {
	if( m_linked ) {
		throw std::runtime_error( "shader_program.get_compile_log: Failed to get compile log because program has been properly linked." );
	}

	return m_errorLog;
}

// Private Member Functions

void shader_program::init_shader_program( const std::vector< const boost::shared_ptr<const shader> >& shaders ) {
	GLuint genId;
	boost::unordered_set<shader_type_t> shaderTypes;

	if( m_linked )
		throw std::runtime_error( "shader_program.init_shader_program: Failed to initialize shader program because it has already been linked." );

	genId = glCreateProgram();

	if( genId == 0 )
		throw std::runtime_error( "shader_program.init_shader_progam: Failed to initialize shader program because there was an OpenGL error when creating the program." );

	if( shaders.size() < 2 )
		throw std::runtime_error( "shader_program.link_shaders: Failed to initialize shader program because there needs to be a least two shaders to link." );

	m_id = genId;
	
	// Increase refence count of the shader_program id
	if( shader_prog_id_ref_count.find( m_id ) == shader_prog_id_ref_count.end() ) {
		shader_prog_id_ref_count.insert( std::pair<const GLuint, unsigned int>( m_id, 1 ) );
	} else {
		// The ref count of the id should be 0 if it exist; OpenGL will no return two of the same id so this should only be reached if the shader program id
		// has been freed
		assert( shader_prog_id_ref_count[m_id] == 0 );

		shader_prog_id_ref_count[m_id] += 1;
	}

	// Set the id in the shader_uniform_store
	m_store.m_shaderProgId = m_id;

	// Check to make sure there are not two or more shaders of the same type in the shaders vector
	for( std::vector< const boost::shared_ptr<const shader> >::const_iterator it = shaders.begin(); it != shaders.end(); ++it ) {
		if( shaderTypes.find( (*it)->get_type() ) != shaderTypes.end() ) {
			throw std::runtime_error( "shader_program.link_shaders: Failed to initialize shader program because multiple shaders of the same type(" + 
				boost::lexical_cast<std::string>( (*it)->get_type() ) + ") were passed to the shader program" );
		}
	}

	attach_shaders( shaders );
}

void shader_program::link_shaders() {
	GLint status;

	// link_shaders should only be called after init_shader_program is called which will throw an exception if already linked
	assert( !m_linked );
	// init_shader_program will have generated an id for the shader_program and would have thrown an exception if a 0 id was generated
	assert( m_id != 0 );

	if( !m_linked && m_id != 0 ) {
		glLinkProgram( m_id );

		// Check to see if the the linking of the shader program has succeeded
		glGetProgramiv( m_id, GL_LINK_STATUS, &status );

		// Check to make sure that OpenGL has not entered an error state during linking of the shader program
		if( GL_NO_ERROR != glGetError() )
			m_errorLog = OPEN_GL_ERROR_STATE_MSG;

		if( GL_FALSE == status ) {
			// If there was error that occured during linking, populate the shader program error log
			handle_link_errors();
			m_linked = false;
		} else {
			m_linked = true;
		}
	}
}

void shader_program::attach_shaders( const std::vector< const boost::shared_ptr<const shader> >& shaders ) {
	bool vertShader = false, fragShader = false;

	for( std::vector< boost::shared_ptr<const shader> >::const_iterator it = shaders.begin(); it != shaders.end(); ++it ) {
		
		if( !(*it)->is_compiled() )
			throw std::runtime_error( "shader_program.attach_shaders: Failed to attach shaders because a shader did not compile properly." + (*it)->get_compile_log() );

		glAttachShader( m_id, (*it)->get_id() );
		
		if( GL_NO_ERROR != glGetError() )
			m_errorLog = OPEN_GL_ERROR_STATE_MSG;

		// Check to see if vertex shader or fragment shader. Needed to make sure that a shader program contains both a vertex shader and a fragment shader.
		if( (*it)->get_type() == vert_shader ) vertShader = true;
		if( (*it)->get_type() == frag_shader ) fragShader = true;
	}

	if( !vertShader || !fragShader )
		throw std::runtime_error( "shader_program.attach_shaders: Failed to attach link shaders because each shader program must contain both a vertex shader and a fragment shader." );
}

void shader_program::handle_link_errors() {
	GLint logLength;
	std::vector<GLchar> log;
	std::string logStr( "" );

	glGetProgramiv( m_id, GL_INFO_LOG_LENGTH, &logLength );
	log.resize( logLength );

	glGetProgramInfoLog( m_id, static_cast<GLsizei>( logLength ), &logLength, &log[0] );

	logStr = std::string( &log[0] );

	m_errorLog = logStr;
}

// Static Variables

const std::string shader_program::OPEN_GL_ERROR_STATE_MSG = std::string( "OpenGL error state encountered." );

std::map<const GLuint, unsigned int> shader_program::shader_prog_id_ref_count = std::map<const GLuint, unsigned int>();

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
