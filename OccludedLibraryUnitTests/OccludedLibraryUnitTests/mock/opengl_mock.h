#pragma once

/* This header file is designed for testing classes that make calls to OpenGL functions. It mimics the behaviour of these functions so that 
 * the classes can be tested without having an OpenGL context. This will need to be added to as the library need more complex functionality.
 */


// Definition necessary to allow for unit testing certain parts without importing glew
#define GL_VERTEX_SHADER 1
#define GL_TESS_CONTROL_SHADER 2
#define GL_TESS_EVALUATION_SHADER 3
#define GL_GEOMETRY_SHADER 4
#define GL_FRAGMENT_SHADER 5
#define GL_COMPUTE_SHADER 6

#define GL_STATIC_DRAW 0
#define GL_STREAM_DRAW 1
#define GL_DYNAMIC_DRAW 2

#define GLint int
#define GLuint unsigned int
#define GLenum unsigned int
#define GLchar char
#define GLsizei int
#define GLboolean bool
#define GLvoid void
#define GLsizeiptr GLsizei

#define GL_TRUE 1
#define GL_FALSE 2
#define GL_NO_ERROR 0
#define GL_COMPILE_STATUS 0
#define GL_INFO_LOG_LENGTH 0
#define GL_LINK_STATUS 0
#define GL_ARRAY_BUFFER 0

bool errorState = false;

GLuint glCreateShader( GLenum shaderType ) {
	if( errorState )
		return 0;
	else
		return 1;
}

GLuint glCreateProgram() {
	if( errorState )
		return 0;
	else
		return 1;
}

void glGetShaderiv( GLuint shader, GLenum pname, GLint* params ) {
	if( errorState )
		*params = GL_TRUE;
	else
		*params = GL_FALSE;
}

GLenum glGetError() {
	return GL_NO_ERROR;
}

void glGetShaderInfoLog( GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog ) {
	if( errorState )
		*infoLog = '\0';
	else
		infoLog = 0;
}

void glGetProgramiv( GLuint program, GLenum pname, GLint *params) {
	if( errorState )
		*params = GL_TRUE;
	else
		*params = GL_FALSE;
}

void glGetProgramInfoLog( GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog ) {
	if( errorState )
		*infoLog = '\0';
	else
		infoLog = 0;
}

GLint glGetAttribLocation( GLuint program, const GLchar *name ) {
	if( errorState )
		return 0;
	else
		return 1;
}

void glGenBuffers( GLsizei n, GLuint * buffers ) {
	if( errorState )
		*buffers = 0;
	else
		*buffers = 1;
}

GLint glGetUniformLocation( GLuint program, const GLchar* name ) {
	if( errorState )
		return -1;
	else
		return 0;
}

void glBufferData( GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {}
void glBindBuffer( GLenum target, GLuint buffer) {}
void glEnableVertexAttribArray( GLuint index ) {}
void glDisableVertexAttribArray( GLuint index ) {}
void glVertexAttribPointer(	GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer ) {}
void glAttachShader( GLuint program, GLuint shader ) {}
void glShaderSource( GLuint shader, GLsizei count, const GLchar **string, const GLint *length ) {}
void glCompileShader( GLuint shader ) {}
void glDeleteShader( GLuint shader ) {}
void glDeleteProgram( GLuint program ) {}
void glUseProgram( GLuint program ) {}
void glLinkProgram( GLuint program ) {}
void glDeleteBuffers( GLsizei n, const GLuint* buffers ) {}
