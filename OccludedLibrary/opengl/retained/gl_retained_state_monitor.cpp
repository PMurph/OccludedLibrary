#include "gl_retained_state_monitor.h"

namespace occluded { namespace opengl { namespace retained {

// Public Member Functions

const GLuint gl_retained_state_monitor::get_new_vao() {
	GLuint vaoId = 0;

	glGenVertexArrays( 1, &vaoId );

	if( vaoId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_state_monitor.get_new_vao: Faieled to create an new vertex array object because of an error with OpenGL" );

	inc_vao_entry( vaoId );

	return vaoId;
}

const bool gl_retained_state_monitor::check_valid_vao_id( const GLuint vaoId ) const {
	bool isValid = false;

	if( vaoId == 0 )
		throw std::runtime_error( "gl_retained_state_monitor.check_valid_vao_id: The invalid vao id 0 was passed as the parameter." );

	if( m_vaoCount.find( vaoId ) != m_vaoCount.end() )
		isValid = true;

	return isValid;
}


const GLuint gl_retained_state_monitor::get_new_vbo( const GLuint vaoId ) {
	GLuint vboId = 0;

	glBindVertexArray( vaoId );


	return 0;
}

// Private Member Functions

gl_retained_state_monitor::gl_retained_state_monitor()
{
}


gl_retained_state_monitor::~gl_retained_state_monitor()
{
}

void gl_retained_state_monitor::inc_vao_entry( const GLuint vaoId ) {
	assert( vaoId != 0 );

	if( vaoId != 0 ) {
		if( m_vaoCount.find( vaoId ) == m_vaoCount.end() )
			m_vaoCount.insert( std::pair<const GLuint, unsigned int>( vaoId, 1 ) );
		else
			m_vaoCount[vaoId] += 1;
	}
}

void gl_retained_state_monitor::dec_vao_entry( const GLuint vaoId ) {
	assert( vaoId != 0 && m_vaoCount.find( vaoId ) != m_vaoCount.end() );

	if( vaoId != 0 && m_vaoCount.find( vaoId ) != m_vaoCount.end() ) {
		if( m_vaoCount[vaoId] == 0 ) {
			throw std::runtime_error( "gl_retained_state_monitor.dec_vao_entry: Failed to decrement the ref count for vao id(" + 
				boost::lexical_cast<std::string>( vaoId ) + ") is already 0." );
		}

		m_vaoCount[vaoId] -= 1;
	}
}

// Static Functions

gl_retained_state_monitor& gl_retained_state_monitor::get_monitor() {
	return state_monitor;
}

gl_retained_state_monitor gl_retained_state_monitor::state_monitor;

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
