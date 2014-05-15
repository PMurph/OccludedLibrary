#pragma once

#include "attributes/attribute_map.h"

namespace occluded { namespace buffers {

class attribute_buffer
{
private:
	attributes::attribute_map m_map;

	std::vector<char> m_data;
	std::vector<unsigned int> m_bufferPointers;
	unsigned int m_numValues;
	bool m_pointersSet;
public:
	attribute_buffer(const attributes::attribute_map& map);
	~attribute_buffer();

	void insert_values( const std::vector<char>& values, const unsigned int numVals );
	void clear_buffer();

	const unsigned int get_num_values() const;
	const std::vector<char>& get_all_data() const;
	const attributes::attribute_map& get_attribute_map() const;
	const std::vector<unsigned int>& get_attribute_data_offsets() const;

private:
	void init_buffer();
	void init_segregated_buffer();
	void insert_segregated_values( const std::vector<char>& values, const unsigned int numVals );
	void insert_interleaved_values( const std::vector<char>& values, const unsigned int numVals );
};

} // end of buffers namespace
} // end of occluded namespace