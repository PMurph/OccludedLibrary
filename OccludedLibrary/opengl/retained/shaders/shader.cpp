#include "shader.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader::shader( const std::string& shaderSrc, const shader_type_t type ):
	m_status( shader_initialized ),
	m_shaderSrc( shaderSrc ),
	m_id( 0 ),
	m_type( type )
{
	compile_shader();
}


shader::~shader()
{
	if( m_id != 0 )
		glDeleteShader( m_id );
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

void shader::compile_shader() {
	GLint status;
	const GLchar * src = NULL;
	GLint srcLength = static_cast<GLint>( m_shaderSrc.size() );

	if( m_shaderSrc == "" )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because source was an empty string." );
	
	if( m_status != shader_initialized )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because shader has already been compiled." );

	m_id = glCreateShader( m_type );

	if( m_id == 0 )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because an error occured while creating the shader." );

	src = m_shaderSrc.c_str();
	glShaderSource( m_id, 1, &src, &srcLength );

	glCompileShader( m_id );

	// Check the compile status of the shader
	glGetShaderiv( m_id, GL_COMPILE_STATUS, &status );

	// Check to see if the compiling of the shader has caused an error in OpenGL
	if( GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "shader.compile_shader: Compiling of the shader caused OpenGL to enter an error state(" + boost::lexical_cast<std::string>( glGetError() ) + ")." );

	if( status != GL_TRUE ) {
		// If the shader did not compile properly, populate the error log and throw an exception
		handle_compile_error();
		throw std::runtime_error( "shader.compile_shader: Shader was not compiled due to compile error." );
	} else {
		m_status = shader_compiled;
	}
}

void shader::handle_compile_error() {
	GLint logLength;
	std::vector<GLchar> error;

	// Get the number of characters in the compile log
	glGetShaderiv( m_id, GL_INFO_LOG_LENGTH, &logLength );
	error.resize( logLength );

	// Get the contents of the compile log
	glGetShaderInfoLog( m_id, logLength, &logLength, &error[0] );

	m_compileLog = std::string( &error[0] );

	m_status = shader_error;
}

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
