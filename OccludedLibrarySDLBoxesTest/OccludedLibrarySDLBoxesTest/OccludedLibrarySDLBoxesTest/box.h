#pragma once

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl/retained/shaders/shader_program.h"
#include "opengl/retained/gl_attribute_buffer.h"

class box
{
private:
	static const float TWO_PI;

	const occluded::shader_program& m_shaderProg;

	occluded::opengl::retained::gl_attribute_buffer m_buffer;
	std::vector<unsigned int> m_indices;
	GLuint& m_vao;
	GLuint m_elemBufferId;
	GLuint m_posAttribPtr, m_colorAttribPtr;
	GLuint m_modelUniformPtr;

	float m_posX, m_posY, m_posZ;
	float m_angle;

public:
	box( const occluded::shader_program& shaderProg, const occluded::buffers::attributes::attribute_map& map, GLuint& vao );
	~box();

	void set_pos( const float posX, const float posY, const float posZ );

	void draw();

	void rotate( const float angle );

private:
	void init_box();

	void setup_element_buffer();
	
	void setup_attrib_pointers();

	void setup_uniforms();

	void set_model_value();

	void set_attrib_pointers();

	static void populate_box_buffer( occluded::opengl::retained::gl_attribute_buffer& vertexBuffer );
	static void place_vertex_in_buffer( occluded::opengl::retained::gl_attribute_buffer& vertexBuffer, float vertX, float vertY, float vertZ, float colR, float colG, float colB );
	static void populate_box_indices( std::vector<unsigned int>& indexBuffer );
};

