#include "stdafx.h"
#include "CppUnitTest.h"

#include <opengl/retained/gl_retained_state_monitor.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace occluded::opengl::retained;

namespace OccludedLibraryUnitTests
{
	TEST_CLASS( gl_retained_state_monitor_test )
	{
	public:
		
		TEST_METHOD_INITIALIZE( gl_retained_state_monitor_test_init )
		{
			errorState = false;

			resetVAOIDs();
			resetVBOIDs();
		}

		TEST_METHOD( gl_retained_state_monitor_gen_new_vao_test )
		{
			GLuint testId = 0;

			gl_retained_state_monitor& monitor = gl_retained_state_monitor::get_monitor();

			testId = monitor.get_new_vao();

			// Check to make sure the first id returned by gen_new_vao is 1
			Assert::AreEqual( testId, static_cast<GLuint>( 1 ) );

			// Check to make sure a new id is returned when gen_new_vao is called again
			Assert::AreNotEqual( testId, monitor.get_new_vao() );

			errorState = true;

			// Check to make sure an exception is thrown if OpenGL enters an error state
			try {
				monitor.get_new_vao();

				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_state_monitor_check_valid_vao_id_test )
		{
			GLuint testVaoId = 0;;

			gl_retained_state_monitor& monitor = gl_retained_state_monitor::get_monitor();

			testVaoId = monitor.get_new_vao();

			// Check to make sure check_valid_vao_id returns true if a valid vao id is passed to it
			Assert::IsTrue( monitor.check_valid_vao_id( testVaoId ) );

			// Check to make sure check_valid_vao_id returns false if a invalid vao id is passed to it
			Assert::IsFalse( monitor.check_valid_vao_id( static_cast<GLuint>( 42 ) ) );

			// Check to make sure an exception is thrown if 0 is passed as the parameter
			try {
				monitor.check_valid_vao_id( static_cast<GLuint>( 0 ) );

				Assert::Fail();
			} catch( const std::runtime_error& ){
			}

			// TODO: Need a test for when the ref count is reduced to 0
		}

		TEST_METHOD( gl_retained_state_monitor_gen_new_vbo_test )
		{
			GLuint testVaoId = 0;
			GLuint testVboId = 0;

			gl_retained_state_monitor& monitor = gl_retained_state_monitor::get_monitor();

			testVaoId = monitor.get_new_vao();
			testVboId = monitor.get_new_vbo( testVaoId );

			// Check to make sure the first id is returned by gen_new_vbo is 1
			Assert::AreEqual( testVboId, static_cast<GLuint>( 1 ) );

			// Check to make sure a new id is returned when gen_new_vao is called again
			Assert::AreNotEqual( testVboId, monitor.get_new_vbo( testVaoId ) );

			errorState = true;

			// Check to make sure an exception is thrown if OpenGL enters an error state
			try {
				monitor.get_new_vbo( testVaoId );
			
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}
	};
}