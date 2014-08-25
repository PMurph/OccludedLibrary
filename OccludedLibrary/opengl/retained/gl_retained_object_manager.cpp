#include "gl_retained_object_manager.h"

namespace occluded { namespace opengl { namespace retained {

// Public Member Functions

void gl_retained_object_manager::delete_objects() {
	delete_vbos();
	delete_vaos();
}

const GLuint gl_retained_object_manager::get_new_vao() {
	GLuint vaoId = 0;

	glGenVertexArrays( 1, &vaoId );

	if( vaoId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_vao: Faieled to create an new vertex array object because of an error in OpenGL" );

	inc_vao_entry( vaoId );

	return vaoId;
}

void gl_retained_object_manager::add_ref_to_vao( const GLuint vaoId ) {
	if( m_vaoCount.find( vaoId ) == m_vaoCount.end() || 
		( m_vaoCount.find( vaoId ) != m_vaoCount.end() && m_vaoCount[vaoId] == 0 ) ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_vao: Failed to add reference to vao because vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );
	}

	assert( vaoId != 0 );

	inc_vao_entry( vaoId );
}

void gl_retained_object_manager::remove_ref_to_vao( const GLuint vaoId ) {
	if( m_vaoCount.find( vaoId ) == m_vaoCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_vao: Failed to remove reference to vao because vao id(" 
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );	
	}

	assert( vaoId != 0 );

	dec_vao_entry( vaoId );
}

const bool gl_retained_object_manager::check_valid_vao_id( const GLuint vaoId ) const {
	bool isValid = false;

	if( vaoId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_vao_id: The invalid vao id 0 was passed as the parameter." );

	if( m_vaoCount.find( vaoId ) != m_vaoCount.end() && m_vaoCount.find( vaoId )->second != 0 )
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

	inc_vbo_entry( std::pair<const GLuint, const GLuint>( vaoId, vboId ) );
	m_vaoCount[vaoId] += 1;

	return vboId;
}

void gl_retained_object_manager::add_ref_to_vbo( const GLuint vaoId, const GLuint vboId ) {
	std::pair<const GLuint, const GLuint> key( vaoId, vboId ) ;

	if( m_vboCount.find( key ) == m_vboCount.end() || m_vboCount[key] == 0 ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_vbo: Failed to add a reference to the vertex buffer object because the vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") or the vbo id(" + boost::lexical_cast<std::string>( vboId ) + 
			") does not correspond to a OpenGL object." );
	}

	inc_vbo_entry( key );
	m_vaoCount[vaoId] += 1;

	assert( m_vaoCount[vaoId] >= m_vboCount[key] );
}

void gl_retained_object_manager::remove_ref_to_vbo( const GLuint vaoId, const GLuint vboId ) {
	std::pair<const GLuint, const GLuint> key( vaoId, vboId );

	if( m_vboCount.find( key ) == m_vboCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_vbo: Failed to remove a reference to the vertex buffer object becayse the vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") or the vbo id(" + boost::lexical_cast<std::string>( vboId ) + 
			") does not correspond to an OpenGL object." );
	}

	dec_vbo_entry( key );
	m_vaoCount[vaoId] -= 1;

	assert( m_vaoCount[vaoId] >= m_vboCount[key] );
}

const bool gl_retained_object_manager::check_valid_vbo_id( const GLuint vaoId, const GLuint vboId ) const {
	bool valid = false;
	std::pair<const GLuint, const GLuint> key( vaoId, vboId );

	if( vaoId == 0 || vboId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_vbo_id: An invalid value of 0 was passed as a parameter." );

	if( m_vboCount.find( key ) != m_vboCount.end() && m_vboCount.find( key )->second != 0 )
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
	delete_objects();
}

void gl_retained_object_manager::delete_vaos() {
	for( std::map< const GLuint, unsigned int>::iterator it = m_vaoCount.begin(); it != m_vaoCount.end(); ++it ) {
		if( it->second != 0 ) {
			glDeleteVertexArrays( 1, &(it->first) );

			assert( GL_NO_ERROR == glGetError() );

			it->second = 0;
		}
	}
}

void gl_retained_object_manager::delete_vbos() {
	for( std::map< const std::pair<const GLuint, const GLuint>, unsigned int >::iterator it = m_vboCount.begin(); it != m_vboCount.end(); ++it ) {
		if( it->second != 0 ) {
			glBindVertexArray( it->first.first );
			glDeleteBuffers(1, &(it->first.second) );

			assert( GL_NO_ERROR == glGetError() );

			it->second = 0;
		}
	}
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

		if( m_vaoCount[vaoId] == 0 )
			glDeleteVertexArrays( 1, &vaoId );
	}
}

void gl_retained_object_manager::inc_vbo_entry( const std::pair<const GLuint, const GLuint>& key ) {
	GLuint vaoId = key.first;
	GLuint vboId = key.second;

	assert( vaoId != 0 && vboId != 0 );

	if( vaoId != 0 && vboId != 0 ) {
		std::pair<const GLuint, const GLuint> key( vaoId, vboId );

		if( m_vboCount.find( key ) == m_vboCount.end() ) {
			m_vboCount.insert( std::pair< const std::pair<const GLuint, const GLuint>, unsigned int>( key, 1 ) );
		} else {
			m_vboCount[key] += 1;
		}
	}
}

void gl_retained_object_manager::dec_vbo_entry( const std::pair<const GLuint, const GLuint>& key ) {
	GLuint vaoId = key.first;
	GLuint vboId = key.second;

	assert( vaoId != 0 && vboId != 0 && m_vboCount.find( key ) != m_vboCount.end() );

	if( vaoId != 0 && vboId != 0 && m_vboCount.find( key ) != m_vboCount.end() ) {
		if( m_vboCount[key] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.inc_vbo_entry: Failed to decrement the ref count for the vao id("
				+ boost::lexical_cast<std::string>( vaoId ) + ") and vbo id(" + boost::lexical_cast<std::string>( vboId ) + ") is already 0" );
		}

		m_vboCount[key] -= 1;

		if( m_vboCount[key] == 0 ) {
			glBindVertexArray( key.first );

			glDeleteBuffers( 1, &(key.second) );
		}
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
