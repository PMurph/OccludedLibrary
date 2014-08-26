#include "gl_retained_object_manager.h"

namespace occluded { namespace opengl { namespace retained {

// Public Member Functions

void gl_retained_object_manager::delete_objects() {
	delete_vbos();
	delete_vaos();
	delete_shaders();
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
	if( m_vaoRefCount.find( vaoId ) == m_vaoRefCount.end() || 
		( m_vaoRefCount.find( vaoId ) != m_vaoRefCount.end() && m_vaoRefCount[vaoId] == 0 ) ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_vao: Failed to add reference to vao because vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );
	}

	inc_vao_entry( vaoId );
}

void gl_retained_object_manager::remove_ref_to_vao( const GLuint vaoId ) {
	if( m_vaoRefCount.find( vaoId ) == m_vaoRefCount.end() ) {
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

	if( m_vaoRefCount.find( vaoId ) != m_vaoRefCount.end() && m_vaoRefCount.find( vaoId )->second != 0 )
		isValid = true;

	return isValid;
}


const GLuint gl_retained_object_manager::get_new_vbo( const GLuint vaoId ) {
	GLuint vboId = 0;

	if( vaoId == 0 || m_vaoRefCount.find( vaoId ) == m_vaoRefCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.get_new_vbo: Failed to generate a vbo because an invalid vao id(" + 
			boost::lexical_cast<std::string>( vaoId ) + ") was passed as a parameter" );
	}

	glBindVertexArray( vaoId );
	glGenBuffers( 1, &vboId );

	if( vboId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_vbo: Failed to generate a vertex buffer object because of an error in OpenGL" );

	inc_vbo_entry( std::pair<const GLuint, const GLuint>( vaoId, vboId ) );
	m_vaoRefCount[vaoId] += 1;

	return vboId;
}

void gl_retained_object_manager::add_ref_to_vbo( const GLuint vaoId, const GLuint vboId ) {
	std::pair<const GLuint, const GLuint> key( vaoId, vboId ) ;

	if( m_vboRefCount.find( key ) == m_vboRefCount.end() || m_vboRefCount[key] == 0 ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_vbo: Failed to add a reference to the vertex buffer object because the vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") or the vbo id(" + boost::lexical_cast<std::string>( vboId ) + 
			") does not correspond to a OpenGL object." );
	}

	inc_vbo_entry( key );
	m_vaoRefCount[vaoId] += 1;

	assert( m_vaoRefCount[vaoId] >= m_vboRefCount[key] );
}

void gl_retained_object_manager::remove_ref_to_vbo( const GLuint vaoId, const GLuint vboId ) {
	std::pair<const GLuint, const GLuint> key( vaoId, vboId );

	if( m_vboRefCount.find( key ) == m_vboRefCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_vbo: Failed to remove a reference to the vertex buffer object because the vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") or the vbo id(" + boost::lexical_cast<std::string>( vboId ) + 
			") does not correspond to an OpenGL object." );
	}

	dec_vbo_entry( key );
	m_vaoRefCount[vaoId] -= 1;

	assert( m_vaoRefCount[vaoId] >= m_vboRefCount[key] );
}

const bool gl_retained_object_manager::check_valid_vbo_id( const GLuint vaoId, const GLuint vboId ) const {
	bool valid = false;
	std::pair<const GLuint, const GLuint> key( vaoId, vboId );

	if( vaoId == 0 || vboId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_vbo_id: An invalid value of 0 was passed as a parameter." );

	if( m_vboRefCount.find( key ) != m_vboRefCount.end() && m_vboRefCount.find( key )->second != 0 )
		valid = true;

	return valid;
}

const GLuint gl_retained_object_manager::get_new_shader( const shaders::shader_type_t shaderType ) {
	GLuint newShaderId = glCreateShader( shaderType );

	if( newShaderId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_shader: Failed to get new shader because an error occured in OpenGL." );

	inc_shader_entry( newShaderId );

	return newShaderId;
}

void gl_retained_object_manager::add_ref_to_shader( const GLuint shaderId ) {
	if( m_shaderRefCount.find( shaderId ) == m_shaderRefCount.end() || m_shaderRefCount[shaderId] == 0 ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_shader: Failed to add a reference to the shader because the shader id("
			+ boost::lexical_cast<std::string>( shaderId ) + ") does not correspond to a valid OpenGL shader object." );
	}

	inc_shader_entry( shaderId );
}

void gl_retained_object_manager::remove_ref_to_shader( const GLuint shaderId ) {
	if( m_shaderRefCount.find( shaderId ) == m_shaderRefCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_shader: Failed to remove reference to shader because the shader id("
			+ boost::lexical_cast<std::string>( shaderId ) + ") does not correspond to a valid OpenGL shader object." );
	}

	dec_shader_entry( shaderId );
}

const bool gl_retained_object_manager::check_valid_shader_id( const GLuint shaderId ) const {
	bool isShader = false;

	if( shaderId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_shader_id: The invalid shader id 0 was passed as a parameter." );

	if( m_shaderRefCount.find( shaderId ) != m_shaderRefCount.end() && m_shaderRefCount.find( shaderId )->second != 0 )
		isShader = true;

	return isShader;
}

const GLuint gl_retained_object_manager::get_new_shader_prog() {
	return 0;
}

void gl_retained_object_manager::add_ref_to_shader_prog( const GLuint shaderProgId ) {
}

void gl_retained_object_manager::remove_ref_to_shader_prog( const GLuint shaderProgId ) {
}

const bool gl_retained_object_manager::check_valid_shader_prog_id( const GLuint shaderId ) const {
	return false;
}


// Private Member Functions

gl_retained_object_manager::gl_retained_object_manager():
	m_vaoRefCount(),
	m_vboRefCount(),
	m_shaderRefCount()
{
}


gl_retained_object_manager::~gl_retained_object_manager()
{
	delete_objects();
}

void gl_retained_object_manager::delete_vaos() {
	for( std::map< const GLuint, unsigned int>::iterator it = m_vaoRefCount.begin(); it != m_vaoRefCount.end(); ++it ) {
		if( it->second != 0 ) {
			glDeleteVertexArrays( 1, &(it->first) );

			assert( GL_NO_ERROR == glGetError() );

			it->second = 0;
		}
	}
}

void gl_retained_object_manager::delete_vbos() {
	for( std::map< const std::pair<const GLuint, const GLuint>, unsigned int >::iterator it = m_vboRefCount.begin(); it != m_vboRefCount.end(); ++it ) {
		if( it->second != 0 ) {
			glBindVertexArray( it->first.first );
			glDeleteBuffers(1, &(it->first.second) );

			assert( GL_NO_ERROR == glGetError() );

			it->second = 0;
		}
	}
}

void gl_retained_object_manager::delete_shaders() {
	for( std::map<const GLuint, unsigned int>::iterator it = m_shaderRefCount.begin(); it != m_shaderRefCount.end(); ++it ) {
		if( it->second != 0 ) {
			glDeleteShader( it->second );

			assert( GL_NO_ERROR == glGetError() );

			it->second = 0;
		}
	}
}

void gl_retained_object_manager::inc_vao_entry( const GLuint vaoId ) {
	assert( vaoId != 0 );

	if( vaoId != 0 ) {
		if( m_vaoRefCount.find( vaoId ) == m_vaoRefCount.end() )
			m_vaoRefCount.insert( std::pair<const GLuint, unsigned int>( vaoId, 1 ) );
		else
			m_vaoRefCount[vaoId] += 1;
	}
}

void gl_retained_object_manager::dec_vao_entry( const GLuint vaoId ) {
	assert( vaoId != 0 && m_vaoRefCount.find( vaoId ) != m_vaoRefCount.end() );

	if( vaoId != 0 && m_vaoRefCount.find( vaoId ) != m_vaoRefCount.end() ) {
		if( m_vaoRefCount[vaoId] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.dec_vao_entry: Failed to decrement the ref count for vao id(" + 
				boost::lexical_cast<std::string>( vaoId ) + ") is already 0." );
		}

		m_vaoRefCount[vaoId] -= 1;

		if( m_vaoRefCount[vaoId] == 0 )
			glDeleteVertexArrays( 1, &vaoId );
	}
}

void gl_retained_object_manager::inc_vbo_entry( const std::pair<const GLuint, const GLuint>& key ) {
	GLuint vaoId = key.first;
	GLuint vboId = key.second;

	assert( vaoId != 0 && vboId != 0 );

	if( vaoId != 0 && vboId != 0 ) {
		std::pair<const GLuint, const GLuint> key( vaoId, vboId );

		if( m_vboRefCount.find( key ) == m_vboRefCount.end() ) {
			m_vboRefCount.insert( std::pair< const std::pair<const GLuint, const GLuint>, unsigned int>( key, 1 ) );
		} else {
			m_vboRefCount[key] += 1;
		}
	}
}

void gl_retained_object_manager::dec_vbo_entry( const std::pair<const GLuint, const GLuint>& key ) {
	GLuint vaoId = key.first;
	GLuint vboId = key.second;

	assert( vaoId != 0 && vboId != 0 && m_vboRefCount.find( key ) != m_vboRefCount.end() );

	if( vaoId != 0 && vboId != 0 && m_vboRefCount.find( key ) != m_vboRefCount.end() ) {
		if( m_vboRefCount[key] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.dec_vbo_entry: Failed to decrement the ref count for the vao id("
				+ boost::lexical_cast<std::string>( vaoId ) + ") and vbo id(" + boost::lexical_cast<std::string>( vboId ) + ") is already 0" );
		}

		m_vboRefCount[key] -= 1;

		if( m_vboRefCount[key] == 0 ) {
			glBindVertexArray( key.first );

			glDeleteBuffers( 1, &(key.second) );
		}
	}
}

void gl_retained_object_manager::inc_shader_entry( const GLuint shaderId ) {
	assert( shaderId != 0 );

	if( shaderId != 0 ) {
		if( m_shaderRefCount.find( shaderId ) == m_shaderRefCount.end() )
			m_shaderRefCount.insert( std::pair<const GLuint, unsigned int>( shaderId, 1 ) );
		else
			m_shaderRefCount[shaderId] += 1;
	}
}

void gl_retained_object_manager::dec_shader_entry( const GLuint shaderId ) {
	assert( shaderId != 0 );

	if( shaderId != 0 && m_shaderRefCount.find( shaderId ) != m_shaderRefCount.end() ) {
		if( m_shaderRefCount[shaderId] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.dec_shader_entry: Failed to decrement the ref count for the shader id(" +
				boost::lexical_cast<std::string>( shaderId ) + ") because the ref count is already 0." );
		}

		m_shaderRefCount[shaderId] -= 1;

		if( m_shaderRefCount[shaderId] == 0 )
			glDeleteShader( shaderId );
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
