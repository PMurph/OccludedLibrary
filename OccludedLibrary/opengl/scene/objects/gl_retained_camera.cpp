#include "gl_retained_camera.h"

namespace occluded { namespace opengl { namespace scene { namespace objects {

gl_retained_camera::gl_retained_camera( const shader_program& shaderProg, const glm::mat4& projection, const glm::mat4& view ):
	m_shaderProg( shaderProg ),
	m_projection( projection ),
	m_view( view )
{
	if( m_shaderProg.get().is_linked() )
		check_store();
}

gl_retained_camera::~gl_retained_camera() {
}

void gl_retained_camera::set_shader_program( const shader_program& newShaderProg ) {
	m_shaderProg = boost::ref( newShaderProg );

	if( m_shaderProg.get().is_linked() )
		check_store();
}

const shader_program& gl_retained_camera::get_shader_program() const {
	return m_shaderProg;
}

// Protected Member Functions

void gl_retained_camera::set_uniform_vals() const {
	// The shader program should be linked and the projection and view uniforms should exist in the uniform stores
	assert( m_shaderProg.get().is_linked() );

	occluded::opengl::retained::shaders::shader_uniform_store& shaderStore = m_shaderProg.get().get_uniform_store();

	assert( shaderStore.has_uniform( SHADER_PROJECTION_UNIFORM_NAME ) &&
		m_shaderProg.get().get_uniform_store().has_uniform( SHADER_VIEW_UNIFORM_NAME ) );

	shaderStore.set_uniform_value( SHADER_PROJECTION_UNIFORM_NAME, m_projection );
	shaderStore.set_uniform_value( SHADER_VIEW_UNIFORM_NAME, m_view );
}

// Private Member Function

void gl_retained_camera::check_store() const {
	occluded::opengl::retained::shaders::shader_uniform_store& shaderStore = m_shaderProg.get().get_uniform_store();

	if( !shaderStore.has_uniform( SHADER_PROJECTION_UNIFORM_NAME ) )
		shaderStore.add_uniform( SHADER_PROJECTION_UNIFORM_NAME, m_projection );

	if( !shaderStore.has_uniform( SHADER_VIEW_UNIFORM_NAME ) )
		shaderStore.add_uniform( SHADER_VIEW_UNIFORM_NAME, m_view );
}

// Static Variables

const std::string gl_retained_camera::SHADER_PROJECTION_UNIFORM_NAME = "projection";
const std::string gl_retained_camera::SHADER_VIEW_UNIFORM_NAME = "view";

} // end of objects namespace
} // end of scene namespace
} // end of opengl namespace
} // end of occluded namespace