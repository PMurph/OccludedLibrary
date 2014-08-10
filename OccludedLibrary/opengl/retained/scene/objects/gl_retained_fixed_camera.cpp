#include "gl_retained_fixed_camera.h"

namespace occluded { namespace opengl { namespace retained { namespace scene  { namespace objects {

gl_retained_fixed_camera::gl_retained_fixed_camera( const shader_program& shaderProg, const glm::mat4& proj, const glm::mat4& view ): 
	gl_retained_camera( shaderProg, proj, view )
{
}


gl_retained_fixed_camera::~gl_retained_fixed_camera() {
}

void gl_retained_fixed_camera::set_for_render() const {
	if( !m_shaderProg.get().is_linked() )
		throw std::runtime_error( "gl_retained_fixed_camera.set_for_render: Failed to setup the camera for rendering because attached shader_program is not linked." );

	gl_retained_camera::set_uniform_vals();
}

const glm::mat4& gl_retained_fixed_camera::get_projection() const {
	return m_projection;
}

const glm::mat4& gl_retained_fixed_camera::get_view() const {
	return m_view;
}

} // end of objects namespace
} // end of scene namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace