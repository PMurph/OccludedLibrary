#include "segregated_attr_buffer.h"

namespace occluded { namespace buffers {

segregated_attr_buffer::segregated_attr_buffer( const attributes::attribute_map& map ):
	attribute_buffer( map )
{
	if( map.is_interleaved() ) {
		throw std::runtime_error( "segregated_attr_buffer: Failed to initialize interleaved attribute buffer because attribute map passed to constructor"
			+ std::string( " was interleaved." ) );
	}
}


segregated_attr_buffer::~segregated_attr_buffer()
{
}

void segregated_attr_buffer::insert_values( const std::vector<char>& values ) {
	unsigned int numVals;
	std::vector<char> newData;
	unsigned int newOffset = 0, dataOffset = 0, valuesOffset = 0, currBuffOffsetIndex = 0;

	if( m_map.get_byte_size() == 0 ) {
		throw std::runtime_error( "attribute_buffer.insert_values: Failed to insert values because the attribute map contained no attributes.");
	}

	if( values.size() == 0 ) {
		throw std::runtime_error( "interleaved_attr_buffer.insert_values: Failed to insert values because an empty vector was passed to function.");
	}

	if( values.size() % m_map.get_byte_size() != 0 ) {
		throw std::runtime_error( "attribute_buffer.insert_values: Failed to insert values because the size of values vector(" + 
			boost::lexical_cast<std::string>( values.size() ) + ") is not a valid size." ); 
	}

	numVals = static_cast<unsigned int>( values.size() / m_map.get_byte_size() );

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
}

} // end of buffers namespace
} // end of occluded namespace
