#include "box.h"

// Public Member Functions

box::box( const occluded::shader_program& shaderProg, const occluded::buffers::attributes::attribute_map& map ):
	m_shaderProg( shaderProg ),
	m_buffer( map, occluded::opengl::retained::static_draw_usage ),
	m_posX( 0.0f ),
	m_posY( 0.0f ),
	m_posZ( 0.0f )
{
	init_box();
}


box::~box()
{
}

void box::set_pos( const float posX, const float posY, const float posZ ) {
	m_posX = posX;
	m_posY = posY;
	m_posZ = posZ;
}

// Private Memmber Functions

void box::init_box() {
}