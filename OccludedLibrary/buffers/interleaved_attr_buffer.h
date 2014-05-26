#pragma once

#include "attribute_buffer.h"

namespace occluded { namespace buffers {

class interleaved_attr_buffer:
	public attribute_buffer
{
public:
	interleaved_attr_buffer( const attributes::attribute_map& map );
	~interleaved_attr_buffer();

	void insert_values( const std::vector<char>& values );
};

} // end of buffers namespace
} // end of occluded namespace