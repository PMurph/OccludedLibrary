#include "attribute_buffer_factory.h"

namespace occluded { namespace buffers {

std::auto_ptr<attribute_buffer> attribute_buffer_factory::create_attribute_buffer( const attributes::attribute_map& map ) {
	std::auto_ptr<attribute_buffer> newBuffer;
	
	if( map.is_interleaved() ) {
		newBuffer.reset( new interleaved_attr_buffer( map ) );
	} else {
		newBuffer.reset( new segregated_attr_buffer( map ) );
	}

	return newBuffer;
}

// private functions

attribute_buffer_factory::attribute_buffer_factory()
{
}


attribute_buffer_factory::~attribute_buffer_factory()
{
}

} // end of buffers namespace
} // end of occluded namespace
