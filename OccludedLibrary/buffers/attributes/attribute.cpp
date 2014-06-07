#include "attribute.h"

namespace occluded { namespace buffers { namespace attributes {

const std::size_t attribute::ATTRIB_SIZE = 4; // May change if more attribute types are added

attribute::attribute( const std::string& name, const unsigned int arity, const attribute_t type, const bool normalized ):
	m_name( name ),
	m_arity( arity ),
	m_type( type ),
	m_normalized( normalized )
{
}


attribute::~attribute()
{
}

const std::string& attribute::get_name() const {
	return m_name;
}

const unsigned int attribute::get_arity() const {
	return m_arity;
}

const attribute_t attribute::get_type() const {
	return m_type;
}

const std::size_t attribute::get_component_size() const {
	return ATTRIB_SIZE;
}

const std::size_t attribute::get_attrib_size() const {
	return ATTRIB_SIZE * m_arity;
}

const bool attribute::is_normalized() const {
	return m_normalized;
}

const bool attribute::operator==( const attribute& other ) const {
	return this->m_name == other.m_name && this->m_arity == other.m_arity && this->m_type == other.m_type;
}

const bool attribute::operator!=( const attribute& other ) const {
	return !( *this == other );
}

} // end of attributes namespace
} // end of buffers namespace
} // end of occluded namespace