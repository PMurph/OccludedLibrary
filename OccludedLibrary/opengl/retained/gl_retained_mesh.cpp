#include "gl_retained_mesh.h"

namespace occluded { namespace opengl { namespace retained {

gl_retained_mesh::gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, 
	const buffer_usage_t usage, const primitive_type_t primitiveType = primitive_triangles ):
	m_shaderProg( shaderProg ),
	m_buffer( gl_attribute_buffer( map, shaderProg, usage ) )
{
}

gl_retained_mesh::gl_retained_mesh( const shaders::shader_program& shaderProg, gl_attribute_buffer& buffer, const std::vector<unsigned int>& faces,
	const primitive_type_t primitiveType = primitive_triangles ):
	m_shaderProg( shaderProg ),
	m_buffer( buffer )
{
}


gl_retained_mesh::~gl_retained_mesh()
{
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
