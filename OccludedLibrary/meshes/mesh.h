#pragma once

#include <vector>

namespace occluded { namespace meshes {

/**
 * \class mesh
 * \brief An abstract class that provides an interface for all mesh classes.
 *
 * An abstract class that provides an interface for all mesh classes. Since meshes are one of the basic objects that will be used in
 * the occluded library, a common interface must be created for all meshes so that any graphics API can be used without changing the
 * way in which the library is used by the end user.
 */
class mesh {
public:
	/**
	 * \brief Initializes the mesh.
	 */
	mesh() {}
	~mesh() {}

	/**
	 * \fn draw
	 * \brief Draws the mesh.
	 *
	 * Draws the mesh to the context.
	 */
	virtual void draw() const = 0;

	/**
	 * \fn add_vertices
	 * \brief Adds vertices to the mesh.
	 *
	 * \param vertices A reference to a vector of bytes.
	 * \return A vector of unsigned ints representing the indices of the vertices added.
	 * 
	 * Adds the vertices contained in the vector to the mesh. An exception will be thrown if the vertices vector is not formatted to be properly
	 * inserted into the data structure that is to contain the data.
	 */
	virtual const std::vector<unsigned int> add_vertices( const std::vector<char>& vertices ) = 0;

	/**
	 * \fn add_faces.
	 * \brief Adds faces to the mesh.
	 *
	 * \param faces A reference to a vector of unsigned ints containing the indices of vertices that make up the face.
	 * \return A vector of unsigned ints representing the indices of the faces added.
	 *
	 * Adds the faces contained in the faceIndices vector to the mesh. An exception will be thrown if the faceIndices vector does not have a size
	 * that is a multiple of the number of vertices required for each face or if an index is encountered that doesn't have a corresponding vertex.
	 */
	//virtual const std::vector<unsigned int> add_faces( const std::vector<unsigned int>& faces ) = 0;

	/**
	 * \fn add_face
	 * \brief Adds a single face to the mesh
	 *
	 * \param faceIndices A reference to a vector of unsigned ints containing the indices of the vertices that make up the face to be added.
	 * \return A unsigned int representing the index of the face that was added.
	 *
	 * Adds a face to the mesh that is made up of the vertices specified by the indices in the faceIndices vector. An exception is thrown if the
	 * faceIndices vector does not contain the correct number of vertices or if it contains a index that does not correspond to a vertex in the
	 * the mesh.
	 */
	virtual const unsigned int add_face( const std::vector<unsigned int>& faceIndices ) = 0;
};

} // end of meshes namespace
} // end of occluded namespace
