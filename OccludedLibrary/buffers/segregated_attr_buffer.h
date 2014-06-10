#pragma once

#include "attribute_buffer.h"

namespace occluded { namespace buffers {

/**
 * \class segregated_attr_buffer
 * \brief An attribute buffer subclass with segregated data.
 *
 * An attribute buffer that stores values in segregated sections. The first value of the first attribute will be stored, then the second
 * value of the first attribute is stored and so on.
 */
class segregated_attr_buffer:
	public attribute_buffer
{
public:
	segregated_attr_buffer( const attributes::attribute_map& map );
	~segregated_attr_buffer();

	/**
	 * \fn insert_values
	 * \brief Inserts values into the attribute buffer.
	 *
	 * \param values A reference to a vector of characters representing the values to be inserted.
	 *
	 * Inserts a value into the attribute buffer. The values in the vector parameter will be split up and each value will be placed in the
	 * correct section of the buffer.
	 */
	void insert_values( const std::vector<char>& values );
};

} // end of buffers namespace
} // end of occluded namespace
