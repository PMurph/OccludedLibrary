#pragma once

#include <map>

#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef UNIT_TESTING
#include <GL/glew.h>
#else
#include "opengl_mock.h"
#endif

#include "shader_program.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

// When more types are needed add them to this typedef
typedef boost::variant<glm::vec3, glm::mat4> uniform_value;

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

	/**
	 * \brief Initializes an uniform store and populates it.
	 *
	 * \param shaderProg A reference to a shader_program.
	 * \param values A reference to a vector of values to populate the store with.
	 */
	shader_uniform_store( const shader_program& shaderProg, const std::vector< std::pair<const std::string, const uniform_value> >& values );
	~shader_uniform_store();

	/**
	 * \fn add_uniform
	 * \brief Adds a new uniform value to the uniform store.
	 *
	 * \param name A reference to string representing the name of the uniform value to be stored.
	 * \param value A refrence to the value to be added.
	 *
	 * Adds a new uniform to the uniform store. This is to be used for a value that is not already in the store.
	 */
	void add_uniform( const std::string& name, const uniform_value& value );

	/**
	 * \fn set_uniform_value
	 * \brief Sets the value of a uniform.
	 *
	 * \param name A reference to string representing the name of the uniform to be updated.
	 * \param value A reference to a uniform value to be set.
	 *
	 * Sets a uniform to a new value. Throws an exception if no uniform of the name is found or the type of passed value does not match the type
	 * of the current value.
	 */
	void set_uniform_value( const std::string& name, const uniform_value& value );

	/**
	 * \fn pass_to_shader
	 * \brief Passes the uniform values to the shader program.
	 *
	 * Passes the uniform values to shader program, so that they can be used to render a scene.
	 */
	void pass_to_shader() const;

	/**
	 * \fn get_value
	 * \brief Gets the value of a uniform.
	 *
	 * \param name A reference to string representing the name of the uniform to retrieve.
	 * \return A value of type T representing the current value of the uniform.
	 *
	 * Gets the current value stored in the uniform store for a specified uniform. If a uniform name is passed that isn't part of the uniform
	 * store, an exception will be thrown.
	 */
	template<typename T>
	const T get_value( const std::string& name ) const {
		accessor_visitor<T> accessor;
		std::map< const std::string, std::pair<GLint, uniform_value> >::const_iterator currValue;

		currValue = m_store.find( convert_to_uniform_name( name ) );

		if( currValue == m_store.end() ) {
			throw std::runtime_error( "shader_uniform_store.get_value: Failed to get value because uniform does not exist in the uniform store." );
		}

		return currValue->second.second.apply_visitor( accessor );
	}

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

	/**
	 * \fn populate_store
	 * \brief Populates the uniform store.
	 *
	 * \param values A reference to a vector of pairs of names and values.
	 *
	 * Iterates through the vector of name-value pairs and adds each of them to the uniform store.
	 */
	void populate_store( const std::vector< std::pair<const std::string, const uniform_value> >& values );

	/**
	 * \class accessor_visitor
	 * \brief Gets the value stored in a uniform
	 */
	template <typename T>
	class accessor_visitor:
		public boost::static_visitor<T>
	{
	private:
		T m_val;

	public:
		const T& operator()( const T& stored ) const {
			return stored;
		}

		template <typename Other>
		const T& operator()( const Other& stored ) const {
			throw std::runtime_error( "shader_uniform_store::accessor_visitor: Failed to get value because uniform value is of the incorrect type." );
			
			return m_val;
		}
	};

	/**
	 * \class gl_uniform_visitor
	 * \brief Calls the appropriate glUniform* function for a uniform value.
	 */
	class gl_uniform_visitor:
		public boost::static_visitor<>
	{
	private:
		GLint m_id;

	public:
		gl_uniform_visitor( const GLint id ): 
			m_id( id ) 
		{
			assert( id != -1 );
		}

		void operator()( const glm::vec3& stored ) const {
			glUniform3fv( m_id, 1, glm::value_ptr( stored ) );
		}

		void operator()( const glm::mat4& stored ) const {
			glUniformMatrix4fv( m_id, 1, false, glm::value_ptr( stored ) );
		}
	};
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
