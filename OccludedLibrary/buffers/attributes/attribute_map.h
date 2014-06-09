#pragma once

#include <vector>

#include <boost/unordered_set.hpp>
#include <boost/lexical_cast.hpp>

#include "attribute.h"

namespace occluded { namespace buffers { namespace attributes {

/**
 * \class attribute_map
 * \brief Defines an organization of attributes.
 *
 * The attribute map defines an organization of attributes.This is to be used with an attribute buffer to determine in which order values are 
 * placed and if the values are interleaved or segregated in the buffer.
 * \see { occluded::buffers::attribute_buffer }
 */
class attribute_map
{
private:
	bool m_interleaved;
	bool m_defining;
	unsigned int m_attribCount;
	std::vector<const attribute> m_attributes;
	boost::unordered_set<const std::string> m_attribNames;

public:
	/**
	 * \brief Initializes the attribute map.
	 * 
	 * \param interleaved A boolean which determines whether or not the values should be segregated or interleaved.
	 *
	 * Initializes the attribute map using a boolean which provides a hint on whether or not values associated with attributes
	 * should be segregated or interleaved. This will start the definition of the map, and therefore must be paired with an 
	 * end_definition call.
	 */
	attribute_map( const bool interleaved );
	~attribute_map();

	/**
	 * \fn add_attribute
	 * \brief Adds an attribute to the map.
	 *
	 * \param newAttrib A reference to the attribute that is to be added to the attribute map.
	 *
	 * Adds a new attribute to the attribute map. If the attribute or an attribute with the same name already exists in the attribute 
	 * map, an exception will be thrown. If the definition of the attribute map has been ended then an exception will be thrown.
	 */
	void add_attribute( const attribute& newAttrib );

	/**
	 * \fn end_definition
	 * \brief Ends the definition of the attribute map.
	 *
	 * Ends the definition of the attribute map. This indicates that the attribute map is ready to be used by other classes.
	 */
	void end_definition();

	/**
	 * \fn reset
	 * \brief Clears attributes and restarts definition of attribute map.
	 *
	 * \param interleaved A boolean which determines whether or not the values should be segregated or interleaved.
	 *
	 * Removes all attributes from the attribute map and restarts the definition of the attribute map. This is to be used in the 
	 * the current definition of the attribute map needs to be changed.
	 * \warning If an attribute map is being used by another class that doesn't make a deep copy of it, this function should not
	 * be called because it may cause undefined behaviour.
	 */
	void reset( const bool interleaved );

	/**
	 * \fn get_byte_size
	 * \brief Gets the byte size of the attribute map.
	 *
	 * \return A std::size_t representing the number of bytes.
	 *
	 * Gets the byte of size that a single value of all the attributes in the attribute map would take up.
	 */
	const std::size_t get_byte_size() const;

	/**
	 * \fn get_attrib_count
	 * \brief Gets the number of attributes in the attribute map.
	 *
	 * \return An unsigned int representing the number of attributes.
	 */
	const unsigned int get_attrib_count() const;

	/**
	 * \fn get_attributes
	 * \brief Gets all the attribute in the attribute map.
	 *
	 * \return A reference to a vector containing all the attributes in the attribute map.
	 */
	const std::vector<const attribute>& get_attributes() const;

	/**
	 * \fn is_interleaved
	 * \brief Gets whether or not the values of the attributes should be interleaved or segregated.
	 *
	 * \return A boolean that is true if the values should be interleaved and false if they should be segregated.
	 */
	const bool is_interleaved() const;

	/**
	 * \fn being_defined
	 * \brief Gets whether or not the attribute map is still being defined.
	 *
	 * \return A boolean that is true if the attribute map is still being defined and false if it has finished being defined.
	 */
	const bool being_defined() const;

	/**
	 * \brief Gets whether or not the attribute maps are the same.
	 * 
	 * \return A boolean that is true if the attributes are the same in both attribute maps and are in the same order, and returns false 
	 * otherwise.
	 */
	const bool operator==( const attribute_map& other ) const;

	/**
	 * \brief Gets whether or not the attribute maps are different.
	 *
	 * \return A boolean that is false if the attributes are the same in both attribute maps and are in the same order, and returns true
	 * otherwise.
	 */
	const bool operator!=( const attribute_map& other ) const;
};

} // end of attributes namespace
} // end of buffers namespace
} // end of occluded namespace
