#pragma once

#include "attributes/attribute_map.h"

namespace occluded { namespace buffers {

/**
 * \class attribute_buffer
 * \brief Provides contiguous storage for values associated with attributes.
 *
 * Provides contiguous storage for values associated with attributes in an attribute map.
 */
class attribute_buffer {
protected:
	attributes::attribute_map m_map;
	std::vector<char> m_data;
	std::vector<unsigned int> m_bufferPointers;
	unsigned int m_numValues;
	bool m_pointersSet;

public:
	/**
	 * \brief Initializes the attribute buffer.
	 *
	 * \param map A reference to an attribute map.
	 *
	 * Intializes the attribute buffer using the provided attribute map.
	 */
	attribute_buffer( const attributes::attribute_map& map );
	~attribute_buffer();

	/**
	 * \fn insert_values
	 * \brief Inserts values into the attribute buffer.
	 *
	 * \param values A reference to a vector of characters representing the values to be inserted.
	 *
	 * Insertes a values into the attribute buffer. The values are inserted such a way that the organization of the values in the buffer
	 * is preserved. The values in the vector parameter are assumed to be organized in the same way as the attributes in the attribute map.
	 * If inserting a single value, a single value must be inserted for every attribute in the attribute map.
	 */
	virtual void insert_values( const std::vector<char>& values ) = 0;

	/**
	 * \fn clear_buffer
	 * \brief Removes all the values from the buffer.
	 */
	void clear_buffer();

	/**
	 * \fn get_byte_size
	 * \brief Gets the size in bytes of the buffer.
	 *
	 * \return A std::size_t representing the size of the attribute buffer in bytes.
	 */
	const std::size_t get_byte_size() const;

	/**
	 * \fn get_num_values
	 * \brief Get the number of values in the buffer.
	 *
	 * \return An unsigned int representing the number of values in the buffer.
	 */
	const unsigned int get_num_values() const;

	/**
	 * \fn get_all_data
	 * \brief Gets the data in the attribute buffer.
	 *
	 * \return A reference to a vector of character representing the values stored in the attribute buffer.
	 */
	const std::vector<char>& get_all_data() const;

	/**
	 * \fn get_attribute_map
	 * \brief Gets the attribute map of the attribute buffer.
	 *
	 * \return A reference to the attribute map of the attribute buffer.
	 */
	const attributes::attribute_map& get_attribute_map() const;

	/**
	 * \fn get_attribute_data_offsets
	 * \brief Gets the offsets of to the values in attribute buffer.
	 *
	 * \return A reference to a vector of unsigned integers representing the location of the first value of each attribute in the
	 * attribute buffer.
	 */
	const std::vector<unsigned int>& get_attribute_data_offsets() const;

private:
	/**
	 * \fn init_buffer
	 * \brief Initializes the attribute buffer.
	 */
	void init_buffer();
};

} // end of buffers namespace
} // end of occluded namespace