#include "gl_retained_mesh.h"

namespace occluded { namespace opengl { namespace retained {

gl_retained_mesh::gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, 
	const buffer_usage_t usage, const primitive_type_t primitiveType ):
	m_shaderProg( shaderProg ),
	m_buffer( gl_attribute_buffer( map, shaderProg, usage ) )
{
}

gl_retained_mesh::gl_retained_mesh( const shaders::shader_program& shaderProg, gl_attribute_buffer& buffer, const std::vector<unsigned int>& faces,
	const primitive_type_t primitiveType ):
	m_shaderProg( shaderProg ),
	m_buffer( buffer )
{
}


gl_retained_mesh::~gl_retained_mesh()
{
}

void gl_retained_mesh::draw() const {
}

const std::vector<unsigned int> gl_retained_mesh::add_vertices( const std::vector<char>& vertices ) {
	unsigned int initNumVals = m_buffer.get_num_values();

	m_buffer.insert_values( vertices );

	return std::vector<unsigned int>( m_buffer.get_num_values() - initNumVals );
} 

const std::vector<unsigned int> gl_retained_mesh::add_faces( const std::vector<unsigned int>& faceIndices ) {
	return std::vector<unsigned int>( 0 );
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
