#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>

#include <opengl/retained/gl_retained_object_manager.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace occluded::opengl::retained;

namespace OccludedLibraryUnitTests
{
	TEST_CLASS( gl_retained_object_manager_test )
	{
	public:
		
		TEST_METHOD_INITIALIZE( gl_retained_object_manager_test_init )
		{
			errorState = false;

			resetVAOIDs();
			resetVBOIDs();
		}

		TEST_METHOD_CLEANUP( gl_retained_object_manager_test_cleanup ) 
		{
			gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();
			errorState = false;

			manager.delete_objects();;
		}

		TEST_METHOD( gl_retained_object_manager_gen_new_vao_test )
		{
			GLuint testId = 0;

			gl_retained_object_manager& monitor = gl_retained_object_manager::get_manager();

			testId = monitor.get_new_vao();

			// Check to make sure the first id returned by gen_new_vao is not 0
			Assert::AreNotEqual( testId, static_cast<GLuint>( 0 ) );

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

		TEST_METHOD( gl_retained_object_manager_check_valid_vao_id_test )
		{
			GLuint testVaoId = 0;;

			gl_retained_object_manager& monitor = gl_retained_object_manager::get_manager();

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

			monitor.remove_ref_to_vao( testVaoId );

			// Check to make sure false is returned for a vaoId that has had its reference count to 0
			Assert::IsFalse( monitor.check_valid_vao_id( testVaoId ) );

			testVaoId = monitor.get_new_vao();

			monitor.delete_objects();

			// Test to make sure false is returned after a call to delete_objects is made
			Assert::IsFalse( monitor.check_valid_vao_id( testVaoId ) );
		}

		TEST_METHOD( gl_retained_object_manager_gen_new_vbo_test )
		{
			GLuint testVaoId = 0;
			GLuint testVboId = 0;

			gl_retained_object_manager& monitor = gl_retained_object_manager::get_manager();

			testVaoId = monitor.get_new_vao();
			testVboId = monitor.get_new_vbo( testVaoId );

			// Check to make sure the first id is returned by gen_new_vbo is not 0
			Assert::AreNotEqual( testVboId, static_cast<GLuint>( 0 ) );

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

		TEST_METHOD( gl_retaiend_object_manager_check_valid_vbo_id_test )
		{
			GLuint testVboId = 0;
			GLuint testVaoId = 0;

			gl_retained_object_manager& monitor = gl_retained_object_manager::get_manager();

			testVaoId = monitor.get_new_vao();
			testVboId = monitor.get_new_vbo( testVaoId );

			// Check to make sure a valid vao id and vbo id returns true when passed to check_valid_vbo_id
			Assert::IsTrue( monitor.check_valid_vbo_id( testVaoId, testVboId ) );

			// Check to make sure false is returned by check_valid_vbo_id when a valid vao id and an invalid vbo id are passed
			Assert::IsFalse( monitor.check_valid_vbo_id( testVaoId, static_cast<GLuint>( 42 ) ) );

			// Check to make sure false is returned by check_valid_vbo_id when an invalid vao id and a valid vbo id are passed
			Assert::IsFalse( monitor.check_valid_vbo_id( static_cast<GLuint>( 42 ), testVboId ) );

			// Check to make sure an exception is thrown if the vaoId parameter is 0
			try {
				monitor.check_valid_vbo_id( static_cast<GLuint>( 0 ), testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Check to make sure an exception is thrown if the vboId parameter is 0
			try {
				monitor.check_valid_vbo_id( testVaoId, static_cast<GLuint>( 0 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			monitor.remove_ref_to_vbo( testVaoId, testVboId );

			// Check to make sure false is returned by check_valid_vbo_id when the ref count is reduced to 0 for a vbo
			Assert::IsFalse( monitor.check_valid_vbo_id( testVaoId, testVboId ) );

			testVaoId = monitor.get_new_vao();
			testVboId = monitor.get_new_vbo( testVaoId );

			monitor.delete_objects();

			// Check to make sure false is returned by check_valid_vbo_id after all the objects are deleted
			Assert::IsFalse( monitor.check_valid_vbo_id( testVaoId, testVboId ) );
		}

		TEST_METHOD( gl_retained_object_manager_add_ref_to_vao_test )
		{
			GLuint testVaoId = 0;

			gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();

			testVaoId = manager.get_new_vao();

			// Test to make sure no exception is thrown if a valid vao id is passed to add_ref_to_vao
			try {
				manager.add_ref_to_vao( testVaoId );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			// Test to make sure an exception is thrown if 0 is passed to add_ref_to_vao as the vaoId
			try {
				manager.add_ref_to_vao( static_cast<GLuint>( 0 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if an invalid vao id is passed to add_ref_to_vao
			try {
				manager.add_ref_to_vao( static_cast<GLuint>( 893 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			manager.remove_ref_to_vao( testVaoId );
			manager.remove_ref_to_vao( testVaoId );

			// Test to make sure an exception is thrown if the reference count for a vaoId has been reduced to 0 and add_ref_to_vao is
			try {
				manager.add_ref_to_vao( testVaoId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testVaoId = manager.get_new_vao();
			
			manager.delete_objects();

			// Test to make sure an exception is thrown if the add_ref_to_vao called after a delete_objects call
			try {
				manager.add_ref_to_vao( testVaoId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_object_manager_remove_ref_to_vao_test )
		{
			GLuint testVaoId = 0;

			gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();

			testVaoId = manager.get_new_vao();

			// Test to make sure no exception is thrown if a valid vao is passed to 
			try {
				manager.remove_ref_to_vao( testVaoId );
			} catch ( const std::exception& ) {
				Assert::Fail();
			}

			// Test to make sure an exception is thrown if 0 is passed to remove_ref_to_vao as the vaoId
			try {
				manager.remove_ref_to_vao( 0 );
			
				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if an invalid vao id is passed as a parameter to remove_ref_to_vao
			try {
				manager.remove_ref_to_vao( static_cast<GLuint>( 985 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if a vao id is passed to remove_ref_to_vao that has had its ref count reduced
			// to 1
			try {
				manager.remove_ref_to_vao( testVaoId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testVaoId = manager.get_new_vao();

			manager.delete_objects();

			// Test to make sure an exception is thrown if a call to remove_ref_to_vao is made after delete_objects call
			try {
				manager.remove_ref_to_vao( testVaoId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_object_manager_add_ref_to_vbo_test )
		{
			GLuint testVaoId = 0;
			GLuint testVboId = 0;

			gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();

			testVaoId = manager.get_new_vao();
			testVboId = manager.get_new_vbo( testVaoId );

			// Test to make sure that no exception is thrown if an valid vao id and vbo id is passed as parameters
			try {
				manager.add_ref_to_vbo( testVaoId, testVboId );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			// Test to make sure that an exception is thrown if an invalid vbo id is passed as a parameters
			try {
				manager.add_ref_to_vbo( testVaoId, static_cast<GLuint>( 3245 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure that an exception is thrown if an invalid vao id is passed as a parameter
			try {
				manager.add_ref_to_vbo( static_cast<GLuint> ( 4325 ), testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if 0 is passed as the vao id
			try {
				manager.add_ref_to_vbo( static_cast<GLuint>( 0 ), testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if 0 is passed as the vbo id
			try {
				manager.add_ref_to_vbo( testVaoId, static_cast<GLuint>( 0 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			manager.remove_ref_to_vbo( testVaoId, testVboId );
			manager.remove_ref_to_vbo( testVaoId, testVboId );

			// Test make sure an exception is thrown if a reference is added to a vbo that has had its reference count reduced to 0
			try {
				manager.add_ref_to_vbo( testVaoId, testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testVaoId = manager.get_new_vao();
			testVboId = manager.get_new_vbo( testVaoId );

			manager.delete_objects();

			// Test to make sure an exception is thrown if add_ref_to_vbo is called after delete_objects is called
			try {
				manager.add_ref_to_vbo( testVaoId, testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}

		TEST_METHOD( gl_retained_object_manager_remove_ref_to_vbo_test )
		{
			GLuint testVaoId = 0;
			GLuint testVboId = 0;

			gl_retained_object_manager& manager = gl_retained_object_manager::get_manager();

			testVaoId = manager.get_new_vao();
			testVboId = manager.get_new_vbo( testVaoId );

			// Test to make sure an exception is thrown if an invalid vbo id is passed as a parameter
			try {
				manager.remove_ref_to_vbo( testVaoId, static_cast<GLuint>( 5467 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if an invalid vao id is passed as a parameter
			try {
				manager.remove_ref_to_vbo( static_cast<GLuint>( 6657 ), testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if 0 is passed as the vaoid parameter
			try {
				manager.remove_ref_to_vbo( static_cast<GLuint>( 0 ), testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is thrown if 0 is passed as the vboid parameter
			try {
				manager.remove_ref_to_vbo( testVaoId, static_cast<GLuint>( 0 ) );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			// Test to make sure an exception is not thrown if a valid vbo and vao are passed as parameters to remove_ref_to_vbo
			try {
				manager.remove_ref_to_vbo( testVaoId, testVboId );
			} catch( const std::exception& ) {
				Assert::Fail();
			}

			// Test to make sure an exception is thrown if a reference count to a valid vbo is already at 0 and a reference count is attempted to
			// be removed from it
			try {
				manager.remove_ref_to_vbo( testVaoId, testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}

			testVaoId = manager.get_new_vao();
			testVboId = manager.get_new_vbo( testVaoId );

			manager.delete_objects();

			// Test to make sure an exception is thrown if remove_ref_to_vbo is called after delete_objects is called
			try {
				manager.remove_ref_to_vbo( testVaoId, testVboId );

				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}
	};
}