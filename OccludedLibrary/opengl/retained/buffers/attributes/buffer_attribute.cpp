#include "buffer_attribute.h"

namespace occluded { namespace opengl { namespace retained { namespace buffers { namespace attributes {

const std::size_t buffer_attribute::ATTRIB_SIZE = 4; // May change if more attribute types are added

buffer_attribute::buffer_attribute( const std::string& name, const unsigned int arity, const attribute_t type ):
	m_name( name ),
	m_arity( arity ),
	m_type( type )
{
}


buffer_attribute::~buffer_attribute()
{
}

const std::string& buffer_attribute::get_name() const {
	return m_name;
}

const unsigned int buffer_attribute::get_arity() const {
	return m_arity;
}

const attribute_t buffer_attribute::get_type() const {
	return m_type;
}

const std::size_t buffer_attribute::get_component_size() const {
	return ATTRIB_SIZE;
}

const std::size_t buffer_attribute::get_attrib_size() const {
	return ATTRIB_SIZE * m_arity;
}

} // end of attributes namespace
} // end of buffers namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace