#pragma once

#include <GL/glew.h>

#include "opengl/retained/shaders/shader_program.h"
#include "opengl/retained/gl_attribute_buffer.h"

class box
{
private:
	const occluded::shader_program& m_shaderProg;

	occluded::opengl::retained::gl_attribute_buffer m_buffer;
	float m_posX, m_posY, m_posZ;

public:
	box( const occluded::shader_program& shaderProg, const occluded::buffers::attributes::attribute_map& map );
	~box();

	void set_pos( const float posX, const float posY, const float posZ );

private:
	void init_box();
};

