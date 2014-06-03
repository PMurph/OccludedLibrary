#include "gl_attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

gl_attribute_buffer::gl_attribute_buffer( const buffers::attributes::attribute_map& map, const buffer_usage_t usage ):
	m_buffer( buffers::attribute_buffer_factory::create_attribute_buffer( map ) ),
	m_usage( usage )
{
	init_buffer();
}


gl_attribute_buffer::~gl_attribute_buffer() {
	glDeleteBuffers( 1, &m_id );
}

void gl_attribute_buffer::bind_buffer() {
	glBindBuffer( GL_ARRAY_BUFFER, m_id );

	if( m_buffer->get_byte_size() > 0 )
		glBufferData( GL_ARRAY_BUFFER, m_buffer->get_byte_size(), &m_buffer->get_all_data()[0], m_usage );
	
	if( glGetError() != GL_NO_ERROR ) {
		throw std::runtime_error( "gl_attribute_buffer.bind_buffer: Failed to bind buffer." );
	}
}

void gl_attribute_buffer::insert_values( const std::vector<char>& values ) {
	m_buffer->insert_values( values );

	bind_buffer();

	if( glGetError() != GL_NO_ERROR ) {
		throw std::runtime_error( "gl_attribute_buffer.bind_buffer: Failed to set buffer's data." );
	}
}

const GLuint gl_attribute_buffer::get_id() const {
	return m_id;
}

const buffers::attributes::attribute_map& gl_attribute_buffer::get_buffer_map() const {
	return m_buffer->get_attribute_map();
}

// Private Method

void gl_attribute_buffer::init_buffer() {
	glGenBuffers( 1, &m_id );

	if( glGetError() != GL_NO_ERROR ) {
		throw std::runtime_error( "gl_attribute_buffer.init_buffer: Failed to generate buffer." );
	}

	bind_buffer();
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
