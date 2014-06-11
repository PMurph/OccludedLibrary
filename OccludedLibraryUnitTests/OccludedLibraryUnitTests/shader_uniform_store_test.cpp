#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader_uniform_store.h"

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

		TEST_METHOD( shader_uniform_store_empty_constructor_test )
		{
			std::auto_ptr<shader_program> testProgram;

			errorState = true;
			testProgram.reset( new shader_program( shaders ) );

			try {
				shader_uniform_store testStore( *testProgram ); 

				// Test to make sure an exception is throw if a non-linked shader program is passed to the constructor
				Assert::Fail();
			} catch( const std::exception& ) {
				
			}

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			try {
				shader_uniform_store testStore( *testProgram );
				// Test to make sure no exception is throw is a linked shader program is passed to the constructor
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			
		}

		TEST_METHOD( shader_uniform_store_populate_constructor_test )
		{
			std::auto_ptr<shader_program> testProgram;
			std::vector< std::pair<const std::string, const uniform_value> > testValues;
			testValues.push_back( std::pair<const std::string, const uniform_value>( "test", glm::mat4( 1 ) ) );

			errorState = true;
			testProgram.reset( new shader_program( shaders ) );

			try {
				shader_uniform_store testStore( *testProgram, testValues );
				// Test to make sure an exception is thrown, if the init and populate constructor is used with a non-linked shader program
				Assert::Fail();
			} catch( const std::exception& ) {
			
			}

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			try {
				shader_uniform_store testStore( *testProgram, testValues );
				// Test to make sure no exception is thrown if a linked shader program and a proper vector is passed to the constructor
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			testValues.push_back( std::pair<const std::string, const uniform_value>( "test", glm::vec3(1) ) );

			try {
				shader_uniform_store testStore( *testProgram, testValues );
				// Test to make sure an exception is thrown if the populate constuctor is passed a vector with two uniforms with the same name
				Assert::Fail();
			} catch( const std::exception& ) {
				
			}
		}

		TEST_METHOD( shader_uniform_store_add_uniform_test )
		{
			std::auto_ptr<shader_program> testProgram;

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			shader_uniform_store testStore( *testProgram );

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
			std::auto_ptr<shader_program> testProgram;

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			shader_uniform_store testStore( *testProgram );

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
			std::auto_ptr<shader_program> testProgram;
			glm::vec3 testVal( 1 );

			errorState = false;
			testProgram.reset( new shader_program( shaders ) );

			shader_uniform_store testStore( *testProgram );
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


			std::vector< std::pair<const std::string, const uniform_value> > values;
			values.push_back( std::pair<const std::string, const uniform_value>( "test", testVal ) );
			shader_uniform_store testStore2( *testProgram, values );
			
			const glm::vec3& returnedVal = testStore.get_value<glm::vec3>( "test" );

			// Test to make sure that the correct value is returned after the uniform store is populated by the populate constructor
			Assert::IsTrue( testVal == returnedVal );
		}
	};
}