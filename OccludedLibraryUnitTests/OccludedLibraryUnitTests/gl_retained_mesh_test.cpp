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
		}
	};
}