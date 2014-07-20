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

	/**
	 * \fn num_verts_for_next_face
	 * \brief Gets the number of vertices needed for the next face.
	 *
	 * \param numFaces An unsigned int that specifies a face in the mesh.
	 * \return Returns an unsigned int representing the number of vertices needed to add a new face.
	 *
	 * Gets the number of vertices needed for the next face to be added to the mesh after the face specified by numFaces parameter. This should 
	 * be called before adding a face to the mesh so that the chance of an exception being thrown by the call is minimized.
	 */
	virtual const unsigned int num_verts_for_next_face( const unsigned int numFaces ) const = 0;
};

} // end of meshes namespace
} // end of occluded namespace
