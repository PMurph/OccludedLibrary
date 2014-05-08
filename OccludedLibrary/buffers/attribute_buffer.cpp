#include "attribute_buffer.h"

namespace occluded { namespace buffers {

attribute_buffer::attribute_buffer(const attributes::attribute_map& map):
	m_map( map ),
	m_data( 0 ),
	m_bufferPointers( 0 ),
	m_numValues( 0 )
{
	if( m_map.being_defined() )
		throw std::runtime_error( "attribute_buffer: Failed to create attribute buffer because attribute map is still being defined." );

	init_buffer();
}


attribute_buffer::~attribute_buffer()
{
}

void attribute_buffer::insert_values( const std::vector<char>& values, const unsigned int numVals ) {
	insert_segregated_values( values, numVals );
}

void attribute_buffer::clear_buffer() {
	// TODO
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

// Private Member Functions

void attribute_buffer::init_buffer() {
	init_segregated_buffer();
}

void attribute_buffer::init_segregated_buffer() {
	m_bufferPointers.resize( m_map.get_attrib_count() );
	memset( &m_bufferPointers[0], 0, m_bufferPointers.size() * sizeof( unsigned int ) );
}

void attribute_buffer::insert_segregated_values( const std::vector<char>& values, const unsigned int numVals ) {
	std::vector<char> newData;
	unsigned int newOffset = 0, dataOffset = 0, valuesOffset = 0;

	newData.resize( m_data.size() + values.size() );

	for( std::vector<const attributes::attribute>::const_iterator it = m_map.get_attributes().begin(); it != m_map.get_attributes().end(); ++it ) {
		//copy the data currently in the buffer
		if( m_numValues > 0 ) {
			memcpy( &newData[newOffset], &m_data[dataOffset], m_numValues * it->get_attrib_size() );
			newOffset += m_numValues * static_cast<unsigned int>( it->get_attrib_size() );
			dataOffset += m_numValues * static_cast<unsigned int>( it->get_attrib_size() );
		}
		
		//copy the new data into the buffer
		memcpy( &newData[newOffset], &values[valuesOffset], numVals * it->get_attrib_size() );
		newOffset += numVals * static_cast<unsigned int>( it->get_attrib_size() );
		valuesOffset += numVals * static_cast<unsigned int>( it->get_attrib_size() );

	}

	m_numValues += numVals;
	m_data = newData;
}

} // end of buffers namespace
} // end of occluded namespace