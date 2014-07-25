#pragma once

#include <cassert>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#ifndef UNIT_TESTING
#include  <GL/glew.h>
#else
#include "opengl_mock.h"
#endif

namespace occluded { 

namespace opengl { namespace retained { namespace shaders {

/**
 * \enum shader_type_t
 * \brief An enum that stores the type of shader.
 *
 * An enum representing the type of shader. It is used to prevent the passing of an invalid shader type to the constructor.
 */
typedef enum SHADER_TYPE {
	vert_shader = GL_VERTEX_SHADER,
	tess_control_shader = GL_TESS_CONTROL_SHADER,
	tess_eval_shader = GL_TESS_EVALUATION_SHADER,
	geo_shader = GL_GEOMETRY_SHADER,
	frag_shader = GL_FRAGMENT_SHADER,
	compute_shader = GL_COMPUTE_SHADER
} shader_type_t;

/**
 * \class shader
 * \brief A wrapper class for an OpenGL shader.
 *
 * A wrapper class for an OpenGL GLSL shader. This is class holds information necessary for creating a GLSL shader as well as making it easy
 * for accessing information about a compiled shader without having to query OpenGL. It also allows for determining if a shader has not compiled
 * properly and for accessing the error log to examine the compilation errors.
 */
class shader 
{
private:
	static const std::string OPEN_GL_ERROR_STATE_MSG;

	std::string m_shaderSrc;
	shader_type_t m_type;

	GLuint m_id;
	bool m_compiled;

	std::string m_compileLog;

public:
	/**
	 * \brief Initializes and compiles the shader.
	 *
	 * \param shaderSrc A reference to a string containing the source code of the GLSL shader.
	 * \param type The type of this GLSL shader.
	 *
	 * Initializes and compiles the shader program specified by the shader source and the shader type provided. If the shader has a problem compiling,
	 * the compile log will be stored and an exception will be thrown.
	 */
	shader( const std::string& shaderSrc, const shader_type_t type );
	~shader();

	/**
	 * \fn get_id
	 * \brief Gets the id of the shader.
	 * \return Returns the id of the GLSL shader.
	 *
	 * Gets the id of the GLSL shader and throws an exception if an error occured during shader compilation.
	 */
	const GLuint get_id() const;

	/**
	 * \fn is_compiled
	 * \brief Returns true if the shader is compiled, and false otherwise.
	 * \return Returns whether or not the shader is compiled.
	 *
	 * Returns true if the GLSL is compiled and false if an error occured during compilation. This is used to quickly determine if a shader can be
	 * used in a GLSL shader program.
	 */
	const bool is_compiled() const;

	/**
	 * \fn get_type
	 * \brief Gets the type of the shader.
	 * \return Returns a shader_type_t representing the shader's type.
	 *
	 * Gets the shader_type_t of the shader. Used to prevent problems with incorrect types of shaders being used in the same shader_program.
	 */
	const shader_type_t get_type() const;

	/**
	 * \fn get_compile_log
	 * \brief Gets the compile log if a compilation error occurs.
	 * \return Returns a reference to the string containing the shader's compile log.
	 *
	 * Gets the the compile log of the GLSL shader if there is a problem during compilation. If there was no problem during compilation, an exception
	 * will be thrown.
	 */
	const std::string& get_compile_log() const;

private:
	/**
	 * \fn compile_shader
	 * \brief Compiles the shader.
	 *
	 * Creates the OpenGL shader, and compiles it. 
	 */
	void compile_shader();

	/**
	 * \fn handle_compile_error
	 * \brief Handles compilation errors.
	 *
	 * Handles compilation errors by populating the compile log.
	 */
	void handle_compile_error();
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace

typedef opengl::retained::shaders::shader shader;

} // end of occluded namespace
