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
	std::vector<unsigned short> m_indices;
	GLuint m_elemBufferId;

public:
	box( const occluded::shader_program& shaderProg, const occluded::buffers::attributes::attribute_map& map );
	~box();

	void set_pos( const float posX, const float posY, const float posZ );

private:
	void init_box();

	void setup_element_buffer();

	static void populate_box_buffer( occluded::opengl::retained::gl_attribute_buffer& vertexBuffer );
	static void place_vertex_in_buffer( occluded::opengl::retained::gl_attribute_buffer& vertexBuffer, float vertX, float vertY, float vertZ, float colR, float colG, float colB );
	static void populate_box_indices( std::vector<unsigned short>& indexBuffer );
};

