#pragma once

#ifndef UNIT_TESTING
#include "GL/glew.h"
#else
#include "opengl_mock.h"
#endif

#include "gl_attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

/**
 * \enum primitive_type_t
 * \brief Stores the type of primitive that will be used to render the mesh.
 *
 * Stores the type of primitive that will be used by OpenGL to render the mesh.
 */
typedef enum PRIMITIVE_TYPE {
	primitive_point = GL_POINTS,
	primitive_lines = GL_LINES,
	primitive_line_strip = GL_LINE_STRIP,
	primitive_line_loop = GL_LINE_LOOP,
	primitive_triangles = GL_TRIANGLES,
	primitive_triangle_strip = GL_TRIANGLE_STRIP,
	primitive_triangle_fan = GL_TRIANGLE_FAN,
	primitive_patches = GL_PATCHES
} primitive_type_t;

/**
 * \class gl_retained_mesh
 * \brief An OpenGL
 */
class gl_retained_mesh
{
private:
	const shaders::shader_program& m_shaderProg;

	GLuint m_vaoId;
	buffer_usage_t m_usage;

	std::auto_ptr<gl_attribute_buffer> m_buffer;

public:
	gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, const buffer_usage_t usage );
	~gl_retained_mesh();

	/**
	 * \fn draw_mesh
	 * \brief Draws the mesh to the frame buffer.
	 */
	void draw_mesh() const;

private:
	void init_mesh();
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
