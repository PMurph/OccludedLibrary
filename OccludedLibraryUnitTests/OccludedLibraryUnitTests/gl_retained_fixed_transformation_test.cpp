#include "stdafx.h"
#include "CppUnitTest.h"

#include <opengl/retained/scene/nodes/gl_retained_fixed_transformation.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace occluded::opengl::retained::scene::nodes;

namespace OccludedLibraryUnitTests
{
	TEST_CLASS ( gl_retained_fixed_transformation_test )
	{
	public:
		
		TEST_METHOD( gl_retained_fixed_transformation_constructor_test )
		{
			gl_retained_fixed_transformation testTransformation;

			// Test to make sure the default constructor sets the 4x4 matrix to the identity
			Assert::IsTrue( testTransformation.get_raw_transformation() == glm::mat4( 1 ) );

			glm::mat4 testMat( 1.f, 2.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f );
			gl_retained_fixed_transformation testTransformation2( testMat );

			// Test to make sure the set transform constructor applies the correct matrx
			Assert::IsTrue( testTransformation2.get_raw_transformation() == testMat );
		}

		TEST_METHOD ( gl_retained_fixed_transformation_get_raw_transformation_test )
		{
			gl_retained_fixed_transformation testTransformation;
			glm::mat4 testMat( 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, .5f, .5f, .5f, .5f );

			testTransformation.set_transformation( gl_retained_fixed_transformation( testMat ) );

			// Test to make sure the set_transformation changes the transformation contained
			Assert::IsTrue( testTransformation.get_raw_transformation() == testMat );
		}
	};
}