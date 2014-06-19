#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/gl_attribute_buffer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained;
using namespace occluded::opengl::retained::shaders;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryUnitTests
{
	static std::vector< const boost::shared_ptr<const shader> > shaders;

	TEST_CLASS( gl_attribute_buffer_test )
	{
	public:
		TEST_CLASS_INITIALIZE( gl_attribute_buffer_init )
		{
			std::string src( "Not Empty" );

			shaders.push_back( boost::shared_ptr<shader>( new shader( src, vert_shader ) ) );
			shaders.push_back( boost::shared_ptr<shader>( new shader( src, frag_shader ) ) );
		}
		
		TEST_METHOD( gl_attribute_buffer_get_id_test )
		{
			std::auto_ptr<shader_program> testProgram( new shader_program( shaders ) );
			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_attribute_buffer testBuffer( testMap, *testProgram, static_draw_usage );

			// Test to make sure the an id is returned
			Assert::AreEqual( testBuffer.get_id(), static_cast<unsigned int>( 1 ) );
		}

		TEST_METHOD( gl_attribute_buffer_get_attribute_map_test )
		{
			std::auto_ptr<shader_program> testProgram( new shader_program( shaders ) );
			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			try {
				gl_attribute_buffer testBuffer( testMap, *testProgram, static_draw_usage );

				// Test to make sure the correct attribute map is returned
				Assert::IsTrue( testBuffer.get_buffer_map() == testMap );
			} catch ( const std::exception& ) {
				Assert::Fail();
			}

			testMap.reset( true );
			testMap.add_attribute( attribute( "test1", 1, attrib_uint ) );
			testMap.add_attribute( attribute( "test2", 2, attrib_float ) );
			testMap.end_definition();

			try {
				gl_attribute_buffer testBuffer1( testMap, *testProgram, stream_draw_usage );

				// Test to make sure the attribute map is actually different if a different map is passed as a parameter
				Assert::IsTrue( testBuffer1.get_buffer_map() == testMap );
			} catch( const std::exception& ) {
				Assert::Fail();
			}
		}

		TEST_METHOD( gl_attribute_buffer_get_usage_test )
		{
			std::auto_ptr<shader_program> testProgram( new shader_program( shaders ) );
			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			try	{ 
				gl_attribute_buffer testBuffer( testMap, *testProgram, stream_draw_usage );

				// Test to make sure that the stream_draw_usage is returned by get_usage
				Assert::IsTrue( testBuffer.get_usage() == stream_draw_usage );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			try {
				gl_attribute_buffer testBuffer2( testMap, *testProgram, dynamic_draw_usage );

				// Test to make sure that the dynamic_draw_usage is returned by get_usage
				Assert::IsTrue( testBuffer2.get_usage() == dynamic_draw_usage );
			} catch( const std::exception& ) {
				Assert::Fail();
			}
		}

		TEST_METHOD( gl_attribute_buffer_get_num_values_test )
		{
			shader_program testProgram( shaders );

			attribute_map testMap( false );
			testMap.add_attribute( attribute( "test", 1, attrib_float ) );
			testMap.end_definition();

			gl_attribute_buffer testBuffer( testMap, testProgram, static_draw_usage );

			// Test to make sure the number of values in the gl_attribute_buffer is initially 0
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_num_values() );

			std::vector<char> data( 3 * sizeof( float ) );

			testBuffer.insert_values( data );

			// Test to make sure the nubmer of values is 3 after 3 values are inserted
			Assert::AreEqual( static_cast<unsigned int>( 3 ), testBuffer.get_num_values() );
		}
	};
}