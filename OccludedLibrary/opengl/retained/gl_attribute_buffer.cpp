#include "gl_attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

gl_attribute_buffer::gl_attribute_buffer( const buffers::attributes::attribute_map& map, const buffer_usage_t usage ):
	//m_buffer( map ),
	m_usage( usage )
{
	init_buffer();
}


gl_attribute_buffer::~gl_attribute_buffer()
{
}

void gl_attribute_buffer::bind_buffer() {
	glBindBuffer( GL_ARRAY_BUFFER, m_id );
}

void gl_attribute_buffer::insert_values( const std::vector<char>& values ) {
	//m_buffer.insert_values( values );

	bind_buffer();
	//glBufferData( GL_ARRAY_BUFFER, m_buffer.get_byte_size(), &m_buffer.get_all_data()[0], m_usage );
}

const GLuint gl_attribute_buffer::get_id() const {
	return m_id;
}

// Private Method

void gl_attribute_buffer::init_buffer() {
	glGenBuffers( 1, &m_id );
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
