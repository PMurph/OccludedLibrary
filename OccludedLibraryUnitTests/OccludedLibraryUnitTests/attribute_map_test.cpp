#include "stdafx.h"
#include "CppUnitTest.h"

#include <buffers/attributes/attribute_map.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryUnitTests
{
	TEST_CLASS( attribute_map_test )
	{
	public:

		TEST_METHOD( attribute_map_being_defined_test )
		{
			attribute_map testMap( false );

			// Test to see if true is returned when the attribute map is created
			Assert::IsTrue( testMap.being_defined() );

			// Test to see if false is returned when the attribute map's definition has been ended
			testMap.end_definition();
			Assert::IsFalse( testMap.being_defined() );

			// Test to see if true after reset is called on the attribute map
			testMap.reset( false );
			Assert::IsTrue( testMap.being_defined() );

			// Test to see if true after attribute is added to the map
			testMap.add_attribute( attribute( "test", 1, attrib_int ) );
			Assert::IsTrue( testMap.being_defined() );
		}

		TEST_METHOD( attribute_map_get_byte_size_test )
		{
			attribute_map testMap( false );
			testMap.end_definition();

			// Test to make sure an empty attribute map returns a byte size of 0
			Assert::AreEqual( static_cast<std::size_t>( 0 ), testMap.get_byte_size() );

			testMap.reset( false );
			testMap.add_attribute( attribute( "testAttrib1", 1, attrib_float ) );
			testMap.end_definition();

			Assert::AreEqual( sizeof( float ), testMap.get_byte_size() );

			testMap.reset( false );
			testMap.add_attribute( attribute( "testAttrib1", 1, attrib_float ) );
			testMap.add_attribute( attribute( "testAttrib2", 2, attrib_uint ) );
			testMap.end_definition();

			Assert::AreEqual( sizeof( float ) + 2 * sizeof( unsigned int ), testMap.get_byte_size() );

		}
		
		TEST_METHOD( attribute_map_attrib_count_test )
		{
			attribute_map testMap( false );
			attribute buffAttrib( "test", 1, attrib_float);
			attribute testAttrib( "another_test", 2, attrib_int );

			// Test to see if attribute count is initially 0
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testMap.get_attrib_count() );
			
			// Test to see if attribute count increases when attribute is added
			testMap.add_attribute( buffAttrib );
			Assert::AreEqual( static_cast<unsigned int>( 1 ), testMap.get_attrib_count() );

			// Test to see if attribute count is properly reset when reset is called
			testMap.reset( true );
			Assert::AreEqual( static_cast<unsigned int>( 0 ), testMap.get_attrib_count() );

			// Test to see if attribute count 
			testMap.add_attribute( testAttrib );
			Assert::AreEqual( static_cast<unsigned int>( 1 ), testMap.get_attrib_count() );
		}

		TEST_METHOD( attribute_map_get_attributes_test )
		{
			attribute_map testMap( false );
			attribute buffAttrib( "test", 1, attrib_float );
			attribute testAttrib( "another_test", 3, attrib_uint );

			try {
				// Test to see if an exception in thrown when get_attributes is called 
				Assert::AreEqual( static_cast<std::size_t>( 0 ), testMap.get_attributes().size() );
				Assert::Fail();
			} catch ( std::exception& ) {

			}

			try {
				testMap.end_definition();
				
				// Test to make sure attribute vector is empty when no attributes are defined and to make sure no exception is thrown when map definition
				// is complete.
				Assert::AreEqual( static_cast<std::size_t>( 0 ), testMap.get_attributes().size() );
			} catch ( std::exception& ) {
				Assert::Fail();
			}

			try {
				testMap.reset( false );

				// Test to see if exception is thrown when trying to retrieve the attributes after reset is called
				Assert::AreEqual( static_cast<std::size_t>( 0 ), testMap.get_attributes().size() );
				Assert::Fail();
			} catch ( std::exception& ) {
				
			}

			try {
				testMap.add_attribute( buffAttrib );
				testMap.end_definition();

				// Test to see if get attributes returns the attribute in a properly defined attribute map
				Assert::AreEqual( static_cast<std::size_t>( testMap.get_attrib_count() ), testMap.get_attributes().size() );
				Assert::AreEqual( std::string( "test" ), testMap.get_attributes()[0].get_name() );
			} catch ( std::exception& ) {
				Assert::Fail();
			}

			// Test to see if only the attributes that are defined since the last reset are retrieved with get attribute
			try	{ 
				testMap.reset( false );
				testMap.add_attribute( testAttrib );
				testMap.end_definition();

				Assert::AreEqual( static_cast<std::size_t>( 1 ), testMap.get_attributes().size() );
				Assert::AreEqual( std::string( "another_test" ), testMap.get_attributes()[0].get_name() );
			} catch ( std::exception& ) {
				Assert::Fail();
			}
		}

		TEST_METHOD( attribute_map_add_attribute_test )
		{
			attribute_map testMap( false );
			attribute buffAttrib( "test", 1, attrib_float );
			attribute testAttribTwo( "testtwo", 2, attrib_int );

			try {
				// Test to see if add attribute doesn't throw an exception when adding an attribute after initially creating the map
				testMap.add_attribute( buffAttrib );
				testMap.reset( false );
			} catch ( std::exception& ) {
				Assert::Fail();
			}

			try {
				// Test to see if add attribute only leads to a single attribute in the attribute map after the map has been reset
				testMap.add_attribute( testAttribTwo );
				Assert::AreEqual( static_cast<unsigned int>( 1 ), testMap.get_attrib_count() );
				
				testMap.end_definition();
				Assert::AreEqual( static_cast<std::size_t>( 1 ), testMap.get_attributes().size() );
			} catch ( std::exception& ) {
				Assert::Fail();
			}

			try {
				// Test to see if adding two of the same attribute only adds it once to the attribute map
				testMap.reset( false );
				testMap.add_attribute( buffAttrib );
				testMap.add_attribute( buffAttrib );
				Assert::Fail();
			} catch ( std::exception& ) {
				testMap.end_definition();

				Assert::AreEqual( static_cast<unsigned int>( 1 ), testMap.get_attrib_count() );
			}

			try {
				// Test to see if an exception is thrown if an attribute is added when definition is finished
				testMap.add_attribute( testAttribTwo );

				Assert::Fail();
			} catch( std::exception& ) {
			}
		}

		TEST_METHOD( attribute_map_is_interleaved_test )
		{
			attribute_map testMap( true );

			// Test to see if is interleaved true when the constructor is passed true
			Assert::IsTrue( testMap.is_interleaved() );

			// Test to see if a reset call with an alternate value for the parameter
			testMap.reset( false );
			Assert::IsFalse( testMap.is_interleaved() );
		}
	};
}