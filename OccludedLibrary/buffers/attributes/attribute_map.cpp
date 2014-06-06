#include "attribute_map.h"

namespace occluded { namespace buffers { namespace attributes {

attribute_map::attribute_map( const bool interleaved ):
	m_attribCount( 0 ),
	m_defining( true ),
	m_interleaved( interleaved )
{
}


attribute_map::~attribute_map() {
}

void attribute_map::add_attribute( const attribute& newAttrib ) {
	if( !m_defining ) {
		throw std::runtime_error( "attribute_map.add_attribute: Failed to add attribut(" + newAttrib.get_name() +
			") because attribute map is not being defined." );
	}

	if( m_attribNames.find( newAttrib.get_name() ) != m_attribNames.end() ) {
		throw std::runtime_error( "attribute_map.add_attribute: Failed to add attribute(" + newAttrib.get_name() + 
			") because attribute map already has an attribute of the same name." );
	}

	m_attributes.push_back( newAttrib );
	m_attribNames.insert( newAttrib.get_name() );

	++m_attribCount;
}

void attribute_map::end_definition() {
	m_defining = false;
}

void attribute_map::reset( const bool interleaved ) {
	m_defining = true;
	m_attribCount = 0;
	m_attributes.clear();
	m_attribNames.clear();
	m_interleaved = interleaved;
}

const std::size_t attribute_map::get_byte_size() const {
	std::size_t byteSize = 0;

	for( std::vector<const attribute>::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it ) {
		byteSize += it->get_attrib_size();
	}

	return byteSize;
}

const unsigned int attribute_map::get_attrib_count() const {
	return m_attribCount;
}

const std::vector<const attribute>& attribute_map::get_attributes() const {
	if( m_defining )
		throw std::runtime_error( "attribute_map.get_attributes: Failed to get attributes because the attribute map is still being defined." );

	return m_attributes;
}

const bool attribute_map::is_interleaved() const {
	return m_interleaved;
}

const bool attribute_map::being_defined() const {
	return m_defining;
}

const bool attribute_map::operator==( const attribute_map& other ) const {
	unsigned int i = 0;
	bool equal = true;
	std::vector<const attribute> otherAttrib;

	// If either attribute_map is still being defined return false
	if( this->m_defining == false && other.being_defined() == false ){
		otherAttrib = other.get_attributes();
	
		// Check the counts of the attributes so that iterating through all the attributes can be avoided if possible
		equal = otherAttrib.size() == m_attribCount;

		for( i = 0; equal && i < otherAttrib.size(); ++i ) {
			// Check to see if the attribute are not equal
			equal = otherAttrib[i] == m_attributes[i]; 
		}
	
		equal = this->m_interleaved == other.m_interleaved && equal;
	} else {
		equal = false;
	}

	return equal;
}

const bool attribute_map::operator!=( const attribute_map& other ) const {
	return !( *this == other );
}

} // end of attributes namespace
} // end of buffers namespace
} // end of occluded namespace
