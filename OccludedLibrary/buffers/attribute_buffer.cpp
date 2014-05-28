#include "attribute_buffer.h"

namespace occluded { namespace buffers {

attribute_buffer::attribute_buffer( const attributes::attribute_map& map ):
	m_map( map ),
	m_data( 0 ),
	m_numValues( 0 ),
	m_pointersSet( false )
{
	if( m_map.being_defined() )
		throw std::runtime_error( "attribute_buffer: Failed to create attribute buffer because attribute map is still being defined." );

	init_buffer();
}


attribute_buffer::~attribute_buffer()
{
	m_bufferPointers.clear();
	m_data.clear();
}

void attribute_buffer::clear_buffer() {
	m_numValues = 0;
	m_data.clear();
	m_pointersSet = false;

	if( m_map.get_attrib_count() > 0 )
		memset( &m_bufferPointers[0], 0, m_map.get_attrib_count() * sizeof( unsigned int ) );
}

const std::size_t attribute_buffer::get_byte_size() const {
	return m_data.size();
}

const unsigned int attribute_buffer::get_num_values() const {
	return m_numValues;
}

const std::vector<char>& attribute_buffer::get_all_data() const {
	return m_data;
}

const attributes::attribute_map& attribute_buffer::get_attribute_map() const {
	return m_map;
}

const std::vector<unsigned int>& attribute_buffer::get_attribute_data_offsets() const {
	return m_bufferPointers;
}

// Private Member Functions

void attribute_buffer::init_buffer() {
	m_bufferPointers.resize( m_map.get_attrib_count() );

	if( m_map.get_attrib_count() > 0 )
		memset( &m_bufferPointers[0], 0, m_bufferPointers.size() * sizeof( unsigned int ) );
}

} // end of buffers namespace
} // end of occluded namespace