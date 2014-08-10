#pragma once

#include <glm/glm.hpp>

namespace occluded { namespace scene { namespace nodes {

/**
 * \class transformation
 * \brief An abstract class for transformations within a scene
 * 
 * An abstract class that defines the interface all transformation classes must implement/
 */
class transformation {
public:
	/**
	 * \fn get__raw_transformation
	 * \brief Gets the raw transformation.
	 *
	 * \return A constant reference to the 4x4 matrix representing the transformation.
	 *
	 * Gets the raw 4x4 matrix representing the transformation contained by the transformation object. This is to be used by the other transformation
	 * objects to manipulate each other, and by the shaders to apply the transformations in the rendering pipeline. 
	 */
	virtual const glm::mat4& get_raw_transformation() const = 0;

};

} // end of nodes namespace
} // end of scene namespace

typedef occluded::scene::nodes::transformation transformation;

} // end of occluded namespace