#include "stdafx.h"
#include "CppUnitTest.h"

#include "opengl/retained/shaders/shader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace occluded::opengl::retained::shaders;

namespace OccludedLibraryUnitTests
{
	TEST_CLASS(shader_test)
	{
	public:
		
		TEST_METHOD( shader_is_compiled_test )
		{
			std::string src( "Not Empty" );
			
			try {
				shader testShader( src, vert_shader );

				Assert::IsTrue( testShader.is_compiled() );
			} catch( std::exception& ) {
				Assert::Fail();
			}

		}

	};
}