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

/*
void attribute_buffer::insert_values( const std::vector<char>& values ) {
	if( m_map.get_byte_size() == 0 ) {
		throw std::runtime_error( "attribute_buffer.insert_values: Failed to insert values because the attribute map contained no attributes.");
	}

	numVals = static_cast<unsigned int>( values.size() / m_map.get_byte_size() );

	if( values.size() % m_map.get_byte_size() != 0 ) {
		throw std::runtime_error( "attribute_buffer.insert_values: Failed to insert values because the size of values vector(" + 
			boost::lexical_cast<std::string>( values.size() ) + ") is not equals to the number bytes needed(" + 
			boost::lexical_cast<std::string>( numVals * m_map.get_byte_size() ) + ")." );
	}

	if( m_map.is_interleaved() ) {
		insert_interleaved_values( values );
	} else {
		insert_segregated_values( values, numVals );
	}
}
*/

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

/*
void attribute_buffer::insert_segregated_values( const std::vector<char>& values, const unsigned int numVals ) {
	std::vector<char> newData;
	unsigned int newOffset = 0, dataOffset = 0, valuesOffset = 0, currBuffOffsetIndex = 0;

	newData.resize( m_data.size() + values.size() );

	for( std::vector<const attributes::attribute>::const_iterator it = m_map.get_attributes().begin(); it != m_map.get_attributes().end(); ++it ) {
		m_bufferPointers[currBuffOffsetIndex] = newOffset;

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
		++currBuffOffsetIndex;
	}

	m_numValues += numVals;
	m_data = newData;
}*/

/*
void attribute_buffer::insert_interleaved_values( const std::vector<char>& values ) {
	unsigned int currIndex = 0, currOffset = 0;
	std::vector<char> newData;
	std::size_t startIndex = m_data.size();

	m_data.resize( m_data.size() + values.size() );

	memcpy( &m_data[startIndex], &values[0], values.size() * sizeof( char ) );

	if( !m_pointersSet ) {
		for( std::vector<attributes::attribute>::const_iterator it = m_map.get_attributes().begin(); it != m_map.get_attributes().end(); ++it ) {
			m_bufferPointers[currIndex] = currOffset;
			currOffset += static_cast<unsigned int>( it->get_attrib_size() );
			++currIndex;
		}

		m_pointersSet = true;
	}
}*/

} // end of buffers namespace
} // end of occluded namespace