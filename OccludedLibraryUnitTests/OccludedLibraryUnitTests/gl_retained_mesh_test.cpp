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

		TEST_METHOD( gl_retained_mesh_add_faces_invalid_param_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute ( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_retained_mesh glMesh( testMap, shaderProg );

			std::vector<unsigned int> indices( 3 );
			indices[0] = 0; indices[1] = 1; indices[2] = 2;

			try {
				glMesh.add_faces( indices );

				// Test to make sure add_faces throws an exception when indices are inserted into a mesh with no vertices
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			std::vector<char> testData( 3 * sizeof( float ) );
			indices[0] = 0; indices[1] = 1; indices[2] = 4;

			glMesh.add_vertices( testData );

			try {
				glMesh.add_faces( indices );

				// Test to make sure add_faces throws an exception when an index being added as part of a face does not correspond
				// to a vertex in the mesh
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			indices[0] = 0; indices[1] = 1; indices[2] = 0;

			try {
				glMesh.add_faces( indices );

				// Test to make sure an exception is thrown when two indices of the same index are found in the same face.
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_mesh_add_faces_invalid_number_of_indices_test )
		{
			shader_program shaderProg( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_uint ) );
			testMap.end_definition();

			gl_retained_mesh testMesh1( testMap, shaderProg );
			std::vector<char> testVertices( 3 * sizeof( unsigned int ) );
			std::vector<unsigned int> testFaces( 1 );
			memset( &testFaces[0], 0, sizeof( unsigned int ) );

			testMesh1.add_vertices( testVertices );

			try {
				testMesh1.add_faces( testFaces );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_faces when no faces are currently present
				// and the primitive used has the same number of vertices required for the first face and for each subsequent face
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testFaces.resize( 3 );
			testFaces[0] = 0;
			testFaces[1] = 1;
			testFaces[2] = 2;

			testMesh1.add_faces( testFaces );

			testFaces.resize( 2 );

			try {
				testMesh1.add_faces( testFaces );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_faces when faces are already in the mesh
				// and the primitive used has the same nubmer of vertices required for the first face and for each subsequent faces
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			gl_retained_mesh testMesh2( testMap, shaderProg, static_draw_usage, primitive_triangle_fan );
			testVertices.resize( 3 * sizeof( unsigned int )  );
			testFaces.resize( 2 );
			testFaces[0] = 0;
			testFaces[1] = 1;

			try {
				testMesh2.add_faces( testFaces );

				// Test to make sure an exception is thrown if an invalid number of indices are passed to add_faces when no faces are currently present
				// and the primitive used has the different number of vertices required for the first face and for each subsequent faces
				Assert::Fail();
			} catch( const std::exception& ) {
			}


		}
	};
}