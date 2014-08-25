#include "gl_attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

gl_attribute_buffer::gl_attribute_buffer( gl_attribute_buffer& other ):
	m_usage( other.m_usage )
{
	gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();

	m_vaoId = other.m_vaoId;
	m_id = other.m_id;
	m_buffer = other.m_buffer;
	m_shaderMap = other.m_shaderMap;

	manager.add_ref_to_vao( m_vaoId );
	manager.add_ref_to_vbo( m_vaoId, m_id );
}

gl_attribute_buffer::gl_attribute_buffer( const GLuint vaoId, const buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, 
	const buffer_usage_t usage = static_draw_usage ):
	m_vaoId( vaoId ),
	m_buffer( buffers::attribute_buffer_factory::create_attribute_buffer( map ) ),
	m_usage( usage ),
	m_shaderMap( new shaders::shader_attribute_map( map, shaderProg ) )
{
	init_buffer();
}


gl_attribute_buffer::~gl_attribute_buffer() {
	gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();

	manager.remove_ref_to_vbo( m_vaoId, m_id );
	manager.remove_ref_to_vao( m_vaoId );
}

void gl_attribute_buffer::insert_values( const std::vector<char>& values ) {
	m_buffer->insert_values( values );

	bind_buffer();

	if( GL_NO_ERROR != glGetError() ) {
		throw std::runtime_error( "gl_attribute_buffer.bind_buffer: Failed to set buffer's data." );
	}
}

void gl_attribute_buffer::bind_buffer() const {
	glBindVertexArray( m_vaoId );
	glBindBuffer( GL_ARRAY_BUFFER, m_id );

	// Check to make sure the buffer has a size greater than 0, so that the glBufferData call does not cause OpenGL to enter an error state.
	if( m_buffer->get_byte_size() > 0 )
		glBufferData( GL_ARRAY_BUFFER, static_cast<GLsizeiptr>( m_buffer->get_byte_size() ), &m_buffer->get_all_data()[0], m_usage );
	
	if( GL_NO_ERROR != glGetError() ) {
		throw std::runtime_error( "gl_attribute_buffer.bind_buffer: Failed to bind buffer." );
	}
}

const GLuint gl_attribute_buffer::get_id() const {
	return m_id;
}

const buffers::attributes::attribute_map& gl_attribute_buffer::get_buffer_map() const {
	return m_buffer->get_attribute_map();
}

const buffer_usage_t gl_attribute_buffer::get_usage() const {
	return m_usage;
}

const unsigned int gl_attribute_buffer::get_num_values() const {
	return m_buffer->get_num_values();
}

void gl_attribute_buffer::prepare_for_render() const {
	bind_buffer();

	m_shaderMap->set_attrib_pointers( *m_buffer );
}

// Private Method

void gl_attribute_buffer::init_buffer() {
	gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();
	
	manager.add_ref_to_vao( m_vaoId );
	m_id = manager.get_new_vbo( m_vaoId );

	bind_buffer();
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
