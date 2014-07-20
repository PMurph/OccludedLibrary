#include "gl_retained_mesh.h"

namespace occluded { namespace opengl { namespace retained {

gl_retained_mesh::gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, 
	const buffer_usage_t usage, const primitive_type_t primitiveType ):
	m_shaderProg( shaderProg ),
	m_buffer( gl_attribute_buffer( map, shaderProg, usage ) ),
	m_primitiveType( primitiveType ),
	m_numFaces( 0 )
{
}

gl_retained_mesh::gl_retained_mesh( const shaders::shader_program& shaderProg, gl_attribute_buffer& buffer, const std::vector<unsigned int>& faces,
	const primitive_type_t primitiveType ):
	m_shaderProg( shaderProg ),
	m_buffer( buffer ),
	m_primitiveType( primitiveType ),
	m_numFaces( 0 )
{
}


gl_retained_mesh::~gl_retained_mesh()
{
}

void gl_retained_mesh::draw() const {
}

const std::vector<unsigned int> gl_retained_mesh::add_vertices( const std::vector<char>& vertices ) {
	std::vector<unsigned int> indices;
	unsigned int initNumVals = m_buffer.get_num_values();

	m_buffer.insert_values( vertices );

	for( unsigned int i = initNumVals; i < m_buffer.get_num_values(); ++i ) {
		indices.push_back( i );
	}

	return indices;
}

const std::vector<unsigned int> gl_retained_mesh::add_faces( const std::vector<unsigned int>& faceIndices ) {
	unsigned int currIndex = 0, currFace = m_numFaces;
	std::vector< std::vector<unsigned int> > toAdd;
	std::vector<unsigned int> addedFaces;

	// Go through each face
	while( currIndex < faceIndices.size() ) {
		const unsigned int numIndicesForFace = num_verts_for_next_face( currFace );
		std::vector<unsigned int> indices( numIndicesForFace );

		if( numIndicesForFace > static_cast<unsigned int>( faceIndices.size() ) - currIndex )
			throw std::exception( "gl_retained_mesh.add_faces: Failed to add faces because incorrect number of indices were found in faceIndices parameter." );

		memcpy( &indices[0], &faceIndices[currIndex], numIndicesForFace * sizeof( unsigned int ) );

		check_face( indices );

		currIndex += numIndicesForFace;
		++currFace;
		toAdd.push_back( indices );
	}

	// Add the faces to mesh
	for( std::vector< std::vector<unsigned int> >::const_iterator it = toAdd.begin(); it != toAdd.end(); ++it ) {
		addedFaces.push_back( m_numFaces );

		insert_face( *it );
	}

	return addedFaces;
}

const unsigned int gl_retained_mesh::add_face( const std::vector<unsigned int>& faceIndices ) {
	std::vector<unsigned int> indices( 0 );
	unsigned int faceNumber = m_numFaces;
	
	// Throw an exception if the incorrect number of faces are passed as the parameter. There are two cases:
	//   1) If there are currently no faces in the mesh, throw an exception if the number of indices passed is not the correct number of 
	//		indices for the first face.
	//   2) If there are currently faces in the mesh, throw an exception if the number of indices passed is not the correct number of indices
	//		for each subsequent face.
	if( ( m_indices.size() == 0 &&  faceIndices.size() != get_num_verts_for_init_face( m_primitiveType )  ) 
		|| ( m_indices.size() != 0 && faceIndices.size() != get_num_verts_for_next_face( m_primitiveType ) ) ) {
		throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces to the mesh because an incorrect number of indices(" + 
			boost::lexical_cast<std::string>( faceIndices.size() ) + ") where passed to the function." );
	}

	check_face( faceIndices );
	insert_face( faceIndices );

	return faceNumber;
}

const unsigned int gl_retained_mesh::get_num_faces() const {
	return m_numFaces;
}

const unsigned int gl_retained_mesh::num_verts_for_next_face( const unsigned int numFaces ) const {
	unsigned int numVerts = 0;

	if( numFaces == 0 )
		numVerts = get_num_verts_for_init_face( m_primitiveType );
	else
		numVerts = get_num_verts_for_next_face( m_primitiveType );

	return numVerts;
}

// Private Member Functions

void gl_retained_mesh::check_face( const std::vector<unsigned int>& faceIndices ) const {
	boost::unordered_set<unsigned int> addedIndices;
	const unsigned int numVertices = m_buffer.get_num_values();

	if( numVertices == 0 )
		throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces to the mesh because there are no vertices in the mesh." );

	// Check to make sure there are no indices that don't correspond to a vertex and that there are no duplicate indices in the face
	for( std::vector<unsigned int>::const_iterator it = faceIndices.begin(); it != faceIndices.end(); ++it ) {
		if( *it >= numVertices ) {
			throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces because an index(" + boost::lexical_cast<std::string>( *it )
				+ ") that dose not correspond to vertex was found." );
		}

		if( addedIndices.find( *it ) != addedIndices.end() ) {
			throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces because an index(" + boost::lexical_cast<std::string>( *it ) 
				+ ") appears more than once in the same face." );
		}

		addedIndices.insert( *it );
	}
}

const unsigned int gl_retained_mesh::insert_face( const std::vector<unsigned int>& faceIndices ) {
	unsigned int newFaceIndex = m_numFaces;
	
	for( std::vector<unsigned int>::const_iterator it = faceIndices.begin(); it != faceIndices.end(); ++it ) {
		m_indices.push_back( *it );
	}

	m_numFaces++;

	return newFaceIndex;
}

// Static Functions

unsigned int gl_retained_mesh::get_num_verts_for_next_face( const primitive_type_t primitiveType ) {
	unsigned int numVerts = 0;
	
	switch( primitiveType ) {
	case primitive_point:
	case primitive_line_loop:
	case primitive_line_strip:
	case primitive_triangle_fan:
	case primitive_triangle_strip:
		numVerts = 1;
		break;
	case primitive_lines:
		numVerts = 2;
		break;
	case primitive_triangles:
		numVerts = 3;
		break;
	case primitive_patches:
	default:
		numVerts = 0;
		break;
	}

	return numVerts;
}

unsigned int gl_retained_mesh::get_num_verts_for_init_face( const primitive_type_t primitiveType ) {
	unsigned int numVerts = 0;
	
	switch( primitiveType ) {
	case primitive_point:
		numVerts = 1;
		break;
	case primitive_lines:
	case primitive_line_loop:
	case primitive_line_strip:
		numVerts = 2;
		break;
	case primitive_triangles:
	case primitive_triangle_fan:
	case primitive_triangle_strip:
		numVerts = 3;
		break;
	case primitive_patches:
	default:
		numVerts = 0;
		break;
	}

	return numVerts;
}

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
