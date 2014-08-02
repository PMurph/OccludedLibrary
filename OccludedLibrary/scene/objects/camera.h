#include <glm/glm.hpp>

namespace occluded { namespace scene { namespace objects {

/**
 * \class camera
 * \brief An abstract class that contains all the methods other camera sub classes must implement.
 *
 * An abstract class that contains all the methods other camera sub classes must implement. A camera contains all the information
 * necessary for displaying a scene of objects from a specific point of view. It specifies what the view volumes shape is, where that
 * view volume is located, how the the view volume is orientated, and how objects in that view volume are projected onto a 2d surface that 
 * the user sees. This usually is implemented by two transformations:
 *	1) The view transformation
 *	2) The projection transformation
 */

class camera
{
public:
	/**
	 * \fn set_for_render
	 * \brief Sets this camera to the camera that will be used for rendering.
	 *
	 * Sets up the OpenGL state so that the scene will be render using the transformation contained within this camera.
	 */
	virtual void set_for_render() const = 0;

	/**
	 * \fn get_projection
	 * \brief Gets the projection transformation.
	 *
	 * \return A 4x4 matrix containing the projection transformation.
	 */
	virtual const glm::mat4& get_projection() const = 0;

	/**
	 * \fn get_view
	 * \brief Gets the view transformation.
	 *
	 * \return A 4x4 matrix containing the view transformation.
	 */
	virtual const glm::mat4& get_view() const = 0;
};

} // end of objects namespace
} // end of scene namespace

typedef scene::objects::camera camera;

} // end of occluded namespace