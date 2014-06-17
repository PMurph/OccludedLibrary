#pragma once

#ifndef UNIT_TESTING
#include "GL/glew.h"
#else
#include "opengl_mock.h"
#endif

#include "gl_attribute_buffer.h"
#include "../../meshes/mesh.h"


namespace occluded { namespace opengl { namespace retained {

/**
 * \enum primitive_type_t
 * \brief Stores the type of primitive that will be used to render the mesh.
 *
 * Stores the type of primitive that will be used by OpenGL to render the mesh. This is to prevent the user from passing the wrong primitive
 * to the mesh.
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
 * \brief An mesh implementation for OpenGL retained mode.
 *
 * 
 */
class gl_retained_mesh:
	public occluded::meshes::mesh
{
private:
	const shaders::shader_program& m_shaderProg;
	
	gl_attribute_buffer m_buffer;
	primitive_type_t m_primitiveType;
	std::vector<unsigned int> m_indices;

public:
	/**
	 * \brief Initializes an empty mesh.
	 *
	 * \param map A reference to the attribute map that will be used to store vertex data.
	 * \param shaderProg A refernce to a shader program that will be used to render the mesh.
	 * \param usage A buffer usage type that specifies how the vertex and index data will be used.
	 * \param primitiveType A primitive type that specifies which OpenGL primitive will be used to construct the faces of the mesh.
	 *
	 * Initializes the mesh by constructing an gl_attribute_buffer from the map, shader program and usage parameters. The default primitive used for
	 * constructing the mesh is primitive_triangles. This constructor is to be used if the mesh is to be built up from scratch. An exception will be
	 * thrown in the attribute map is still being defined or if the shader program has not been linked.
	 */
	gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, const buffer_usage_t usage, 
		const primitive_type_t primitiveType );

	/**
	 * \brief Initializes an mesh.
	 *
	 * \param shaderProg A reference to a shader program that will be used to render the mesh.
	 * \param buffer A reference to a gl_attribute_buffer which stores the vertice data.
	 * \param faces A reference to a vector of unsigned integers representing the vertices that make up the faces of the mesh.
	 * \param primitiveType A primitive type that specifies which OpenGL primitive will be used to construct the faces of the mesh.
	 *
	 * Initializes the mesh by making a copy of an gl_attribute_buffer and a vector of indices. he default primitive used for constructing the mesh is
	 * primitive_triangles. This constructor is to be used if the mesh is to be pre-built or to be built up from another mesh. An exception will be
	 * thrown in the attribute map is still being defined or if the shader program has not been linked.
	 */
	gl_retained_mesh( const shaders::shader_program& shaderProg, gl_attribute_buffer& buffer, const std::vector<unsigned int>& faces,
		const primitive_type_t primitiveType );
	~gl_retained_mesh();


private:
	void init_mesh();
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
