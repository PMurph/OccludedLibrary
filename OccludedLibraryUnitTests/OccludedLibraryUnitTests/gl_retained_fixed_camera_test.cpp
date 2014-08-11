#include "stdafx.h"
#include "CppUnitTest.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <opengl/retained/scene/objects/gl_retained_fixed_camera.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace occluded::opengl::retained::scene::objects;
using namespace occluded::opengl::retained::shaders;

namespace OccludedLibraryUnitTests
{
	static std::vector< const boost::shared_ptr<const shader> > shaders;
	static glm::mat4 testProj;
	static glm::mat4 testView;

	TEST_CLASS( gl_retained_fixed_camera_test )
	{
	public:
		TEST_CLASS_INITIALIZE( gl_retained_fixed_camera_init )
		{
			std::string src( "Not Empty" );

			shaders.push_back( boost::shared_ptr<shader>( new shader( src, vert_shader ) ) );
			shaders.push_back( boost::shared_ptr<shader>( new shader( src, frag_shader ) ) );

			testProj = glm::perspective( 1.047198f, 4.0f/3.0f, 1.0f, 5.0f );
			testView = glm::mat4( 1 );
		}
		
		TEST_METHOD( gl_retained_fixed_camera_get_projection_test )
		{
			shader_program testProgram( shaders );
			gl_retained_fixed_camera testCamera( testProgram, testProj, testView );

			// Test to make sure the correct matrix is returned by the get_projection function
			Assert::IsTrue( testProj == testCamera.get_projection().get_raw_transformation() );

			gl_retained_fixed_camera testCamera2( testProgram, glm::mat4( 1 ), glm::mat4( 1 ) );

			// Test to make sure a different matrix is returned by the get_projection if the matrix passed to the constructor is changed
			Assert::IsFalse( testProj == testCamera2.get_projection().get_raw_transformation() );
		}

		TEST_METHOD( gl_retained_fixed_camera_get_view_test )
		{
			shader_program testProgram( shaders );
			gl_retained_fixed_camera testCamera( testProgram, testProj, testView );

			// Test to make sure the correct matrix is returned by the get_view function
			Assert::IsTrue( testView == testCamera.get_view().get_raw_transformation() );

			gl_retained_fixed_camera testCamera2( testProgram, glm::mat4( 0 ), glm::mat4( 0 ) );

			// Test to make sure a different matrix is returned by get_view if the matrix passed to the constructor is changed
			Assert::IsFalse( testView == testCamera2.get_view().get_raw_transformation() );
		}

		TEST_METHOD( gl_retained_fixed_camera_set_for_render_test )
		{
			shader_program testProgram( shaders );
			gl_retained_fixed_camera testCamera( testProgram, testProj, testView );

			try {
				testCamera.set_for_render();
			} catch( const std::exception& ) {
				// Test to make sure no exception is thrown by set_for_render when the shader_program is properly linked
				Assert::Fail();
			}

			shader_program testProgram2;
			gl_retained_fixed_camera testCamera2( testProgram2, testProj, testView );

			// Test to make sure an exception is thrown by set_for_render
			try {
				testCamera2.set_for_render();

				// Test to make sure an exception is thrown by set_to_render when the shader_program is properly linked
				Assert::Fail();
			} catch( const std::exception& ) {
			}
		}
	};
}