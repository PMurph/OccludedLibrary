#include "gl_retained_mesh.h"

namespace occluded { namespace opengl { namespace retained {

gl_retained_mesh::gl_retained_mesh( const occluded::buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, 
	const buffer_usage_t usage, const primitive_type_t primitiveType ):
	m_shaderProg( shaderProg ),
	m_buffer( gl_attribute_buffer( map, shaderProg, usage ) ),
	m_primitiveType( primitiveType )
{
}

gl_retained_mesh::gl_retained_mesh( const shaders::shader_program& shaderProg, gl_attribute_buffer& buffer, const std::vector<unsigned int>& faces,
	const primitive_type_t primitiveType ):
	m_shaderProg( shaderProg ),
	m_buffer( buffer ),
	m_primitiveType( primitiveType )
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

const std::vector<unsigned int> gl_retained_mesh::add_faces( const std::vector<unsigned int>& faces ) {
	boost::unordered_set<unsigned int> faceIndices;
	unsigned int numVertices = m_buffer.get_num_values();
	
	if( numVertices == 0 )
		throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces to the mesh because there are no vertices in the mesh." );

	if( faces.size() % get_num_verts_for_next_face( m_primitiveType ) != 0 ) {
		throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces to the mesh because an incorrect number of indices(" + 
			boost::lexical_cast<std::string>( numVertices ) + ") where passed to the function." );
	}

	for( std::vector<unsigned int>::const_iterator it = faces.begin(); it != faces.end(); ++it ) {
		if( *it >= numVertices ) {
			throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces because an index(" + boost::lexical_cast<std::string>( *it )
				+ ") that dose not correspond to vertex was found." );
		}

		if( faceIndices.find( *it ) != faceIndices.end() ) {
			throw std::runtime_error( "gl_retained_mesh.add_faces: Failed to add faces because an index(" + boost::lexical_cast<std::string>( *it ) 
				+ ") appears more than once in the same face." );
		}

		faceIndices.insert( *it );
	}

	return std::vector<unsigned int>( 0 );
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
