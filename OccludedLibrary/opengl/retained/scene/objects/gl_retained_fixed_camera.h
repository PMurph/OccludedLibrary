#pragma once

#include "gl_retained_camera.h"

namespace occluded { namespace opengl { namespace retained { namespace scene { namespace objects {

/**
 * \class gl_retained_fixed_camera
 * \brief A camera with a fixed position and a fixed orientation.
 *
 * A camera with a fixed position and a fixed orientation that are set at the creation of the camera. This is for the simple creation
 * of a camera for a scene where the scene may change but the view of the scene does not.
 */
class gl_retained_fixed_camera:
	public gl_retained_camera
{
public:
	/**
	 * \brief Initializes an OpenGL fixed camera
	 *
	 * \param shaderProg A reference to the shader program used for rendering.
	 * \param proj A reference to a 4x4 matrix representing the projection transformation of the camera.
	 * \param view A reference to a 4x4 matrix representing the view transformation of the camera.
	 *
	 * Initializes an OpenGL camera that has fixed projection and view transformations.
	 */
	gl_retained_fixed_camera( const shader_program& shaderProg, const glm::mat4& proj, const glm::mat4& view );
	~gl_retained_fixed_camera();

	/**
	 * \fn set_for_render
	 * \brief Sets this camera to the camera that will be used for rendering.
	 *
	 * Sets this camera as the camera to be used for next render. Throws an exception if attached shader_program is not properly linked.
	 */
	void set_for_render() const;

	/**
	 * \fn get_projection
	 * \brief Gets the projection transformation.
	 *
	 * \return A 4x4 matrix containing the projection transformation.
	 */
	const glm::mat4& get_projection() const;

	/**
	 * \fn get_view
	 * \brief Gets the view transformation.
	 *
	 * \return A 4x4 matrix containing the view transformation.
	 */
	const glm::mat4& get_view() const;
};

} // end of objects namespace
} // end of scene namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
