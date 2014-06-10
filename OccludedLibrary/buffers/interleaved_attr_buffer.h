#pragma once

#include "attribute_buffer.h"

namespace occluded { namespace buffers {

/**
 * \class interleaved_attr_buffer
 * \brief An attribute buffer subclass with interleaved data.
 *
 * An attribute buffer that stores values such that they are interleaved. The first value of the first attribute will be stored then
 * the first value of the second attribute will stored and so on.
 */
class interleaved_attr_buffer:
	public attribute_buffer
{
public:
	interleaved_attr_buffer( const attributes::attribute_map& map );
	~interleaved_attr_buffer();

	/**
	 * \fn insert_values
	 * \brief Inserts values into the attribute buffer.
	 *
	 * \param values A reference to a vector of characters representing the values to be inserted.
	 *
	 * Inserts a value into the attribute buffer. Since the attribute values are interleaved and the vector passed as a parameter
	 * has interleaved values, the values can be placed as the end of the buffer.
	 */
	void insert_values( const std::vector<char>& values );
};

} // end of buffers namespace
} // end of occluded namespace