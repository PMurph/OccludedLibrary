#pragma once

#include <boost/ref.hpp>

#include "../../../../scene/objects/camera.h"
#include "../../shaders/shader_program.h"

namespace occluded { namespace opengl { namespace retained { namespace scene { namespace objects {

/**
 * \class gl_retained_camera
 * \brief An abstract OpenGL camera class which contains the necessary method that any retained camera will use.
 *
 * This is an abstract class for a camera that uses the OpenGL retained mode to render objects in the scene. It provides a number of
 * functions that are common to all retained cameras.
 */
class gl_retained_camera:
	public occluded::camera
{
private:
	static const std::string SHADER_PROJECTION_UNIFORM_NAME;
	static const std::string SHADER_VIEW_UNIFORM_NAME;

protected:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	
	boost::reference_wrapper<const shader_program> m_shaderProg;

public:
	gl_retained_camera( const shader_program& shaderProg, const glm::mat4& projection, const glm::mat4& view );
	~gl_retained_camera();

	/**
	 * \fn set_shader_program
	 * \brief Changes the shader program used by the camera.
	 *
	 * \param newShaderProgram A reference to a shader_program object.
	 *
	 * Changes the shader program used by the camera to a new shader program. An exception is thrown if the newShaderProgram is not a linked
	 * shader program. This is to make it so that when a scene contains multiple objects that use different shader programs, the projection and
	 * view uniforms for each shader program can be set quickly.
	 */
	void set_shader_program( const shader_program& newShaderProg );

	/**
	 * \fn get_shader_program
	 * \brief Gets the shader_program currently being used by this camera.
	 *
	 * \return A constant reference to the shader_program currently being used.
	 */
	const shader_program& get_shader_program() const;

protected:
	/**
	 * \fn set_uniform_vals
	 * \brief Sets the view and projection uniforms of the shader_program.
	 */
	void set_uniform_vals() const;

private:
	/**
	 * \fn check_store
	 * \brief Checks to see if the values exist in the shader_program's uniform store.
	 */
	void check_store() const;

};

} // end of objects namespace
} // end of scene namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace