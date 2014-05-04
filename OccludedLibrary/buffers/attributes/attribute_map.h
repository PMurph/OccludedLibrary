#pragma once

#include <vector>

#include <boost/unordered_set.hpp>
#include <boost/lexical_cast.hpp>

#include "buffer_attribute.h"

namespace occluded { namespace buffers { namespace attributes {

class attribute_map
{
private:
	bool m_interleaved;
	bool m_defining;
	unsigned int m_attribCount;
	std::vector<const buffer_attribute> m_attributes;
	boost::unordered_set<const std::string> m_attribNames;

public:
	attribute_map( const bool interleaved );
	~attribute_map();

	void add_attribute( const buffer_attribute& newAttrib );

	void end_definition();

	void reset( const bool interleaved );

	const unsigned int get_attrib_count() const;

	const std::vector<const buffer_attribute>& get_attributes() const;

	const bool is_interleaved() const;
};

} // end of attributes namespace
} // end of buffers namespace
} // end of occluded namespace
