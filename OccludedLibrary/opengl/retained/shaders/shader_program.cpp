#include "shader_program.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader_program::~shader_program()
{
}

shader_program::shader_program( const std::string& vertShaderSrc, const std::string& tessControlShaderSrc, const std::string& tessEvalShaderSrc,
		const std::string& geoShaderSrc, const std::string& fragShaderSrc, const std::string& computeShaderSrc ):
	m_linked( false ),
	m_id( 0 ),
	m_errorLog( "" )
{
	init_shaders( vertShaderSrc, tessControlShaderSrc, tessEvalShaderSrc, geoShaderSrc, fragShaderSrc, computeShaderSrc );
}

void shader_program::link_shaders() {
	GLint status;

	assert( !m_linked );
	assert( m_shaders.size() >= 2 );

	if( m_linked ) {
		throw std::runtime_error( "shader_program.link_shaders: Failed to link shaders because shaders have already been linked." );
	}

	if( m_shaders.size() < 2 ) {
		throw std::runtime_error( "shader_program.link_shaders: Failed to link shaders because not enough shaders have been initialized." );
	}

	glLinkProgram( m_id );
	assert( GL_NO_ERROR == glGetError() );

	glGetProgramiv( m_id, GL_LINK_STATUS, &status );
	assert( GL_NO_ERROR == glGetError() );

	if( GL_FALSE == status ) {
		handle_link_errors();
		assert( false );
		m_linked = false;
	} else {
		m_linked = true;
	}
}

void shader_program::use_program() {
	assert( m_linked );

	if( !m_linked ) {
		throw std::runtime_error( "shader_program.use_program: Failed to use program because the program has not been properly linked." );
	}

	glUseProgram( m_id );
	assert( GL_NO_ERROR == glGetError() );
}

const GLuint shader_program::get_id() const {
	assert( m_linked );

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

	assert( !m_linked );

	if( m_linked ) {
		throw std::runtime_error( "shader_program.get_compile_log: Failed to get compile log because program has been properly linked." );
	}

	return m_errorLog;
}

// Private Member Functions

void shader_program::init_shaders( const std::string& vertShaderSrc, const std::string& tessControlShaderSrc, const std::string& tessEvalShaderSrc,
		const std::string& geoShaderSrc, const std::string& fragShaderSrc, const std::string& computeShaderSrc ) {
	if( m_linked ) {
		throw std::runtime_error( "shader_program.init_shaders: Failed to inialize shaders because shaders have already been linked." );
	}

	if( vertShaderSrc == "" || fragShaderSrc == "" ) {
		throw std::runtime_error( "shader_program.init_shaders: Failed to initialize shaders because either the vertex or fragment shader source was not specified.");
	}

	m_id = glCreateProgram();
	assert( GL_NO_ERROR == glGetError() );

	if( vertShaderSrc != "" )
		init_vert_shaders( vertShaderSrc );

	if( tessControlShaderSrc != "" )
		init_tess_control_shader( tessControlShaderSrc );

	if( tessEvalShaderSrc != "" )
		init_tess_eval_shader( tessEvalShaderSrc );

	if( geoShaderSrc != "" )
		init_geo_shader( geoShaderSrc );

	if( fragShaderSrc != "" )
		init_frag_shader( fragShaderSrc );

	if( computeShaderSrc != "" )
		init_comp_shader( computeShaderSrc );

	attach_shaders();
}

void shader_program::init_vert_shaders( const std::string& vertShaderSrc ) {
	m_shaders.push_back( shader( vertShaderSrc, vert_shader ) );

	m_shaders.back().compile_shader();

	assert( m_shaders.back().is_compiled() );
}

void shader_program::init_tess_control_shader( const std::string& tessControlShaderSrc ) {
	m_shaders.push_back( shader( tessControlShaderSrc, tess_control_shader ) );

	m_shaders.back().compile_shader();

	assert( m_shaders.back().is_compiled() );
}

void shader_program::init_tess_eval_shader( const std::string& tessEvalShaderSrc ) {
	m_shaders.push_back( shader( tessEvalShaderSrc, tess_eval_shader ) );

	m_shaders.back().compile_shader();

	assert( m_shaders.back().is_compiled() );
}

void shader_program::init_geo_shader( const std::string& geoShaderSrc ) {
	m_shaders.push_back( shader( geoShaderSrc, geo_shader ) );

	m_shaders.back().compile_shader();

	assert( m_shaders.back().is_compiled() );
}

void shader_program::init_frag_shader( const std::string& fragShaderSrc ) {
	m_shaders.push_back( shader( fragShaderSrc, frag_shader ) );

	m_shaders.back().compile_shader();

	assert( m_shaders.back().is_compiled() );
}

void shader_program::init_comp_shader( const std::string& computeShaderSrc ) {
	m_shaders.push_back( shader( computeShaderSrc, compute_shader ) );

	m_shaders.back().compile_shader();

	assert( m_shaders.back().is_compiled() );
}

void shader_program::attach_shaders() {
	for( std::vector<shader>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it ) {
		if( !it->is_compiled() ) {
			throw std::runtime_error( "shader_program.attach_shaders: Failed to attach shaders because a shader did not compile properly." + it->get_compile_log() );
		}

		glAttachShader( m_id, it->get_id() );
		assert( GL_NO_ERROR == glGetError() );
	}
}

void shader_program::handle_link_errors() {
	GLint logLength;
	std::vector<GLchar> log;
	std::string logStr( "" );

	glGetProgramiv( m_id, GL_INFO_LOG_LENGTH, &logLength );
	assert( GL_NO_ERROR == glGetError() );

	log.resize( logLength );

	glGetProgramInfoLog( m_id, static_cast<GLsizei>( logLength ), &logLength, &log[0] );
	assert( GL_NO_ERROR == glGetError() );

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
