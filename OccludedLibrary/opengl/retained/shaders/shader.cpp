#include "shader.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader::shader( const std::string& shaderSrc, const shader_type_t type ):
	m_status( shader_initialized ),
	m_shaderSrc( shaderSrc ),
	m_id( 0 ),
	m_type( type )
{
}


shader::~shader()
{
}

void shader::compile_shader() {
	GLint status;
	const GLchar * src = NULL;
	GLint srcLength = m_shaderSrc.size();

	if( m_shaderSrc == "" )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because source was an empty string." );
	
	if( m_status != shader_initialized )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because shader has already been compiled." );

	m_id = glCreateShader( m_type );
	assert( GL_NO_ERROR == glGetError() );

	src = m_shaderSrc.c_str();

	glShaderSource( m_id, 1, &src, &srcLength );
	assert( GL_NO_ERROR == glGetError() );

	glCompileShader( m_id );
	assert( GL_NO_ERROR == glGetError() );

	glGetShaderiv( m_id, GL_COMPILE_STATUS, &status );
	assert( GL_NO_ERROR == glGetError() );

	if( status != GL_TRUE ) {
		handle_compile_error();
		throw std::runtime_error( "shader.compile_shader: Shader was not compiled due to compile error." );
	} else {
		m_status = shader_compiled;
	}
}

const GLuint shader::get_id() const {
	if( m_status != shader_compiled )
		throw std::runtime_error( "shader.get_id: Failed to get shader id because it has not been compiled." );

	return m_id;
}

const bool shader::is_compiled() const {
	return shader_compiled == m_status;
}

const shader_type_t shader::get_type() const {
	shader_type_t thisShaderType = invalid_shader;

	if( m_status == shader_compiled )
		thisShaderType = m_type;

	return thisShaderType;
}

const std::string& shader::get_compile_log() const {
	if( m_status != shader_error )
		throw std::runtime_error( "shader.get_compile_log: Failed to get compile log because there has been no compile error" );

	return m_compileLog;
}

// private member functions

void shader::handle_compile_error() {
	GLint logLength;
	std::vector<GLchar> error;

	glGetShaderiv( m_id, GL_INFO_LOG_LENGTH, &logLength );
	assert( GL_NO_ERROR == glGetError() );

	error.resize( logLength );

	glGetShaderInfoLog( m_id, logLength, &logLength, &error[0] );
	assert( GL_NO_ERROR == glGetError() );

	m_compileLog = std::string( &error[0] );

	m_status = shader_error;
}

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
