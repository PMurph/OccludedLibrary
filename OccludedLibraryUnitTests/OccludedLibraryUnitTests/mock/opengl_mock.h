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

#define GL_POINTS 0
#define GL_LINES 1
#define GL_LINE_STRIP 2
#define GL_LINE_LOOP 3
#define GL_TRIANGLES 4
#define GL_TRIANGLE_STRIP 5
#define GL_TRIANGLE_FAN 6
#define GL_PATCHES 7

#define GLint int
#define GLuint unsigned int
#define GLenum unsigned int
#define GLchar char
#define GLsizei int
#define GLboolean bool
#define GLvoid void
#define GLfloat float
#define GLsizeiptr GLsizei

#define GL_TRUE 1
#define GL_FALSE 2
#define GL_NO_ERROR 0
#define GL_ERROR 1
#define GL_LINK_STATUS 0
#define GL_ARRAY_BUFFER 0
#define GL_ELEMENT_ARRAY_BUFFER 1

#define GL_COMPILE_STATUS 0
#define GL_INFO_LOG_LENGTH 0

#define GL_UNSIGNED_BYTE 0
#define GL_UNSIGNED_SHORT 1
#define GL_UNSIGNED_INT 2

extern bool errorState; // If true, the mock should mimic OpenGL functions returning errors
extern bool programLinkError; // If true, the mock will return GL_FALSE when glGetProgramiv is called
static GLuint currVAOID = 1;
static GLuint currVBOID = 1;
static GLuint currShaderProgID = 1;

inline GLuint glCreateShader( GLenum shaderType ) {
	return 1;
}

inline GLuint glCreateProgram() {
	if( errorState )
		return 0;

	currShaderProgID++;
	return currShaderProgID - 1;
}

inline void glGetShaderiv( GLuint shader, GLenum pname, GLint* params ) {
	if( errorState )
		*params = GL_FALSE;
	else
		*params = GL_TRUE;
}

inline GLenum glGetError() {
	if( errorState )
		return GL_ERROR;
	else
		return GL_NO_ERROR;
}

inline void glGetShaderInfoLog( GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog ) {
	if( errorState )
		*infoLog = '\0';
	else
		infoLog = 0;
}

inline void glGetProgramiv( GLuint program, GLenum pname, GLint *params) {
	if( programLinkError )
		*params = GL_FALSE;
	else
		*params = GL_TRUE;
}

inline void glGetProgramInfoLog( GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog ) {
	if( errorState )
		*infoLog = '\0';
	else
		infoLog = 0;
}

inline GLint glGetAttribLocation( GLuint program, const GLchar *name ) {
	if( errorState )
		return 0;
	else
		return 1;
}

inline void glGenVertexArrays( GLsizei n, GLuint* arrays ) {
	if( errorState )
		*arrays = 0;
	else {
		*arrays = currVAOID;
		currVAOID++;
	}
}

inline void glGenBuffers( GLsizei n, GLuint* buffers ) {
	if( errorState )
		*buffers = 0;
	else {
		*buffers = currVBOID;
		currVBOID++;
	}
}

inline GLint glGetUniformLocation( GLuint program, const GLchar* name ) {
	if( errorState )
		return -1;
	else
		return 0;
}

inline void glBufferData( GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {}
inline void glBindVertexArray( GLuint array ) {}
inline void glBindBuffer( GLenum target, GLuint buffer) {}
inline void glEnableVertexAttribArray( GLuint index ) {}
inline void glDisableVertexAttribArray( GLuint index ) {}
inline void glVertexAttribPointer(	GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer ) {}
inline void glAttachShader( GLuint program, GLuint shader ) {}
inline void glShaderSource( GLuint shader, GLsizei count, const GLchar **string, const GLint *length ) {}
inline void glCompileShader( GLuint shader ) {}
inline void glDeleteShader( GLuint shader ) {}
inline void glDeleteProgram( GLuint program ) {}
inline void glUseProgram( GLuint program ) {}
inline void glLinkProgram( GLuint program ) {}
inline void glDeleteBuffers( GLsizei n, const GLuint* buffers ) {}
inline void glUniform3fv( GLint location, GLsizei count, const GLfloat *value ) {}
inline void glUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value ) {}
inline void glDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices ) {}

inline void resetVBOIDs() {
	currVBOID = 1;
}

inline void resetVAOIDs() {
	currVAOID = 1;
}