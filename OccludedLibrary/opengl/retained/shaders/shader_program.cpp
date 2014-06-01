#include "shader_program.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader_program::shader_program():
	m_linked( false ),
	m_id( 0 ),
	m_errorLog( "No Error: Empty shader program." )
{
}

shader_program::shader_program( const std::vector<shader>& shaders ):
	m_linked( false ),
	m_id( 0 ),
	m_errorLog()
{
	init_shader_program( shaders );
	link_shaders();
}

shader_program::~shader_program()
{
}

void shader_program::use_program() {
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
const bool shader_program::is_linked() const {
	return m_linked;
}

const std::string& shader_program::get_error_log() const {
	std::string log("");

	if( m_linked ) {
		throw std::runtime_error( "shader_program.get_compile_log: Failed to get compile log because program has been properly linked." );
	}

	return m_errorLog;
}

// Private Member Functions

void shader_program::init_shader_program( const std::vector<shader>& shaders ) {
	if( m_linked )
		throw std::runtime_error( "shader_program.init_shader_program: Failed to initialize shader program because it has already been linked." );

	m_id = glCreateProgram();

	if( m_id == 0 )
		throw std::runtime_error( "shader_program.init_shader_progam: Failed to initialize shader program because there was an OpenGL error when creating the program." );

	if( m_shaders.size() < 2 )
		throw std::runtime_error( "shader_program.link_shaders: Failed to link shaders because there needs to be a least two shaders to link." );

	attach_shaders( shaders );
}

void shader_program::link_shaders() {
	GLint status;

	// link_shaders should only be called after init_shader_program is called which will throw an exception if already linked
	assert( !m_linked );
	// init_shader_program will have generated an id for the shader_program and would have thrown an exception if a 0 id was generated
	assert( m_id != 0 );

	glLinkProgram( m_id );

	glGetProgramiv( m_id, GL_LINK_STATUS, &status );

	if( GL_FALSE == status ) {
		handle_link_errors();
		m_linked = false;
	} else {
		m_linked = true;
	}
}

void shader_program::attach_shaders( const std::vector<shader>& shaders ) {
	bool vertShader = false, fragShader = false;

	for( std::vector<shader>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it ) {
		if( !it->is_compiled() )
			throw std::runtime_error( "shader_program.attach_shaders: Failed to attach shaders because a shader did not compile properly." + it->get_compile_log() );

		glAttachShader( m_id, it->get_id() );
		
		if( glGetError() == GL_NO_ERROR )
			throw std::runtime_error( "shader_program.attach_shaders: OpenGL error thrown when trying to attach shader." );

		// Check to see if vertex shader or fragment shader. Needed to make sure that a shader program contains both a vertex shader and a fragment shader.
		if( it->get_type() == vert_shader ) vertShader = true;
		if( it->get_type() == frag_shader ) fragShader = true;
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

	for( std::vector<GLchar>::iterator it = log.begin(); it != log.end(); ++it ) {
		logStr.push_back( *it );
	}

	logStr.push_back( '\n' );

	// Adds all the logs 
	for( std::vector<shader>::const_iterator it = m_shaders.begin(); it != m_shaders.end(); ++it ) {
		if( !it->is_compiled() ) {
			logStr += it->get_compile_log();
			logStr.push_back( '\n' );
		}
	}

	m_errorLog = logStr;

	throw std::runtime_error( "shader_program.handle_link_errors: Failed to link shader programs due to link errors." );
}

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
