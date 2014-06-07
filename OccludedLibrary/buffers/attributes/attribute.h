#pragma once

#include <string>

namespace occluded { namespace buffers { namespace attributes {

/**
 * \enum attribute_t
 * \brief Stores the type of the attribute
 *
 * Stores the type of the attribute, so that attributes can be easily compared and appropriate values can be assigned to it.
 */
typedef enum ATTRIB_TYPE {
	attrib_float,
	attrib_uint,
	attrib_int,
	attrib_invalid
} attribute_t;

/**
 * \class attribute
 * \brief Stores metadata about a value or a set of values.
 *
 * Stores metadata about a value or a set of values. This is to used by an attribute_map.
 * \see { occluded::buffers::attributes::attribute_map }.
 */
class attribute
{
private:
	static const std::size_t ATTRIB_SIZE;

	std::string m_name;
	unsigned int m_arity;
	attribute_t m_type;
	bool m_normalized;
public:
	/**
	 * \brief Initializes the attribute.
	 *
	 * \param name A reference to string representing the name of the attribute.
	 * \param arity An unsigned int representing the number of primitives in the attribute.
	 * \param type An attribute_t representing the type of the primitive of the attribute.
	 * \param normalized An optional boolean representing whether or not the attribute needs to be normalized. The default value is false.
	 *
	 * Initializes the attribute using the information provided by the parameters. The normalized parameter has a default value of false.
	 */
	attribute( const std::string& name, const unsigned int arity, const attribute_t type, const bool normalized = false );
	~attribute();

	/**
	 * \fn get_name
	 * \brief Gets the name of the attribute.
	 *
	 * \return Returns a reference to string representing the name of the attribute.
	 */
	const std::string& get_name() const;

	/**
	 * \fn get_arity
	 * \brief Gets the arity of the attribute.
	 * 
	 * \return Returns an unsigned int representing the arity of the attribute.
	 */
	const unsigned int get_arity() const;

	/**
	 * \fn get_type
	 * \brief Get the type of the attribute
	 *
	 * \return Returns an attribute_t representing the type of the attribute
	 */
	const attribute_t get_type() const;

	/**
	 * \fn get_component_size
	 * \brief Gets the size of the individual primitives that make up the attribute.
	 *
	 * \return Returns a std::size_t representing the size(in bytes) of the individual primitives of the attribute.
	 */
	const std::size_t get_component_size() const;

	/**
	 * \fn get_attrib_size
	 * \brief Gets the size of a single value of the attribute.
	 *
	 * \return Returns a std::size_t representing the size(in bytes) of a single value of the attribute.
	 */
	const std::size_t get_attrib_size() const;

	/**
	 * \fn is_normalized
	 * \brief Gets whether or not the attribute's values need to be normalized
	 *
	 * \return Returns a boolean representing whether or not the attribute's values needed to be normalized.
	 */
	const bool is_normalized() const;

	/**
	 * \brief Gets whether or not the two attributes are the same.
	 * 
	 * \param other The other attribute that is being tested against.
	 * \return Returns true if the two attributes have the same name, arity, and type, otherwise returns false.
	 */
	const bool operator==( const attribute& other ) const;

	/**
	 * \brief Gets whether or not the two attributes are not the same.
	 *
	 * \param other The other attribute that is being tested against.
	 * \return Returns false if the two attribute have the same name, arity, and type, otherwise returns true.
	 */
	const bool operator!=( const attribute& other ) const;
};

} // end of attributes namespace
} // end of buffers namespace
} // end of occluded namespace