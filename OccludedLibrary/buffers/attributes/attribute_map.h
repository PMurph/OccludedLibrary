#pragma once

#include <vector>

#include <boost/unordered_set.hpp>
#include <boost/lexical_cast.hpp>

#include "attribute.h"

namespace occluded { namespace buffers { namespace attributes {

class attribute_map
{
private:
	bool m_interleaved;
	bool m_defining;
	unsigned int m_attribCount;
	std::vector<const attribute> m_attributes;
	boost::unordered_set<const std::string> m_attribNames;

public:
	attribute_map( const bool interleaved );
	~attribute_map();

	void add_attribute( const attribute& newAttrib );
	void end_definition();
	void reset( const bool interleaved );

	const std::size_t get_byte_size() const;
	const unsigned int get_attrib_count() const;
	const std::vector<const attribute>& get_attributes() const;
	const bool is_interleaved() const;
	const bool being_defined() const;
	const bool operator==( const attribute_map& other ) const;
	const bool operator!=( const attribute_map& other ) const;
};

} // end of attributes namespace
} // end of buffers namespace
} // end of occluded namespace
