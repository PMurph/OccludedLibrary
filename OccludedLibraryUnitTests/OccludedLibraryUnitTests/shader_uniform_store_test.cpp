#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader_program.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained::shaders;

namespace OccludedLibraryUnitTests
{
	static std::vector< const boost::shared_ptr<const shader> > shaders;

	TEST_CLASS( shader_uniform_store_test )
	{
	public:
		TEST_CLASS_INITIALIZE( shader_uniform_store_init )
		{
			errorState = false;

			shaders.push_back( boost::shared_ptr<const shader>( new shader( "Not Empty", vert_shader ) ) );
			shaders.push_back( boost::shared_ptr<const shader>( new shader( "Not Empty", frag_shader ) ) );
		}

		TEST_METHOD( shader_uniform_store_add_uniform_test )
		{
			boost::shared_ptr<shader_program> testProgram;

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			shader_uniform_store& testStore = testProgram->get_uniform_store();

			testStore.add_uniform( "test", glm::mat4( 1 ) );

			try {
				testStore.add_uniform( "test", glm::mat4( 1 ) );

				// Tests to make sure that an exception if thrown when a uniform is added that has the same name as uniform already
				// in the store
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			try {
				testStore.add_uniform( "", glm::vec3( 1 ) );
				
				// Test to make sure an exception is thrown when a uniform with an empty name is passed
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( shader_uniform_store_set_uniform_value_test )
		{
			boost::shared_ptr<shader_program> testProgram;

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			shader_uniform_store& testStore = testProgram->get_uniform_store();

			try {
				testStore.set_uniform_value( "test", glm::mat4( 1 ) );

				// Test to make sure an exception is thrown if the uniform does not exist in the store
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testStore.add_uniform( "test", glm::vec3( 1 ) );

			try {
				testStore.set_uniform_value( "test", glm::mat4( 1 ) );

				// Test to make sure an exception is thrown if the type of the uniform value passed does not match the type
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( shader_uniform_store_get_value_test ) 
		{
			boost::shared_ptr<shader_program> testProgram;
			glm::vec3 testVal( 1 );

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			shader_uniform_store& testStore = testProgram->get_uniform_store();

			testStore.add_uniform( "test", testVal );

			try {
				const glm::vec3& returnedVal = testStore.get_value<glm::vec3>( "test" );
				
				// Test to make sure the initial value is returned when getting the stored value
				Assert::IsTrue( testVal == returnedVal );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			try {
				const glm::mat4& returnedVal = testStore.get_value<glm::mat4>( "test" );

				// Test to make sure an exception is thrown if the uniform value is of the wrong type
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			try {
				const glm::vec3& returnedVal = testStore.get_value<glm::vec3>( "test2" );

				// Test to make sure an exception is thrown if the uniform doesn't exist
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testVal = glm::vec3( 1.f, 2.f, 3.f );
			testStore.set_uniform_value( "test", testVal );

			try {
				const glm::vec3& returnedVal = testStore.get_value<glm::vec3>( "test" );

				// Test to make sure the the correct value is returned after the uniform has been updated
				Assert::IsTrue( testVal == returnedVal );
			} catch( const std::exception& ) {
				Assert::Fail();
			}
		}

		TEST_METHOD( shader_uniform_store_has_value_test )
		{
			shader_program testProg( shaders );
			shader_uniform_store& store = testProg.get_uniform_store();

			// Test to make sure that if there are no values in the store, false is returned by the has_uniform function
			Assert::IsFalse( store.has_uniform( "test") );

			store.add_uniform( "test", glm::mat4( 1 ) );

			// Test to make sure that if the uniform is in the store, true is returned by the has_uniform function
			Assert::IsTrue( store.has_uniform( "test" ) );

			// Test to make sure that if the uniform is not in the store but the the store is not empty, false is returned by the has_uniform function
			Assert::IsFalse( store.has_uniform( "test2" ) );
		}
	};
}