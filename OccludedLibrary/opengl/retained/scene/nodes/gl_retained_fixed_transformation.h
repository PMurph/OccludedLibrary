#pragma once

#include "../../../../scene/nodes/transformation.h"

namespace occluded { namespace opengl { namespace retained { namespace scene { namespace nodes {

/**
 * \class gl_retain
 */
class gl_retained_fixed_transformation:
	public occluded::transformation
{
private:
	glm::mat4 m_transform;

public:
	/**
	 * \brief Constructs the fixed transformation object.
	 *
	 * Constructs a fixed transformation object containing the identity transformation.
	 */
	gl_retained_fixed_transformation();

	/**
	 * \brief Constructs the fixed transformation object with a starting value.
	 *
	 * \param transform A reference to a 4x4 matrix representing the starting value of the fixed transformation.
	 */
	gl_retained_fixed_transformation( const glm::mat4& transform );
	~gl_retained_fixed_transformation();

	/**
	 * \fn get_raw_transformation
	 * \brief Gets the raw transformation.
	 *
	 * \return A constant reference to a 4x4 matrix representing the raw transformation.
	 */
	const glm::mat4& get_raw_transformation() const;

	/**
	 * \fn set_transformation
	 * \brief Sets the transformation to a new transformation.
	 *
	 * \param new_transform A reference to the transformation containing the transformation.
	 *
	 * Changes the stored transformation to a new transformation represented by the 4x4 matrix returned by the new_transform parameter's 
	 * get_raw_transformation function.
	 */
	void set_transformation( const occluded::transformation& new_transform );
};

} // end of nodes namespace
} // end of scene namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace