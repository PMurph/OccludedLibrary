#pragma once

#include <glm/glm.hpp>

namespace occluded { namespace scene { namespace objects {

/**
 * \class model
 * \brief An abstract class representing a model.
 *
 * An abstract class representing a model in a scene. A model is any object or collection of objects that is part of the scene and can be rendered.
 */
class model {
private:
	glm::mat4 m_model;

public:
	/**
	 * \fn get_model
	 * \brief Gets the model transformation.
	 *
	 * \return A reference to a 4x4 matrix representing the model transformation of this model.
	 */
	virtual const glm::mat4& get_model() const = 0;

};

} // end of objects namespace
} // end of scene namespace
} // end of occluded namespace