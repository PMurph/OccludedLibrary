#include "shader.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader::shader( const std::string& shaderSrc, const shader_type_t type ):
	m_compiled( false ),
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
	if( !m_compiled )
		throw std::runtime_error( "shader.get_id: Failed to get shader id because it has not been compiled." );

	return m_id;
}

const bool shader::is_compiled() const {
	return m_compiled;
}

const shader_type_t shader::get_type() const {
	shader_type_t thisShaderType;

	if( !m_compiled )
		throw std::runtime_error( "shader.get_type: Failed to get shader type because it has not been compiled." );

	thisShaderType = m_type;

	return thisShaderType;
}

const std::string& shader::get_compile_log() const {
	if( m_compiled )
		throw std::runtime_error( "shader.get_compile_log: Failed to get compile log because there has been no compile error" );

	return m_compileLog;
}

// private member functions

void shader::compile_shader() {
	GLuint genId;
	GLint status;
	const GLchar * src = NULL;
	GLint srcLength = static_cast<GLint>( m_shaderSrc.size() );

	// Shader should not be compiled at this point
	assert( !m_compiled );

	if( m_shaderSrc == "" )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because source was an empty string." );

	genId = glCreateShader( m_type );

	if( genId == 0 )
		throw std::runtime_error( "shader.compile_shader: Failed to compile shader because an error occured while creating the shader." );

	m_id = genId;

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
	} else {
		m_compiled = true;
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

	m_compiled = false;
}

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
