#include "gl_retained_fixed_transformation.h"

namespace occluded { namespace opengl { namespace retained { namespace scene { namespace nodes {

gl_retained_fixed_transformation::gl_retained_fixed_transformation():
	m_transform( glm::mat4( 1 ) )
{
}

gl_retained_fixed_transformation::gl_retained_fixed_transformation( const glm::mat4& transform ):
	m_transform( transform )
{
}

gl_retained_fixed_transformation::~gl_retained_fixed_transformation()
{
}

void gl_retained_fixed_transformation::set_transformation( const occluded::transformation& new_transform ) {
	m_transform = new_transform.get_raw_transformation();
}

const glm::mat4& gl_retained_fixed_transformation::get_raw_transformation() const {
	return m_transform;
}

} // end of nodes namespace
} // end of scene namespace 
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace