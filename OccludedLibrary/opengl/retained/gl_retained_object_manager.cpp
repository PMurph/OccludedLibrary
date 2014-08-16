#include "gl_retained_object_manager.h"

namespace occluded { namespace opengl { namespace retained {

// Public Member Functions

const GLuint gl_retained_object_manager::get_new_vao() {
	GLuint vaoId = 0;

	glGenVertexArrays( 1, &vaoId );

	if( vaoId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_vao: Faieled to create an new vertex array object because of an error in OpenGL" );

	inc_vao_entry( vaoId );

	return vaoId;
}

void gl_retained_object_manager::add_ref_to_vao( const GLuint vaoId ) {
	if( vaoId == 0 || m_vaoCount.find( vaoId ) == m_vaoCount.end() || 
		( m_vaoCount.find( vaoId ) != m_vaoCount.end() && m_vaoCount[vaoId] == 0 ) ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_vao: Failed to add reference to vao because vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );
	}

	inc_vao_entry( vaoId );
}

void gl_retained_object_manager::remove_ref_to_vao( const GLuint vaoId ) {
	if( vaoId == 0 || m_vaoCount.find( vaoId ) == m_vaoCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_vao: Failed to remove reference to vao because vao id(" 
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );	
	}

	dec_vao_entry( vaoId );
}

const bool gl_retained_object_manager::check_valid_vao_id( const GLuint vaoId ) const {
	bool isValid = false;

	if( vaoId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_vao_id: The invalid vao id 0 was passed as the parameter." );

	if( m_vaoCount.find( vaoId ) != m_vaoCount.end() )
		isValid = true;

	return isValid;
}


const GLuint gl_retained_object_manager::get_new_vbo( const GLuint vaoId ) {
	GLuint vboId = 0;

	if( vaoId == 0 || m_vaoCount.find( vaoId ) == m_vaoCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.get_new_vbo: Failed to generate a vbo because an invalid vao id(" + 
			boost::lexical_cast<std::string>( vaoId ) + ") was passed as a parameter" );
	}

	glBindVertexArray( vaoId );
	glGenBuffers( 1, &vboId );

	if( vboId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_vbo: Failed to generate a vertex buffer object because of an error in OpenGL" );

	inc_vbo_entry( vaoId, vboId );

	return vboId;
}

const bool gl_retained_object_manager::check_valid_vbo_id( const GLuint vaoId, const GLuint vboId ) const {
	bool valid = false;
	std::pair<const GLuint, const GLuint> key( vaoId, vboId );

	if( vaoId == 0 || vboId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_vbo_id: An invalid value of 0 was passed as a parameter." );

	if( m_vboCount.find( key ) != m_vboCount.end() )
		valid = true;

	return valid;
}

// Private Member Functions

gl_retained_object_manager::gl_retained_object_manager():
	m_vaoCount(),
	m_vboCount()
{
}


gl_retained_object_manager::~gl_retained_object_manager()
{
}

void gl_retained_object_manager::inc_vao_entry( const GLuint vaoId ) {
	assert( vaoId != 0 );

	if( vaoId != 0 ) {
		if( m_vaoCount.find( vaoId ) == m_vaoCount.end() ) {
			m_vaoCount.insert( std::pair<const GLuint, unsigned int>( vaoId, 1 ) );
		} else {
			m_vaoCount[vaoId] += 1;
		}
	}
}

void gl_retained_object_manager::dec_vao_entry( const GLuint vaoId ) {
	assert( vaoId != 0 && m_vaoCount.find( vaoId ) != m_vaoCount.end() );

	if( vaoId != 0 && m_vaoCount.find( vaoId ) != m_vaoCount.end() ) {
		if( m_vaoCount[vaoId] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.dec_vao_entry: Failed to decrement the ref count for vao id(" + 
				boost::lexical_cast<std::string>( vaoId ) + ") is already 0." );
		}

		m_vaoCount[vaoId] -= 1;
	}
}

void gl_retained_object_manager::inc_vbo_entry( const GLuint vaoId, const GLuint vboId ) {
	assert( vaoId != 0 && vboId != 0 );

	if( vaoId != 0 && vboId != 0 ) {
		std::pair<const GLuint, const GLuint> key( vaoId, vboId );

		if( m_vboCount.find( key ) != m_vboCount.end() ) {
			m_vboCount.insert( std::pair< const std::pair<const GLuint, const GLuint>, unsigned int>( key, 1 ) );
		} else {
			m_vboCount[key] += 1;
		}
	}
}

void gl_retained_object_manager::dec_vbo_entry( const GLuint vaoId, const GLuint vboId ) {
	std::pair<const GLuint, const GLuint> key( vaoId, vboId );

	assert( vaoId != 0 && vboId != 0 && m_vboCount.find( key ) != m_vboCount.end() );

	if( vaoId != 0 && vboId != 0 && m_vboCount.find( key ) != m_vboCount.end() ) {
		if( m_vboCount[key] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.inc_vbo_entry: Failed to decrement the ref count for the vao id("
				+ boost::lexical_cast<std::string>( vaoId ) + ") and vbo id(" + boost::lexical_cast<std::string>( vboId ) + ") is already 0" );
		}

		m_vboCount[key] -= 1;
	}
}

// Static Functions

gl_retained_object_manager& gl_retained_object_manager::get_manager() {
	return object_manager;
}

gl_retained_object_manager gl_retained_object_manager::object_manager;

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
