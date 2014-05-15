#include "gl_attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

gl_attribute_buffer::gl_attribute_buffer( const buffers::attributes::attribute_map& map ):
	m_buffer( map )
{
	init_buffer();
}


gl_attribute_buffer::~gl_attribute_buffer()
{
}

void gl_attribute_buffer::bind_buffer() {
}

void gl_attribute_buffer::setup_data_store() {
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
