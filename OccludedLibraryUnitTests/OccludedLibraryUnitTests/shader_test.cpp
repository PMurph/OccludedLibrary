#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained::shaders;

bool errorState = false;

/**
 * NOTE: This is not a complete verification of the function of shader class since it relies heavily on how 
 * OpenGL functions.
 */
namespace OccludedLibraryUnitTests
{
	TEST_CLASS( shader_test )
	{
	public:
		TEST_METHOD( shader_is_compiled_test )
		{
			std::string src( "Not Empty" );
			std::auto_ptr<shader> testShader;
			
			try {
				errorState = false;
				testShader.reset( new shader( src, vert_shader ) );

				// Check to make sure it compiles properly
				Assert::IsTrue( testShader->is_compiled() );
			} catch( std::exception& ) {
				Assert::Fail();
			}

			try {
				errorState = true;

				testShader.reset( new shader( src, geo_shader ) );

				// Check to make sure it "fails" to compile
				Assert::IsFalse( testShader->is_compiled() );
			} catch( std::exception& ) {
				Assert::Fail();
			}

			src = "";

			try {
				errorState = true;
				testShader.reset( new shader( src, frag_shader ) );

				// Test to make sure it throws an exception when an empty string is passed as the shader source
				Assert::Fail();
			} catch( std::exception& ) {
			}
		}

		TEST_METHOD( shader_get_id_test )
		{
			std::string src( "Not Empty" );
			std::auto_ptr<shader> testShader;

			try {
				errorState = false;
				testShader.reset( new shader( src, vert_shader ) );

				// Check to make sure the id is correct when the shader compiles properly
				Assert::AreEqual( testShader->get_id(), static_cast<unsigned int>( 1 ) );
			} catch( std::exception& ) {
				Assert::Fail();
			}

			try {
				errorState = true;
				testShader.reset( new shader( src, tess_eval_shader ) );

				// If it fails to compile, an exception should be thrown when trying to get the id
				testShader->get_id();
				Assert::Fail();
			} catch( std::exception& ) {
				
			}
		}

		TEST_METHOD( shader_get_type_test )
		{
			std::auto_ptr<shader> testShader;
			std::string src( "Not Empty" );

			try {
				errorState = false;
				testShader.reset( new shader( src, vert_shader ) );

				// Check to make sure the vertex_shader is returned, when properly compiled
				Assert::IsTrue( testShader->get_type() == vert_shader );
			} catch( std::exception& ) {
				Assert::Fail();
			}

			try {
				errorState = true;
				testShader.reset( new shader( src, tess_control_shader ) );

				// Check to make sure that an exception is thrown, when compiling fails
				testShader->get_type();
				Assert::Fail();
			} catch( std::exception& ) {
			}
		}

		TEST_METHOD( shader_get_compile_log_test )
		{
			std::auto_ptr<shader> testShader;
			std::string src( "Not Empty" );

			try {
				errorState = false;
				testShader.reset( new shader( src, compute_shader ) );

				// Check to make sure an exception is thrown if the shader is properly compiled
				testShader->get_compile_log();
				Assert::Fail();
			} catch( std::exception& ) {
			}

			try {
				errorState = true;
				testShader.reset( new shader( src, vert_shader ) );

				// Check to make sure no exception is thrown if the shader does not properly compiled
				Assert::IsTrue( "" == testShader->get_compile_log() );
			} catch( std::exception& ) {
				Assert::Fail();
			}
		}
	};
}