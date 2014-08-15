#pragma once

#include <glm/glm.hpp>

#include "../nodes/transformation.h"

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
	 * \fn get_model_transform
	 * \brief Gets the model transformation.
	 *
	 * \return A constant reference to the transformation object representing the model transformation of this model.
	 */
	virtual const occluded::transformation& get_model_transform() const = 0;

};

} // end of objects namespace
} // end of scene namespace
} // end of occluded namespace