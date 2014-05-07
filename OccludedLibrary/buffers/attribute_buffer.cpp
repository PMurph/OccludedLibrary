#include "attribute_buffer.h"

namespace occluded { namespace buffers {

attribute_buffer::attribute_buffer(const attributes::attribute_map& map):
	m_map( map ),
	m_data( 0 )
{
	if( m_map.being_defined() )
		throw std::runtime_error( "attribute_buffer: Failed to create attribute buffer because attribute map is still being defined." );
}


attribute_buffer::~attribute_buffer()
{
}

void attribute_buffer::insert_values( const std::vector<char>& values ) {
	//m_data = values;
	for( std::vector<char>::const_iterator it = values.begin(); it != values.end(); ++it ) {
		m_data.push_back( *it );
	}
}

const std::vector<char>& attribute_buffer::get_all_data() const {
	return m_data;
}

const attributes::attribute_map& attribute_buffer::get_attribute_map() const {
	return m_map;
}

} // end of buffers namespace
} // end of occluded namespace