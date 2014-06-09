#pragma once

#include <map>

#include <boost/variant.hpp>

#include <glm/glm.hpp>

#include "shader_program.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

// When more types are needed add them to this typedef
typedef boost::variant<glm::mat4> uniform_value;

/**
 * \class shader_uniform_store
 * \brief Stores uniform values for a particular shader program.
 *
 * Stores the uniform values for a particular shader program by maintaining a mapping from uniforms' names to their values. This
 * class provides a way of storing all the uniform values of a shader_program. By convention any name provided will be converted
 * to the uniform equivalent which has a u prefixed to the beginning and the first letter of the original name capitalized( "model"
 * becomes "uModel" ).
 */
class shader_uniform_store
{
private:
	const shader_program& m_shaderProg;

	std::map< const std::string, std::pair<GLint, uniform_value> > m_store;

public:
	/**
	 * \brief Initializes an empty uniform store.
	 * 
	 * \param shaderProg A reference to a shader_program.
	 */
	shader_uniform_store( const shader_program& shaderProg );
	~shader_uniform_store();

	/**
	 * \fn add_uniform_value
	 * \brief Adds a new uniform value to the uniform store.
	 *
	 * \param name A reference to string representing the name of the uniform value to be stored.
	 * \param value A refrence to the value to be added.
	 *
	 * Adds a new uniform to the uniform store. This is to be used for a value that is not already in the store.
	 */
	void add_uniform( const std::string& name, const uniform_value& value );

private:
	/**
	 * \fn convert_to_uniform_name
	 * \brief Converts a name to the equivalent uniform variable name.
	 *
	 * \param A reference to a string representing the unconverted name.
	 * \return A string representing the converted name.
	 *
	 * Converts a name to the equivalent uniform variable name by converting the first letter to capital and prefixing the 
	 * name with a 'u'.
	 */
	const std::string convert_to_uniform_name( const std::string& name ) const;

};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
