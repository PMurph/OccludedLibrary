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
 * \brief A mesh implementation for OpenGL retained mode.
 *
 * A mesh implementation for OpenGL retained mode. This contains a gl_attribute_buffer and a vector of unsigned int representing the faces of the
 * of the mesh. This is class contains everything needed to draw a single object in object coordinates. The purpose of this class is to be contained
 * within a model class and allow the model class to render the mesh with a single call.
 * /see { occluded::opengl::retained::gl_attribute_buffer }
 */
class gl_retained_mesh:
	public occluded::meshes::mesh
{
private:
	const shaders::shader_program& m_shaderProg;
	
	gl_attribute_buffer m_buffer;
	primitive_type_t m_primitiveType;
	std::vector<unsigned int> m_indices;

	unsigned int m_numFaces;


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
	gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, 
		const buffer_usage_t usage = static_draw_usage, const primitive_type_t primitiveType = primitive_triangles );

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
		const primitive_type_t primitiveType = primitive_triangles );
	~gl_retained_mesh();

	/**
	 * \fn draw
	 * \brief Draws the mesh.
	 *
	 * * Draws the gl_retained_mesh to the context.
	 */
	void draw() const;

	/**
	 * \fn add_vertices
	 * \brief Adds vertices to the mesh.
	 *
	 * \param vertices A reference to a vector of bytes.
	 * \return A vector of unsigned ints representing the indices of the vertices added.
	 * 
	 * Adds the vertices contained in the vector to the gl_attribute_buffer in the gl_retained_mesh. An exception will be thrown if the vertices 
	 * vector is not formatted to be properly inserted into the data structure that is to contain the data.
	 */
	const std::vector<unsigned int> add_vertices( const std::vector<char>& vertices );

	/**
	 * \fn add_faces.
	 * \brief Adds faces to the mesh.
	 *
	 * \param faceIndices A reference to a vector of unsigned ints containing the indices of vertices that make up the faces to be added.
	 * \return A vector of unsigned ints representing the indices of the faces that were added.
	 *
	 * Adds the faces contained in the faceIndices vector to the mesh. An exception will be thrown if the faceIndices vector does not have a size
	 * that is a multiple of the number of vertices required for each face or if an index is encountered that doesn't have a corresponding vertex.
	 */
	//const std::vector<unsigned int> add_faces( const std::vector<unsigned int>& faceIndices );

	/**
	 * \fn add_face
	 * \brief Adds a single face to the mesh.
	 *
	 * \param faceIndices A reference to a vector of unsigned ints containing the indices of the vertices that make up the face to be added.
	 * \return A unsigned int representing the index of the face that was added.
	 *
	 * Adds a face to the mesh that is made up of the vertices specified by the indices in the faceIndices vector. An exception is thrown if the
	 * faceIndices vector does not contain the correct number of vertices or if it contains a index that does not correspond to a vertex in the
	 * the mesh.
	 */
	const unsigned int add_face( const std::vector<unsigned int>& faceIndcies );

	/**
	 * \fn get_num_faces
	 * \brief Gets the number of faces in the mesh.
	 *
	 * \return A unsigned in representing the nubmer of faces in the mesh.
	 */
	const unsigned int get_num_faces() const;

private:
	


	/**
	 * \fn get_num_verts_of_face
	 * \brief Gets the number of vertices that are required when adding another face.
	 * 
	 * \param primitiveType A primitive_type_t that represents the primitive that are the faces of the mesh.
	 * \return An unsigned int representing the number of vertices required for adding another primitive to the mesh. 
	 *
	 * Returns the number of vertices required for adding another face to the mesh. This may not be the same as the number of vertices required
	 * for initial face of the mesh. For example: if the primitive is primitive_triangles the initial number of vertices is equal to the number
	 * of vertices required to add another face where as if the primitive is primitive_triangle_strips the number of vertices required to initially
	 * add a face is 3, but to add another face only one vertex is required. A 0 is returned for primitive_patches since the number of vertices
	 * required for primitive_patches varies.
	 */
	static unsigned int get_num_verts_for_next_face( const primitive_type_t primitiveType );

	/**
	 * \fn get_num_verts_for_init_primitive
	 * \brief Get ths number of vertices that are required when adding the first face.
	 *
	 * \param primitiveType A primitive_type_t that represents the primitive that are the faces of the mesh.
	 * \return An unsigned in representing the number of vertices required for adding the first face to the mesh.
	 *
	 * Returns the number of vertices required for adding the first face to the mesh. This may not be the same as the number of vertices requied
	 * for every additional face of the mesh. For example: if the primitive_triangles the initial number of vertices is equal to the number of
	 * vertices required to add another face where as if the primitive is primitive_triangle_strips the number of vertices requied to initially
	 * add a face is 3, but to add another face only one vertex is required. A 0 is returned for primitive_patches since the number of vertices
	 * required for primitive_patches varies.
	 */
	static unsigned int get_num_verts_for_init_face( const primitive_type_t primitiveType );
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
