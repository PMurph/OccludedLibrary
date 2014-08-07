#pragma once

#include <glm/glm.hpp>

namespace occluded { namespace scene { namespace nodes {

/**
 * \class transformation
 * \brief An abstract class for transformations within a scene
 * 
 * An abstract class that defines the interface all transformation classes must implement/
 */
class transoformation {
public:
	/**
	 * \fn get_transformation
	 * \brief Gets the raw transformation.
	 *
	 * \return A constant reference to the 4x4 matrix representing the transformation.
	 */
	virtual const glm::mat4& get_transformation() const = 0;

};

} // end of nodes namespace
} // end of scene namespace
} // end of occluded namespace