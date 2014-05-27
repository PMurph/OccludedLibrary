#pragma once

#include "interleaved_attr_buffer.h"
#include "segregated_attr_buffer.h"

namespace occluded { namespace buffers {

class attribute_buffer_factory
{
public:
	static std::auto_ptr<attribute_buffer> create_attribute_buffer( const attributes::attribute_map& map );

private:
	attribute_buffer_factory();
	~attribute_buffer_factory();
};

} // end of buffers namespace
} // end of occluded namespace