#include "interleaved_attr_buffer.h"

namespace occluded { namespace buffers {

interleaved_attr_buffer::interleaved_attr_buffer( const attributes::attribute_map& map ):
	attribute_buffer( map )
{
	if( !map.is_interleaved() ) {
		throw std::runtime_error( "interleaved_attr_buffer: Failed to initialize interleaved attribute buffer because attribute map passed to constructor"
			+ std::string( " was not interleaved." ) );
	}
}


interleaved_attr_buffer::~interleaved_attr_buffer()
{
}

void interleaved_attr_buffer::insert_values( const std::vector<char>& values ) {
	unsigned int currIndex = 0, currOffset = 0;
	std::vector<char> newData;
	std::size_t startIndex = m_data.size();
	
	if( m_map.get_byte_size() == 0 ) {
		throw std::runtime_error( "interleaved_attr_buffer.insert_values: Failed to insert values because the attribute map contained no attributes.");
	}

	if( values.size() == 0 ) {
		throw std::runtime_error( "interleaved_attr_buffer.insert_values: Failed to insert values because an empty vector was passed to function.");
	}

	if( values.size() % m_map.get_byte_size() != 0 ) {
		throw std::runtime_error( "attribute_buffer.insert_values: Failed to insert values because the size of values vector(" + 
			boost::lexical_cast<std::string>( values.size() ) + ") is not a valid size." ); 
	}

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
}

} // end of buffers namespace
} // end of occluded namespace
