#include "gl_retained_mesh.h"

namespace occluded { namespace opengl { namespace retained {

gl_retained_mesh::gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, const buffer_usage_t usage ):
	m_shaderProg( shaderProg ),
	m_usage( usage )
{
}


gl_retained_mesh::~gl_retained_mesh()
{
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
