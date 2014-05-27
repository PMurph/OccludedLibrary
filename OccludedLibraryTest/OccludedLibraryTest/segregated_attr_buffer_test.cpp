#include "stdafx.h"
#include "CppUnitTest.h"

#include <buffers/segregated_attr_buffer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::buffers;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryTest
{
	static const float MAX_ERR = 0.0001f;

	static attribute_map* testMap = NULL;

	TEST_CLASS( segregated_attr_buffer_test )
	{
	public:
		TEST_CLASS_INITIALIZE( segregated_attr_buffer_class_init )
		{
			testMap = new attribute_map( false );
		}

		TEST_CLASS_CLEANUP( segregated_attr_buffer_class_delete )
		{
			delete testMap;
			testMap = NULL;
		}

		TEST_METHOD_INITIALIZE( segregated_attr_buffer_method_init )
		{
			attribute testAttrib( "test", 1, attrib_float );
			testMap->reset( false );
			testMap->add_attribute( testAttrib );
		}

		TEST_METHOD( segregated_attr_buffer_invalid_map_test )
		{
			testMap->reset( true );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->end_definition();

			try {
				segregated_attr_buffer testBuffer( *testMap );

				// Test to make sure an exception is thrown if an interleaved attribute map is passed to a segregated attribute buffer's constructor
				Assert::Fail();
			} catch( const std::exception& ) {

			}
		}

		TEST_METHOD( segregated_attr_buffer_get_all_data_test )
		{
			attribute otherAttrib( "test2", 2, attrib_int );
			testMap->add_attribute( otherAttrib );
			testMap->end_definition();

			segregated_attr_buffer testBuffer( *testMap );

			std::vector<char> data( sizeof( float ) + 2 * sizeof( int ) );
			float value = 1.5f;
			int testInt1 = 10, testInt2 = -10;
			memcpy( &data[0], &value, sizeof( float ) );
			memcpy( &data[sizeof( float )], &testInt1, sizeof( int ) );
			memcpy( &data[sizeof( float ) + sizeof( int )], &testInt2, sizeof( int ) );

			testBuffer.insert_values( data );

			value = 2.0f;
			testInt1 = 15;
			testInt2 = -15;
			memcpy( &data[0], &value, sizeof( float ) );
			memcpy( &data[sizeof( float )], &testInt1, sizeof( int ) );
			memcpy( &data[sizeof( float ) + sizeof( int )], &testInt2, sizeof( int ) );

			testBuffer.insert_values( data );

			const std::vector<char>& testData = testBuffer.get_all_data();
			float testFloatVal1 = *( reinterpret_cast<const float*>( &testData[0] ) );
			float testFloatVal2 = *( reinterpret_cast<const float*>( &testData[sizeof( float )] ) );
			int testIntVal1 = *( reinterpret_cast<const int*>( &testData[2 * sizeof( float )] ) );
			int testIntVal2 = *( reinterpret_cast<const int*>( &testData[2 * sizeof( float ) + sizeof( int )] ) );
			int testIntVal3 = *( reinterpret_cast<const int*>( &testData[2 * sizeof( float ) + 2 * sizeof( int )] ) );
			int testIntVal4 = *( reinterpret_cast<const int*>( &testData[2 * sizeof( float ) + 3 * sizeof( int )] ) );

			// Test to make sure two values can be added to a segregated buffer
			Assert::IsTrue( testFloatVal1 >= 1.5f - MAX_ERR && testFloatVal1 <= 1.5f + MAX_ERR );
			Assert::IsTrue( testFloatVal2 >= 2.0f - MAX_ERR && testFloatVal2 <= 2.0f + MAX_ERR );
			Assert::AreEqual( testIntVal1, 10 );
			Assert::AreEqual( testIntVal2, -10 );
			Assert::AreEqual( testIntVal3, 15 );
			Assert::AreEqual( testIntVal4, -15 );

			testBuffer.clear_buffer();

			data.resize( data.size() + sizeof( float ) + 2 * sizeof( int ) );
			
			value = 2.5f;
			testInt1 = 25;
			testInt2 = -25;
			memcpy( &data[0], &value, sizeof( float ) );
			value = -2.5f;
			memcpy( &data[sizeof( float )], &value, sizeof( float ) );
			memcpy( &data[2 * sizeof( float )], &testInt1, sizeof( int ) );
			memcpy( &data[2 * sizeof( float ) + sizeof( int )], &testInt2, sizeof( int ) );
			testInt1 = 45;
			testInt2 = 1;
			memcpy( &data[2 * sizeof( float ) + 2 * sizeof( int )], &testInt1, sizeof( int ) );
			memcpy( &data[2 * sizeof( float ) + 3 * sizeof( int )], &testInt2, sizeof( int ) );

			testBuffer.insert_values( data );

			// Test to make sure multipule segregated values values can be inserted into the buffer in a single call
			Assert::AreEqual( static_cast<unsigned int>( 2 ), testBuffer.get_num_values() );

			testFloatVal1 = *( reinterpret_cast<const float*>( &testBuffer.get_all_data()[0] ) );
			testFloatVal2 = *( reinterpret_cast<const float*>( &testBuffer.get_all_data()[sizeof( float )] ) );
			testIntVal1 = *( reinterpret_cast<const int*>( &testBuffer.get_all_data()[2*sizeof( float )] ) );
			testIntVal2 = *( reinterpret_cast<const int*>( &testBuffer.get_all_data()[2*sizeof( float ) + sizeof( int )] ) );
			testIntVal3 = *( reinterpret_cast<const int*>( &testBuffer.get_all_data()[2*sizeof( float ) + 2*sizeof( int )] ) );
			testIntVal4 = *( reinterpret_cast<const int*>( &testBuffer.get_all_data()[2*sizeof( float ) + 3*sizeof( int )] ) );

			Assert::IsTrue( testFloatVal1 >= 2.5f - MAX_ERR && testFloatVal1 <= 2.5f + MAX_ERR );
			Assert::IsTrue( testFloatVal2 >= -2.5f - MAX_ERR && testFloatVal2 <= -2.5f + MAX_ERR );
			Assert::AreEqual( testIntVal1, 25 );
			Assert::AreEqual( testIntVal2, -25 );
			Assert::AreEqual( testIntVal3, 45 );
			Assert::AreEqual( testIntVal4, 1 );
		}

		TEST_METHOD( segregated_attr_buffer_insert_invalid_param_test )
		{
			testMap->reset( false );
			testMap->end_definition();

			segregated_attr_buffer testBuffer( *testMap );
			std::vector<char> data( 4 );

			try {
				testBuffer.insert_values( data );
				
				// Test to see if an exception is thrown when trying to insert values to a buffer with no attributes in its attribute map
				Assert::Fail();
			} catch ( std::exception& ) {
			}

			testMap->reset( false );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->end_definition();

			data.resize( 5 );

			segregated_attr_buffer testBuffer2( *testMap );

			try {
				testBuffer2.insert_values( data );

				// Test to see if an exception is thrown when an vector of invalid size is passed to the insert_values function
				Assert::Fail();
			} catch( std::exception& ) {
			}
		}

		// Make an interleaved equivalent
		TEST_METHOD( segregated_attr_buffer_get_attribute_data_offsets_empty_buffer_test )
		{
			testMap->end_definition();

			segregated_attr_buffer testBuffer( *testMap );

			// Test to make sure an empty buffer with a single attribute returns a vector with a single offset that is 0
			Assert::AreEqual( static_cast<std::size_t>( 1 ), testBuffer.get_attribute_data_offsets().size() );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );

			testMap->reset( false );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->add_attribute( attribute( "test2", 1, attrib_uint ) );
			testMap->end_definition();

			segregated_attr_buffer testBuffer2( *testMap );

			// Test to make sure an empty buffer with two attributes returns a vector with two offsets of 0
			Assert::AreEqual( static_cast<std::size_t>( 2 ), testBuffer2.get_attribute_data_offsets().size() );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer2.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer2.get_attribute_data_offsets()[1] );
		}

		TEST_METHOD( segregated_attr_buffer_get_attribute_data_offsets_segregated_test )
		{
			testMap->add_attribute( attribute( "test2", 2, attrib_int ) );
			testMap->end_definition();
			
			segregated_attr_buffer testBuffer( *testMap );
			std::vector<char> data( sizeof( float ) + 2 * sizeof( int ) );

			testBuffer.insert_values( data );

			// Test to make sure the offsets adjust properly after a single value is inserted into the buffer
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 4 ), testBuffer.get_attribute_data_offsets()[1] );

			data.resize( 2 * ( sizeof( float ) + 2 * sizeof( int ) ) );

			// Test to make sure the offsets adjust properly after two additional values are inserted into the buffer
			testBuffer.insert_values( data );

			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 12 ), testBuffer.get_attribute_data_offsets()[1] );

			// Test to make sure the offsets are reset after buffer is cleared
			testBuffer.clear_buffer();
			
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[1] );
		}
	};
}