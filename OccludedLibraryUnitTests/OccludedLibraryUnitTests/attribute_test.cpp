#include "stdafx.h"
#include "CppUnitTest.h"

#include <buffers/attributes/attribute.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryUnitTests
{
	TEST_CLASS( attribute_test )
	{
	public:
		
		TEST_METHOD( attibute_name_test )
		{
			attribute attrib( "test", 1, attrib_float );
			Assert::AreEqual( std::string("test"), attrib.get_name() );
		}

		TEST_METHOD( attribute_arity_test )
		{
			attribute attrib( "test", 1, attrib_float );
			Assert::AreEqual( static_cast<unsigned int>( 1 ), attrib.get_arity() );

			attrib = attribute( "test", 2, attrib_float );
			Assert::AreEqual( static_cast<unsigned int>( 2 ), attrib.get_arity() );
		}

		TEST_METHOD( attribute_type_test )
		{
			attribute attrib( "test", 1, attrib_float );
			Assert::IsTrue( attrib_float == attrib.get_type() );

			attrib = attribute( "test", 1, attrib_uint );
			Assert::IsTrue( attrib_uint == attrib.get_type() );
		}

		TEST_METHOD( attribute_comp_size_test )
		{
			attribute attrib( "test", 1, attrib_float );
			Assert::AreEqual( static_cast<std::size_t>( 4 ), attrib.get_component_size() );

			attrib = attribute( "test", 1, attrib_uint );
			Assert::AreEqual( static_cast<std::size_t>( 4 ), attrib.get_component_size() );
		}

		TEST_METHOD( attribute_attrib_size_test )
		{
			attribute attrib( "test", 1, attrib_float );
			Assert::AreEqual( static_cast<std::size_t>( 4 ), attrib.get_attrib_size() );

			attrib = attribute( "test", 2, attrib_float );
			Assert::AreEqual( static_cast<std::size_t>( 8 ), attrib.get_attrib_size() );
		}

		TEST_METHOD( attribute_is_normalized_test )
		{
			attribute testAttrib( "test", 1, attrib_float );
			// Test to make sure the default is false
			Assert::IsFalse( testAttrib.is_normalized() );

			testAttrib = attribute( "test2", 2, attrib_uint, true );
			// Test to make sure setting it to true
			Assert::IsTrue( testAttrib.is_normalized() );
		}
	};
}