#pragma once

#include <string>

namespace occluded { namespace opengl { namespace retained { namespace buffers { namespace attributes {

typedef enum ATTRIB_TYPE {
	attrib_float,
	attrib_uint,
	attrib_int,
	attrib_invalid
} attribute_t;

class buffer_attribute
{
private:
	static const std::size_t ATTRIB_SIZE;

	std::string m_name;
	unsigned int m_arity;
	attribute_t m_type;

public:
	buffer_attribute( const std::string& name, const unsigned int arity, const attribute_t type );
	~buffer_attribute();

	const std::string& get_name() const;

	const unsigned int get_arity() const;

	const attribute_t get_type() const;

	const std::size_t get_component_size() const;

	const std::size_t get_attrib_size() const;
};

} // end of attributes namespace
} // end of buffers namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace