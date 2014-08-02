#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader_program.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained::shaders;

bool programLinkError = false;

/**
 * NOTE: This is not a complete verification of the function of shader class since it relies heavily on how 
 * OpenGL functions.
 */
namespace OccludedLibraryUnitTests
{
	static std::vector< const boost::shared_ptr<const shader> > m_shaders;

	TEST_CLASS( shader_program_test )
	{
	public:
		TEST_CLASS_INITIALIZE( shader_program_test_int )
		{
			errorState = false;
			std::string src( "Not Empty" );

			m_shaders.push_back( boost::shared_ptr<const shader>( new shader( src, vert_shader ) ) );
			m_shaders.push_back( boost::shared_ptr<const shader>( new shader( src, frag_shader ) ) );
		}

		TEST_METHOD_INITIALIZE( shader_program_test_method_init )
		{
			errorState = false;
			programLinkError = false;
		}

		TEST_METHOD( shader_program_copy_constructor_test )
		{
			errorState = false;
			shader_program testProg( m_shaders );
			std::auto_ptr<shader_program> testProgCopy( new shader_program( testProg ) );

			// Test to make sure the copyied program has the same id
			Assert::AreEqual( testProg.get_id(), testProgCopy->get_id() );

			// Test to make sure the copy of the program has a copy of the uniform store that does not throw exceptions
			shader_uniform_store& testStore = testProgCopy->get_uniform_store();
			testStore.add_uniform( "test", glm::vec3( 1 ) );

			shader_program testProg2;

			try {
				testProgCopy.reset( new shader_program( testProg2 ) );

				// Test to make sure an exception is thrown if the a place holder shader_program is copied
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( shader_program_initialization_test )
		{
			std::auto_ptr<shader_program> testProgram;
			std::string src( "Not Empty" );
			std::vector< const boost::shared_ptr<const shader> > shaders;

			shaders.push_back( boost::shared_ptr<const shader>( new shader( src, tess_eval_shader ) ) );

			try {
				errorState = false;
				testProgram.reset( new shader_program( shaders ) );

				// Test to make sure that an exception is thrown if there are not enough shaders
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			shaders.push_back( boost::shared_ptr<const shader>( new shader( src, tess_control_shader ) ) );

			try {
				errorState = false;
				testProgram.reset( new shader_program( shaders ) );

				// Test to make sure that an exception is thrown if the incorrect shaders are passed
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}
		
		TEST_METHOD( shader_program_is_linked_test )
		{
			std::auto_ptr<shader_program> testProgram;

			try {
				errorState = false;
				testProgram.reset( new shader_program( m_shaders ) );

				// Test to see if is_linked returns true when called after shader program is properly linked
				Assert::IsTrue( testProgram->is_linked() );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			try {
				programLinkError = true;
				testProgram.reset( new shader_program( m_shaders ) );

				// Test to see if is is_linked returns false when called after the shader program is not linked properly
				Assert::IsFalse( testProgram->is_linked() );
			} catch( const std::exception& ) {
				Assert::Fail();
			}
		}

		TEST_METHOD( shader_program_get_id_test )
		{
			std::auto_ptr<shader_program> testProgram;

			try {
				errorState = false;
				testProgram.reset( new shader_program( m_shaders ) );

				// Test to see if get_id returns the correct id if the shader program is properly linked
				Assert::AreNotEqual( testProgram->get_id(), static_cast<unsigned int>( 0 ) );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			programLinkError = true;
			testProgram.reset( new shader_program( m_shaders ) );

			try {
				// Test to see if get_id returns throws an exception if the shader program is not properly linked
				testProgram->get_id();
				Assert::Fail();
			} catch( const std::exception& ) {

			}
		}

		TEST_METHOD( shader_program_get_uniform_store_test )
		{
			std::auto_ptr<shader_program> testProgram( new shader_program );

			try {
				testProgram->get_uniform_store();

				// Test to make sure an exception is thrown if the shader_program is not properly linked
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testProgram.reset( new shader_program( m_shaders ) );

			try {
				testProgram->get_uniform_store();
			} catch( const std::exception& ) {
				// Test to make sure that an exception is thrown if the shader_program is properly linked
				Assert::Fail();
			}

			shader_uniform_store& testStore = testProgram->get_uniform_store();

			// Test to make sure that the shader program id is being set in the uniform store. Done by making sure no assertion is triggered in the next call.
			testStore.add_uniform( "test", glm::vec3( 0 ) );
		}

		TEST_METHOD( shader_program_get_compile_log_test )
		{
			std::auto_ptr<shader_program> testProgram;

			try {
				programLinkError = true;
				testProgram.reset( new shader_program( m_shaders ) );

				// Test to see if get_compile_log doesn't throw an exception when the shader program is not properly linked
				Assert::IsTrue( "" == testProgram->get_error_log() );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			programLinkError = false;
			testProgram.reset( new shader_program( m_shaders ) );

			try {
				// Make sure an exception is throw when get_compile_log is called on a shader program that has been properly linked
				testProgram->get_error_log();
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}
	};
}