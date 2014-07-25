#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader_attribute_map.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained::shaders;
using namespace occluded::buffers::attributes;

/**
 * NOTE: This is not a complete verification of the function of shader class since it relies heavily on how 
 * OpenGL functions.
 */
namespace OccludedLibraryUnitTests
{
	static std::vector< const boost::shared_ptr< const shader > > shaders;

	TEST_CLASS( shader_attribute_map_test )
	{
	public:
		TEST_CLASS_INITIALIZE( shader_attribute_map_init_test )
		{
			std::string src( "Not Empty" );
			errorState = false;

			shaders.push_back( boost::shared_ptr<const shader>( new shader( src, vert_shader ) ) );
			shaders.push_back( boost::shared_ptr<const shader>( new shader( src, frag_shader ) ) );
		}

		TEST_METHOD( shader_attribute_map_constructor_test )
		{
			attribute_map testMap( false );
			std::auto_ptr<shader_program> testShaderProg;
			
			errorState = false;
			testShaderProg.reset( new shader_program( shaders ) );

			try {
				shader_attribute_map testShaderMap( testMap,  *testShaderProg );

				// Test to make sure an exception is thrown if an attribute map is still being defined
				Assert::Fail();
			} catch( const std::exception& ) {
				
			}

			testShaderProg.reset( new shader_program( shaders ) );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			errorState = true;

			try {
				shader_attribute_map testShaderMap( testMap, *testShaderProg );

				// Test to make sure an exception is thrown if the shader program is no linked
				Assert::Fail();
			} catch ( const std::exception& ) {
			
			}

			errorState = false;
			testShaderProg.reset( new shader_program( shaders ) );

			try {
				shader_attribute_map testShaderMap( testMap, *testShaderProg );
				// Test to make sure no exception is thrown if the proper parameters are provided to the constructor
			} catch( const std::exception& ) {
				Assert::Fail();
			}
		}
	};
}