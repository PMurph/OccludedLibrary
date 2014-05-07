#pragma once

#include "attributes/attribute_map.h"

namespace occluded { namespace buffers {

class attribute_buffer
{
private:
	attributes::attribute_map m_map;

	std::vector<char> m_data;

public:
	attribute_buffer(const attributes::attribute_map& map);
	~attribute_buffer();

	void insert_values( const std::vector<char>& values );

	const std::vector<char>& get_all_data() const; //TODO

	const attributes::attribute_map& get_attribute_map() const; //TODO
};

} // end of buffers namespace
} // end of occluded namespace