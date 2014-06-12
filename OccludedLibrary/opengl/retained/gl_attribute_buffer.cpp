#include "gl_attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

gl_attribute_buffer::gl_attribute_buffer( gl_attribute_buffer& other ):
	m_usage( other.m_usage )
{
	m_id = other.m_id;
	m_buffer = other.m_buffer;
	m_shaderMap = other.m_shaderMap;

	refCounts[m_id] += 1;
}

gl_attribute_buffer::gl_attribute_buffer( const buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, const buffer_usage_t usage ):
	m_buffer( buffers::attribute_buffer_factory::create_attribute_buffer( map ) ),
	m_usage( usage ),
	m_shaderMap( new shaders::shader_attribute_map( map, shaderProg ) )
{
	init_buffer();
}


gl_attribute_buffer::~gl_attribute_buffer() {
	// Reference counts should never be 0 when the destructor is called
	assert( refCounts[m_id] > 0 );

	refCounts[m_id] -= 1;

	// If this is the final copy of the gl_attribute_buffer, delete the vertex buffer object that was created for it
	if( refCounts[m_id] == 0 )
		glDeleteBuffers( 1, &m_id );
}

void gl_attribute_buffer::insert_values( const std::vector<char>& values ) {
	m_buffer->insert_values( values );

	bind_buffer();

	if( GL_NO_ERROR != glGetError() ) {
		throw std::runtime_error( "gl_attribute_buffer.bind_buffer: Failed to set buffer's data." );
	}
}

void gl_attribute_buffer::bind_buffer() const {
	glBindBuffer( GL_ARRAY_BUFFER, m_id );

	// Check to make sure the buffer has a size greater than 0, so that the glBufferData call does not cause OpenGL to enter an error state.
	if( m_buffer->get_byte_size() > 0 )
		glBufferData( GL_ARRAY_BUFFER, m_buffer->get_byte_size(), &m_buffer->get_all_data()[0], m_usage );
	
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

void gl_attribute_buffer::prepare_for_render() const {
	bind_buffer();

	m_shaderMap->set_attrib_pointers( *m_buffer );
}

// Private Method

void gl_attribute_buffer::init_buffer() {
	glGenBuffers( 1, &m_id );

	if( GL_NO_ERROR != glGetError() ) {
		throw std::runtime_error( "gl_attribute_buffer.init_buffer: Failed to generate buffer." );
	}

	// The entry should not exist in map or its count should be 0
	assert( refCounts.find( m_id ) == refCounts.end() || refCounts.find( m_id )->second == 0 );

	refCounts.insert( std::pair<GLuint, unsigned int>( m_id, 1 ) );

	bind_buffer();
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
