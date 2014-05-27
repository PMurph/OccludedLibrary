#include "stdafx.h"
#include "CppUnitTest.h"

#include <buffers/segregated_attr_buffer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::buffers;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryTest
{
	static const float MAX_ERR = 0.0001f;

	static attribute_buffer* testBuffer = NULL;
	static attribute_map* testMap = NULL;

	TEST_CLASS( attribute_buffer_test )
	{
	public:
		TEST_CLASS_INITIALIZE( attribute_buffer_class_init )
		{
			testMap = new attribute_map( false );
		}

		TEST_CLASS_CLEANUP( attribute_buffer_class_delete )
		{
			delete testMap;
			testMap = NULL;
		}

		TEST_METHOD_INITIALIZE( attribute_buffer_method_init )
		{
			attribute testAttrib( "test", 1, attrib_float );
			testMap->reset( false );
			testMap->add_attribute( testAttrib );
		}

		TEST_METHOD_CLEANUP( attribute_buffer_method_cleanup )
		{
			delete testBuffer;
			testBuffer = NULL;
		}
		
		TEST_METHOD( attribute_buffer_constructor_test )
		{
			// Test to see if an exception is thrown if the attribute map is still being defined
			try {
				testBuffer = new segregated_attr_buffer( *testMap );
				Assert::Fail();
			} catch ( std::exception& ) {

			}

			testMap->end_definition();
			delete testBuffer;
			testBuffer = NULL;

			try {
				testBuffer = new segregated_attr_buffer( *testMap );
			} catch ( std::exception& ) {
				Assert::Fail();
			}
		}

		TEST_METHOD( attribute_buffer_clear_buffer_test )
		{
			testMap->end_definition();
			testBuffer = new segregated_attr_buffer( *testMap );
			std::vector<char> data( sizeof( float ) );
			float value = 4.0f;
			
			memcpy( &data[0], &value, sizeof( float ) );

			testBuffer->insert_values( data );
			testBuffer->clear_buffer();

			// Test to make sure the number of values, and the data of the buffer are set to 0 when clear is called
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer->get_num_values() );
			Assert::AreEqual( static_cast<std::size_t>( 0 ), testBuffer->get_all_data().size() );
		}

		TEST_METHOD( attribute_buffer_get_map_test )
		{
			testMap->end_definition();
			testBuffer = new segregated_attr_buffer( *testMap );

			// Test to see if the attribute map returned contains the same attributes as the attribute map passed to its constructor
			Assert::AreEqual( static_cast<unsigned int>( 1 ), testBuffer->get_attribute_map().get_attrib_count() );
			Assert::AreEqual( std::string( "test" ), testBuffer->get_attribute_map().get_attributes()[0].get_name() );
		}

		TEST_METHOD( attribute_buffer_get_num_values_test )
		{
			testMap->end_definition();
			testBuffer = new segregated_attr_buffer( *testMap );
			std::vector<char> data( 2 * sizeof( float ) );

			// Test to see if the number of values is initially 0
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer->get_num_values() );

			testBuffer->insert_values( data );

			// Test to see if the number of values is 2 after 2 values are inserted
			Assert::AreEqual( static_cast<unsigned int>( 2 ), testBuffer->get_num_values() );
		}

		TEST_METHOD( attribute_buffer_get_all_data_simple_test )
		{
			// Includes all test on single attribute attribute_map 
			testMap->end_definition();
			testBuffer = new segregated_attr_buffer( *testMap );

			std::vector<char> data( sizeof( float ) );
			float value = 1.5f;
			memcpy( &data[0], reinterpret_cast<char*>( &value ), sizeof( float ) );

			testBuffer->insert_values( data );
			float testVal = *( reinterpret_cast<const float*>( &testBuffer->get_all_data()[0] ) );

			// Test to see if a single value can be inserted into to the buffer
			Assert::IsTrue( testVal >= value - MAX_ERR && testVal <= value + MAX_ERR );

			value = 2.0f;
			memcpy( &data[0], reinterpret_cast<char *>( &value ), sizeof( float ) );

			testBuffer->insert_values( data );
			testVal = *( reinterpret_cast<const float*>( &testBuffer->get_all_data()[0] ) );

			// Test to see if the first value in the data returned is the initial value added to the buffer
			Assert::IsTrue( testVal >= 1.5f - MAX_ERR && testVal <= 1.5f + MAX_ERR );
			
			// Test to see if the second value is the newly added value
			testVal = *( reinterpret_cast<const float*>( &testBuffer->get_all_data()[sizeof( float )] ) );
			Assert::IsTrue( testVal >= value - MAX_ERR && testVal <= value + MAX_ERR );

			testBuffer->clear_buffer();
			value = 2.5f;
			memcpy( &data[0], reinterpret_cast<char *>( &value ), sizeof( float ) );
			testBuffer->insert_values( data );
			testVal = *( reinterpret_cast<const float*>( &testBuffer->get_all_data()[0] ) );

			Assert::IsTrue( testVal >= 2.5f - MAX_ERR && testVal <= 2.5f + MAX_ERR );
		}

		TEST_METHOD( attribute_buffer_get_byte_size_test )
		{
			testMap->end_definition();

			testBuffer = new segregated_attr_buffer( *testMap );
			std::vector<char> data( sizeof( float ) );

			// Test to make sure an empty buffer has a byte size of 0
			Assert::AreEqual( static_cast<std::size_t>( 0 ), testBuffer->get_byte_size() );

			testBuffer->insert_values( data );

			// Test to make sure a buffer with a single float value has a byte size of 4
			Assert::AreEqual( static_cast<std::size_t>( sizeof( float ) ), testBuffer->get_byte_size() );

			testBuffer->clear_buffer();

			// Test to make sure the buffer byte size is 0 after being cleared
			Assert::AreEqual( static_cast<std::size_t>( 0 ), testBuffer->get_byte_size() );

			testMap->reset( false );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->add_attribute( attribute( "test2", 2, attrib_uint ) );
			testMap->end_definition();

			delete testBuffer;
			testBuffer = NULL;
			
			testBuffer = new segregated_attr_buffer( *testMap );
			data.resize( 2 * sizeof( float ) + 4 * sizeof( unsigned int ) );

			testBuffer->insert_values( data );

			// Test to make sure that the byte size adjust correctly for more complex attribute maps
			Assert::AreEqual( static_cast<std::size_t>( 24 ), testBuffer->get_byte_size() );
		}
	};
}