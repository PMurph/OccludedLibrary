#pragma once

#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>
#include <GL/glew.h>

#include "shader.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

/**
 * \class shader_program
 * \brief A wrapper class for an OpenGL GLSL shader program.
 *
 * A wrapper class for an OpenGL GLSL shader program. Used to create, attach shaders to, link, and use a shader program as well as to store and quickly access information
 * about a shader program with out having to query OpenGL directly. It also makes it easy to determine if a shader program is being used incorrectly by throwing exceptions
 * when a function is called while the shader program is in the incorrect state or if invalid arguments are passed to it.
 */
class shader_program
{
private:
	GLuint m_id;
	bool m_linked;
	std::string m_errorLog;

public:
	/**
	 * \brief Creates an empty shader program that cannot be used for rendering.
	 *
	 * Create an empty shader program that cannot be used for rendering. This is used to make it easier for the programmer to use and not violate the programming standards of
	 * his/her project.
	 */
	shader_program();

	/**
	 * \brief Creates and initializes a OpenGL GLSL shader program.
	 *
	 * \param shaders A vector of shader_ptrs to shaders that will be attached to the shader program.
	 *
	 * Creates a shader program, attaches the shaders in the shaders vector, and links the shader program. If everything works correctly the shader program can be used for
	 * rendering. If a problem occurs, the error log will be populated and an exception will be raised by this constructor.
	 */
	shader_program( const std::vector< const boost::shared_ptr<const shader> >& shaders );
	~shader_program();

	/**
	 * \fn use_program
	 * \brief Tells OpenGL to use the shader program.
	 *
	 * Tells OpenGL to use this GLSL shader program for rendering. This function will throw an exception if the shader program was not linked properly. Used to quickly tell
	 * the user that there is a problem with the shader program with out causing OpenGL to enter an error state.
	 */
	void use_program() const;

	/**
	 * \fn get_id
	 * \brief Gets the id of the shader program
	 * \return Returns the id of the OpenGL GLSL shader program. 
	 *
	 * Returns the id of the OpenGL GLSL shader program. This function will throw an exception if the shader program was not properly linked. This can be but should not be
	 * used to make a call to glUseProgram. Should be used for getting or binding the location of attributes and uniforms.
	 * \warning Do not use for making a call to glDeleteProgram because it will cause an error when the deconstructor tries to delete the program.
	 */
	const GLuint get_id() const;

	/**
	 * \fn is_linked
	 * \brief Returns true if the shader program is linked.
	 * \return Returns a boolean which is true if the program has been properly linked, and false otherwise.
	 *
	 * Returns true if the shader program has been properly linked, and false if a problem occured during the attaching or linking of shaders. Used to quickly detemine if
	 * it is safe to call other functions of the shader program.
	 */
	const bool is_linked() const;

	/**
	 * \fn get_error_log
	 * \brief Gets the error log of the shader program
	 * \return Returns a reference to the string containing the error log of the shader program
	 *
	 * Returns a reference to a string containing the error log of the shader program if the shader program encountered problems during the linking stage. If the program
	 * has been properly linked, this function will throw an exception. Used to easily determine what the errors during the linking process are.
	 */
	const std::string& get_error_log() const;

private:
	/**
	 * \fn init_shader_program
	 * \brief Initializes the shader program
	 * 
	 * \param shaders A vertor of shared_ptrs to GLSL shaders that will be attached to the shader program.
	 * 
	 * Creates the shader program and checks for problems that may be caused by the improper combination of shaders in the shaders vector.
	 */
	void init_shader_program( const std::vector< const boost::shared_ptr<const shader> >& shaders );

	/**
	 * \fn link_shaders
	 * \brief Links the shader program
	 * 
	 * Links the shader program.
	 */
	void link_shaders();

	/**
	 * \fn attach_shaders
	 * \brief Attaches the shaders to the shader program.
	 *
	 * \param A vector of shared_ptrs to GLSL shaders that will be attached to the shader program
	 *
	 * Attaches the shaders in the shaders vector to the shader program, so that they can be linked.
	 */
	void attach_shaders( const std::vector< const boost::shared_ptr< const shader> >& shaders );

	/**
	 * \fn handle_link_errors
	 * \brief Handles any link errors.
	 *
	 * Handles errors that occur during the linking process by querying the OpenGL link log and populating the errorLog string with the OpenGL link log's 
	 * contents
	 */
	void handle_link_errors();
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace

typedef opengl::retained::shaders::shader_program shader_program;

} // end of occluded namespace
