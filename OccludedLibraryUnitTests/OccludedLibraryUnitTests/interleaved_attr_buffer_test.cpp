#include "stdafx.h"
#include "CppUnitTest.h"

#include <buffers/interleaved_attr_buffer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::buffers;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryUnitTests
{
	static const float MAX_ERR = 0.0001f;

	static attribute_map* testMap = NULL;

	TEST_CLASS( interleaved_attr_buffer_test )
	{
	public:
		
		TEST_CLASS_INITIALIZE( interleaved_attr_buffer_class_init )
		{
			testMap = new attribute_map( true );
		}

		TEST_CLASS_CLEANUP( interleaved_attr_buffer_class_delete )
		{
			delete testMap;
			testMap = NULL;
		}

		TEST_METHOD_INITIALIZE( interleaved_attr_buffer_method_init )
		{
			attribute testAttrib( "test", 1, attrib_float );
			testMap->reset( true );
			testMap->add_attribute( testAttrib );
		}

		TEST_METHOD( interleaved_attr_buffer_invalid_map_test )
		{
			testMap->reset( false );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->end_definition();

			try {
				interleaved_attr_buffer testBuffer( *testMap );

				// Test to make sure an exception is thrown if a segregated attribute map is passed to an interleaved attribute buffer's constructor
				Assert::Fail();
			} catch( const std::exception& ) {

			}
		}

		TEST_METHOD( interleaved_attr_buffer_get_all_data_test )
		{
			testMap->reset( true );
			testMap->add_attribute( attribute( "test1", 1, attrib_uint ) );
			testMap->add_attribute( attribute( "test2", 1, attrib_float ) );
			testMap->end_definition();

			interleaved_attr_buffer testBuffer( *testMap );
			unsigned int intVal = 4, intVal2 = 5, testUIntVal;
			float floatVal = 1.25f, floatVal2 = 3.45f, testFloatVal;
			std::vector<char> data( sizeof( float ) + sizeof( unsigned int ) );
			std::vector<char> testData;
			memcpy( &data[0], &intVal, sizeof( unsigned int ) );
			memcpy( &data[sizeof( unsigned int )], &floatVal, sizeof( float ) );

			testBuffer.insert_values( data );

			testUIntVal = *( reinterpret_cast<const unsigned int*>( &testBuffer.get_all_data()[0] ) );
			testFloatVal = *( reinterpret_cast<const float*>( &testBuffer.get_all_data()[sizeof( unsigned int )] ) );

			// Test to see if one interleaved value can be inserted properly
			Assert::AreEqual( static_cast<unsigned int>( 4 ), testUIntVal );
			Assert::IsTrue(  testFloatVal >= 1.25f - MAX_ERR && testFloatVal <= 1.25f + MAX_ERR );

			testBuffer.clear_buffer();

			intVal = 1;
			floatVal = -3.124f;
			data.resize( 2 * sizeof( float ) + 2 * sizeof( unsigned int ) );

			memcpy( &data[0], &intVal, sizeof( unsigned int ) );
			memcpy( &data[sizeof( float )], &floatVal, sizeof( float ) );
			memcpy( &data[sizeof( float ) + sizeof( unsigned int )], &intVal2, sizeof( unsigned int ) );
			memcpy( &data[sizeof( float ) + 2 * sizeof( unsigned int )], &floatVal2, sizeof( float ) );

			testBuffer.insert_values( data );

			testUIntVal = *( reinterpret_cast<const unsigned int*>( &testBuffer.get_all_data()[0] ) );
			testFloatVal = *( reinterpret_cast<const float*>( &testBuffer.get_all_data()[sizeof( unsigned int )] ) );

			// Test to see if two interleaved value can be inserted properly
			Assert::AreEqual( static_cast<unsigned int>( 1 ), testUIntVal );
			Assert::IsTrue( testFloatVal >= -3.124f - MAX_ERR && testFloatVal <= -3.124f + MAX_ERR );

			testUIntVal = *( reinterpret_cast<const unsigned int*>( &testBuffer.get_all_data()[sizeof( float ) + sizeof( unsigned int )] ) );
			testFloatVal = *( reinterpret_cast<const float*>( &testBuffer.get_all_data()[sizeof( float ) + 2 * sizeof( unsigned int )] ) );

			Assert::AreEqual( static_cast<unsigned int>( 5 ), testUIntVal );
			Assert::IsTrue( testFloatVal >= 3.45f - MAX_ERR && testFloatVal <= 3.45f + MAX_ERR );
		}

		TEST_METHOD( interleaved_attr_buffer_insert_invalid_param_test )
		{
			testMap->reset( true );
			testMap->end_definition();

			interleaved_attr_buffer testBuffer( *testMap );
			std::vector<char> data( 4 );

			try {
				testBuffer.insert_values( data );
				
				// Test to see if an exception is thrown when trying to insert values to a buffer with no attributes in its attribute map
				Assert::Fail();
			} catch ( std::exception& ) {
			}

			testMap->reset( true );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->end_definition();

			data.resize( 5 );

			interleaved_attr_buffer testBuffer2( *testMap );

			try {
				testBuffer2.insert_values( data );

				// Test to see if an exception is thrown when an vector of invalid size is passed to the insert_values function
				Assert::Fail();
			} catch( std::exception& ) {
			}
		}

		TEST_METHOD( interleaved_attr_buffer_get_attribute_data_offsets_empty_buffer_test )
		{
			testMap->end_definition();

			interleaved_attr_buffer testBuffer( *testMap );

			// Test to make sure an empty buffer with a single attribute returns a vector with a single offset that is 0
			Assert::AreEqual( static_cast<std::size_t>( 1 ), testBuffer.get_attribute_data_offsets().size() );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );

			testMap->reset( true );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->add_attribute( attribute( "test2", 1, attrib_uint ) );
			testMap->end_definition();

			interleaved_attr_buffer testBuffer2( *testMap );

			// Test to make sure an empty buffer with two attributes returns a vector with two offsets of 0
			Assert::AreEqual( static_cast<std::size_t>( 2 ), testBuffer2.get_attribute_data_offsets().size() );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer2.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer2.get_attribute_data_offsets()[1] );
		}

		TEST_METHOD( interleaved_attr_buffer_get_attribute_data_offsets_test )
		{
			testMap->reset( true );
			testMap->add_attribute( attribute( "test", 1, attrib_float ) );
			testMap->add_attribute( attribute( "test2", 2, attrib_uint ) );
			testMap->add_attribute( attribute( "test3", 1, attrib_int ) );
			testMap->end_definition();

			interleaved_attr_buffer testBuffer( *testMap );
			std::vector<char> data( sizeof( float ) + 2 * sizeof( unsigned int ) + sizeof( int ) );

			testBuffer.insert_values( data );

			// Test to make sure the offsets adjusts properly after a single value is inserted into the buffer
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 4 ), testBuffer.get_attribute_data_offsets()[1] );
			Assert::AreEqual( static_cast<unsigned int>( 12 ), testBuffer.get_attribute_data_offsets()[2] );

			data.resize( data.size() * 2 );

			testBuffer.insert_values( data );

			// Test to make sure the offsets adjusts properly after two values are inserted into the buffer
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 4 ), testBuffer.get_attribute_data_offsets()[1] );
			Assert::AreEqual( static_cast<unsigned int>( 12 ), testBuffer.get_attribute_data_offsets()[2] );

			testBuffer.clear_buffer();

			// Test to make sure the offsets are 0 after the buffer has been cleared
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[1] );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[2] );

			testBuffer.insert_values( data );

			// Tests to make sure the offsets properly adjusts after the buffer has been cleared and values are inserted
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testBuffer.get_attribute_data_offsets()[0] );
			Assert::AreEqual( static_cast<unsigned int>( 4 ), testBuffer.get_attribute_data_offsets()[1] );
			Assert::AreEqual( static_cast<unsigned int>( 12 ), testBuffer.get_attribute_data_offsets()[2] );
		}
	};
}