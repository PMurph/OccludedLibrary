#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/gl_retained_mesh.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained;
using namespace occluded::opengl::retained::shaders;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryUnitTests
{
	std::vector< const boost::shared_ptr<const shader> > shaders;

	TEST_CLASS( gl_retained_mesh_test )
	{
	public:
		TEST_CLASS_INITIALIZE( gl_retained_mesh_init )
		{
			std::string src( "Not Empty" );

			shaders.push_back( boost::shared_ptr<shader>( new shader( src, vert_shader ) ) );
			shaders.push_back( boost::shared_ptr<shader>( new shader( src, frag_shader ) ) );
		}

		TEST_METHOD( gl_retained_mesh_constructor_test )
		{
			std::auto_ptr<shader_program> shaderProg( new shader_program( shaders ) );

			attribute_map testMap( true );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );

			try {
				gl_retained_mesh testMesh( testMap, *shaderProg, static_draw_usage );

				// Test to make sure an exception is thrown when a attribute_map passed to the constructor is still being defined
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_mesh_add_vertices_test )
		{
			shader_program shaderProg( shaders );
			
			attribute_map testMap( true );
			testMap.add_attribute( attribute( "test1", 1, attrib_float ) );
			testMap.add_attribute( attribute( "test2", 1, attrib_uint ) );
			testMap.end_definition();

			gl_retained_mesh glMesh( testMap, shaderProg, static_draw_usage );
			
			std::vector<char> data( 3 * sizeof( float ) + 2 * sizeof( unsigned int ) );

			try {
				glMesh.add_vertices( data );

				// Test to make sure an exception is thrown in a vector of incorrect size is passed to the add_vertices function
				Assert::Fail();
			} catch( const std::exception& ) {

			}

			data.resize( 3 * sizeof( float ) + 3 * sizeof( unsigned int ) );
			std::vector<unsigned int> indices;

			try {
				indices = glMesh.add_vertices( data );
			} catch( const std::exception& ) {
				// Test to make sure no exception is thrown when valid vector is passed to add_vertices function
				Assert::Fail();
			}

			// Test to make sure the vector returned by add_vertices is the correct size
			Assert::AreEqual( static_cast<std::size_t>( 3 ), indices.size() );

			// Test to make sure the vector returned from add_vertices contains the correct values
			Assert::AreEqual( static_cast<unsigned int>( 0 ), indices[0] );
			Assert::AreEqual( static_cast<unsigned int>( 1 ), indices[1] );
			Assert::AreEqual( static_cast<unsigned int>( 2 ), indices[2] );

			data.resize( sizeof( float ) + sizeof( unsigned int ) );

			indices = glMesh.add_vertices( data );

			// Test to make sure the vector returned from add_vertices has the proper indices in it
			Assert::AreEqual( static_cast<unsigned int>( 3 ), indices[0] );
		}

		TEST_METHOD( gl_retained_mesh_add_face_invalid_param_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute ( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh glMesh( testMap, shaderProg );

			std::vector<unsigned int> indices( 3 );
			indices[0] = 0; indices[1] = 1; indices[2] = 2;

			try {
				glMesh.add_face( indices );

				// Test to make sure add_faces throws an exception when indices are inserted into a mesh with no vertices
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			std::vector<char> testData( 3 * sizeof( float ) );
			indices[0] = 0; indices[1] = 1; indices[2] = 4;

			glMesh.add_vertices( testData );

			try {
				glMesh.add_face( indices );

				// Test to make sure add_faces throws an exception when an index being added as part of a face does not correspond
				// to a vertex in the mesh
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			indices[0] = 0; indices[1] = 1; indices[2] = 0;

			try {
				glMesh.add_face( indices );

				// Test to make sure an exception is thrown when two indices of the same index are found in the same face.
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_mesh_add_face_invalid_number_of_indices_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_uint ) );
			testMap.end_definition();

			gl_retained_mesh testMesh1( testMap, shaderProg );
			std::vector<char> testVertices( 3 * sizeof( unsigned int ) );
			std::vector<unsigned int> testIndices( 1 );
			memset( &testIndices[0], 0, sizeof( unsigned int ) );

			testMesh1.add_vertices( testVertices );

			try {
				testMesh1.add_face( testIndices );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_faces when no faces are currently present
				// and the primitive used has the same number of vertices required for the first face and for each subsequent face
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testIndices.resize( 3 );
			testIndices[0] = 0;
			testIndices[1] = 1;
			testIndices[2] = 2;

			testMesh1.add_face( testIndices );

			testIndices.resize( 2 );

			try {
				testMesh1.add_face( testIndices );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_faces when faces are already in the mesh
				// and the primitive used has the same nubmer of vertices required for the first face and for each subsequent faces
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			gl_retained_mesh testMesh2( testMap, shaderProg, static_draw_usage, primitive_triangle_fan );
			testVertices.resize( 3 * sizeof( unsigned int )  );
			testIndices.resize( 2 );
			testIndices[0] = 0;
			testIndices[1] = 1;

			testMesh2.add_vertices( testVertices );

			try {
				testMesh2.add_face( testIndices );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_face when no faces are currently present
				// and the primitive used has the different number of vertices required for the first face and for each subsequent faces
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testMesh2.add_vertices( testVertices );
			testIndices.resize( 3 );
			testIndices[0] = 0;
			testIndices[1] = 1;
			testIndices[2] = 2;

			testMesh2.add_face( testIndices );

			testIndices[0] = 3;
			testIndices[1] = 4;
			testIndices[2] = 5;

			try { 
				testMesh2.add_face( testIndices );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_face when faces are currently present
				// and the primitive used has a different number of vertices required for the first face and for each subsequent faces
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_mesh_get_num_faces_test ) 
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh testMesh( testMap, shaderProg, static_draw_usage, primitive_triangle_fan );
			std::vector<char> vertices( 4 * sizeof( float ) );
			std::vector<unsigned int> indices( 3 );

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;

			testMesh.add_vertices( vertices );

			// Test to make sure get_num_faces returns 0 for an empty mesh
			Assert::AreEqual( testMesh.get_num_faces(), static_cast<unsigned int>( 0 ) );

			testMesh.add_face( indices );

			// Test to make sure that after a face is added get_num_faces returns 1 when there is 1 face in the mesh
			Assert::AreEqual( testMesh.get_num_faces(), static_cast<unsigned int>( 1 ) );

			indices.resize( 1 );
			indices[0] = 3;
			testMesh.add_face( indices );

			// Test to make sure that adding another face after the first one get_num_faces increases the number of faces returned
			Assert::AreEqual( testMesh.get_num_faces(), static_cast<unsigned int>( 2 ) );

			indices.resize( 3 );
			indices[0] = 1;
			indices[1] = 2;
			indices[2] = 3;
			testMesh.add_faces( indices );

			// Test to make sure the correct number is returned after an add_faces call is made that inserts mulitple faces
			Assert::AreEqual( testMesh.get_num_faces(), static_cast<unsigned int>( 5 ) );

			indices[1] = 20;

			try {
				testMesh.add_faces( indices );
			} catch( const std::exception& ) {
			}

			// Test to make sure that the number returned does not change if an exception occurs during adding of multiple faces
			Assert::AreEqual( testMesh.get_num_faces(), static_cast<unsigned int>( 5 ) );
			
		}

		TEST_METHOD( gl_retained_mesh_add_face_correct_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh testMesh( testMap, shaderProg, static_draw_usage, primitive_triangle_fan );
			std::vector<char> vertices( 4 * sizeof( float ) );
			std::vector<unsigned int> indices( 3 );

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;

			testMesh.add_vertices( vertices );

			// Test to make sure that the correct face index is returned when add_face is called when no faces are in the mesh
			Assert::AreEqual( testMesh.add_face( indices ), static_cast<unsigned int>( 0 ) );

			indices.resize( 1 );
			indices[0] = 3;

			// Test to make sure that the correct face index is returned when add_face is called when there is one face in the mesh
			Assert::AreEqual( testMesh.add_face( indices ), static_cast<unsigned int>( 1 ) );

			indices[0] = 1;

			// Test to make sure that the correct face index is returned when more that one face is in the mesh
			Assert::AreEqual( testMesh.add_face( indices ), static_cast<unsigned int>( 2 ) );
		}

		TEST_METHOD( gl_retained_mesh_add_faces_invalid_param_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh testMesh( testMap, shaderProg );
			std::vector<char> vertices( 5 * sizeof( float ) );
			std::vector<unsigned int> indices( 6 );

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 2;
			indices[4] = 3;
			indices[5] = 4;

			try {
				testMesh.add_faces( indices );

				// Test to make sure that an exception is thrown if add_faces is called on a mesh without vertices
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testMesh.add_vertices( vertices );

			indices.resize( 5 );

			try {
				testMesh.add_faces( indices );
			
				// Test to make sure that an exception is thrown if add_faces is called with an invalid number of indices is passed to it
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			indices.resize( 6 );
			indices[5] = 20;

			try {
				testMesh.add_faces( indices );

				// Test to make sure that an exception is thrown if an index is found in any of the faces that dose not correspond to a
				// vertex in the mesh
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_mesh_add_faces_valid_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh testMesh( testMap, shaderProg );
			std::vector<char> vertices( 5 * sizeof( float ) );
			std::vector<unsigned int> indices( 6 );

			testMesh.add_vertices( vertices );

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 1;
			indices[4] = 3;
			indices[5] = 4;

			std::vector<unsigned int> facesAdded = testMesh.add_faces( indices );

			// Test to make sure the correct face indices are returned from the add_faces call on an empty mesh with a primitive that requires
			// the same number of faces for the initial face and each subsequent face
			Assert::AreEqual( facesAdded.size(), static_cast<std::size_t>( 2 ) );
			Assert::AreEqual( facesAdded[0], static_cast<unsigned int>( 0 ) );
			Assert::AreEqual( facesAdded[1], static_cast<unsigned int>( 1 ) );

			gl_retained_mesh testMesh2( testMap, shaderProg, static_draw_usage, primitive_triangle_strip );

			testMesh2.add_vertices( vertices );
			indices.resize( 5 );

			facesAdded = testMesh2.add_faces( indices );

			// Test to make sure the correct face indices are returned from the add_faces call on an empty mesh with a primitive that requires
			// different numbers of faces for the initial face and each subsequent face
			Assert::AreEqual( facesAdded.size(), static_cast<std::size_t>( 3 ) );
			Assert::AreEqual( facesAdded[0], static_cast<unsigned int>( 0 ) );
			Assert::AreEqual( facesAdded[2], static_cast<unsigned int>( 2 ) );

			facesAdded = testMesh2.add_faces( indices );

			// Test to make sure the correct number of faces were added from the added from an add_faces call on a non-empty mesh with a primitive
			// that requires different numbers of faces for the initial face and each subsequent face
			Assert::AreEqual( facesAdded.size(), static_cast<std::size_t>( 5 ) );
			Assert::AreEqual( facesAdded[0], static_cast<unsigned int>( 3 ) );
			Assert::AreEqual( facesAdded[4], static_cast<unsigned int>( 7 ) );
		}

		TEST_METHOD( gl_retained_mesh_num_verts_for_next_face_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh testMesh( testMap, shaderProg );
			std::vector<char> vertices( 5 * sizeof( float ) );
			std::vector<unsigned int> indices( 3 );

			testMesh.add_vertices( vertices );
			
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;

			// Makes sure the correct number is returned for an empty mesh using a primitive that has the same vertices requirements for the
			// initial face as well as subsequent faces
			Assert::AreEqual( testMesh.num_verts_for_next_face( testMesh.get_num_faces() ), static_cast<unsigned int>( 3 ) );

			testMesh.add_face( indices );

			// Make sure the correct number is returned for a non empty mesh using a primitive that has the same vertices requirements for the
			// initial face as well as subsequent faces
			Assert::AreEqual( testMesh.num_verts_for_next_face( testMesh.get_num_faces() ), static_cast<unsigned int>( 3 ) );

			gl_retained_mesh testMesh2( testMap, shaderProg, static_draw_usage, primitive_lines );

			testMesh2.add_vertices( vertices );

			// Make sure the correct number is returned if a primitive with different vertice requirements for its faces is used
			Assert::AreEqual( testMesh2.num_verts_for_next_face( testMesh2.get_num_faces() ), static_cast<unsigned int>( 2 ) );

			gl_retained_mesh testMesh3( testMap, shaderProg, static_draw_usage, primitive_triangle_fan );

			testMesh3.add_vertices( vertices );
			testMesh3.add_face( indices );

			// Make sure that the correct number is returned if called on a non empty mesh with a primitive that has different vertice requirements
			// for the first face and subsequent faces
			Assert::AreEqual( testMesh3.num_verts_for_next_face( testMesh3.get_num_faces() ), static_cast<unsigned int>( 1 ) );
		}
	};
}