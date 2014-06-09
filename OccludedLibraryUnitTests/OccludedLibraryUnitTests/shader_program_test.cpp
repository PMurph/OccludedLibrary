#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader_program.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained::shaders;

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
		TEST_CLASS_INITIALIZE( shader_program_init_test )
		{
			errorState = false;
			std::string src( "Not Empty" );

			m_shaders.push_back( boost::shared_ptr<const shader>( new shader( src, vert_shader ) ) );
			m_shaders.push_back( boost::shared_ptr<const shader>( new shader( src, frag_shader ) ) );
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
				errorState = true;
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
				Assert::AreEqual( testProgram->get_id(), static_cast<unsigned int>( 1 ) );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			errorState = true;
			testProgram.reset( new shader_program( m_shaders ) );

			try {
				// Test to see if get_id returns throws an exception if the shader program is not properly linked
				testProgram->get_id();
				Assert::Fail();
			} catch( const std::exception& ) {

			}
		}

		TEST_METHOD( shader_program_get_compile_log_test )
		{
			std::auto_ptr<shader_program> testProgram;

			try {
				errorState = true;
				testProgram.reset( new shader_program( m_shaders ) );

				// Test to see if get_compile_log doesn't throw an exception when the shader program is not properly linked
				Assert::IsTrue( "" == testProgram->get_error_log() );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			errorState = false;
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