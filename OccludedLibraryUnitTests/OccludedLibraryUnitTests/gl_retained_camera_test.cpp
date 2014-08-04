#include "stdafx.h"
#include "CppUnitTest.h"

#include <opengl/scene/objects/fixed_gl_camera.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::scene::objects;
using namespace occluded::opengl::retained::shaders;

namespace OccludedLibraryUnitTests
{
	static std::vector< const boost::shared_ptr<const shader> > shaders;
	static glm::mat4 testProj;
	static glm::mat4 testView;

	TEST_CLASS( gl_retained_camera_test )
	{
	public:
		TEST_CLASS_INITIALIZE( gl_retained_camera_init )
		{
			std::string src( "Not Empty" );

			shaders.push_back( boost::shared_ptr<shader>( new shader( src, vert_shader ) ) );
			shaders.push_back( boost::shared_ptr<shader>( new shader( src, frag_shader ) ) );

			testProj = glm::perspective( 60.0f, 4.0f/3.0f, 1.0f, 5.0f );
			testView = glm::mat4( 1 );
		}

		TEST_METHOD( gl_retained_camera_get_shader_program_test )
		{
			shader_program testProg( shaders );
			shader_program testProg2( shaders );

			std::auto_ptr<gl_retained_camera> testCamera( new fixed_gl_camera( testProg, testProj, testView ) );

			// Test to make sure the correct shader_program is returned by get_shader_program
			Assert::AreEqual( testProg.get_id(), testCamera->get_shader_program().get_id() );

			testCamera->set_shader_program( testProg2 );

			// Test to make sure the correct shader_program is returned by get_shader_program after set_shader_program has been called
			Assert::AreEqual( testProg2.get_id(), testCamera->get_shader_program().get_id() );
		}

	};
}