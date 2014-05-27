#include "stdafx.h"
#include "CppUnitTest.h"

#include "buffers/attribute_buffer_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::buffers;
using namespace occluded::buffers::attributes;

namespace OccludedLibraryTest
{
	TEST_CLASS( attribute_buffer_factory_test )
	{
	public:
		
		TEST_METHOD( attribute_buffer_factory_correct_buffer_type_test )
		{
			attribute_map testMap( true );
			testMap.add_attribute( attribute( "test", 1, attrib_uint ) );
			testMap.end_definition();

			try {
				std::auto_ptr<attribute_buffer> testBuffer = attribute_buffer_factory::create_attribute_buffer( testMap );
				
				Assert::IsTrue( testBuffer->get_attribute_map().is_interleaved() );
			} catch( std::exception& ) {
				Assert::Fail();
			}

			testMap.reset( false );
			testMap.add_attribute( attribute( "test", 1, attrib_uint ) );
			testMap.end_definition();

			try {
				std::auto_ptr<attribute_buffer> testBuffer = attribute_buffer_factory::create_attribute_buffer( testMap );

				Assert::IsFalse( testBuffer->get_attribute_map().is_interleaved() );
			} catch( std::exception& ) {
				Assert::Fail();
			}
		}

	};
}