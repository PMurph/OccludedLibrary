#include "gl_retained_object_manager.h"

namespace occluded { namespace opengl { namespace retained {

// Public Member Functions

void gl_retained_object_manager::delete_objects() {
	delete_vbos();
	delete_vaos();
	delete_shaders();
	delete_shader_programs();
}

const GLuint gl_retained_object_manager::get_new_vao() {
	GLuint vaoId = 0;

	glGenVertexArrays( 1, &vaoId );

	if( vaoId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_vao: Faieled to create an new vertex array object because of an error in OpenGL" );

	inc_entry( m_vaoRefCount, vaoId );

	return vaoId;
}

void gl_retained_object_manager::add_ref_to_vao( const GLuint vaoId ) {
	if( m_vaoRefCount.find( vaoId ) == m_vaoRefCount.end() || 
		( m_vaoRefCount.find( vaoId ) != m_vaoRefCount.end() && m_vaoRefCount[vaoId] == 0 ) ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_vao: Failed to add reference to vao because vao id("
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );
	}

	inc_entry( m_vaoRefCount, vaoId );
}

void gl_retained_object_manager::remove_ref_to_vao( const GLuint vaoId ) {
	if( m_vaoRefCount.find( vaoId ) == m_vaoRefCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_vao: Failed to remove reference to vao because vao id(" 
			+ boost::lexical_cast<std::string>( vaoId ) + ") does not correspond to a valid vao." );	
	}

	assert( vaoId != 0 );

	dec_entry( m_vaoRefCount, vaoId );

	if( m_vaoRefCount.find( vaoId )->second == 0 )
		glDeleteVertexArrays( 1, &vaoId );
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

	inc_entry( m_shaderRefCount, newShaderId );

	return newShaderId;
}

void gl_retained_object_manager::add_ref_to_shader( const GLuint shaderId ) {
	if( m_shaderRefCount.find( shaderId ) == m_shaderRefCount.end() || m_shaderRefCount[shaderId] == 0 ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_shader: Failed to add a reference to the shader because the shader id("
			+ boost::lexical_cast<std::string>( shaderId ) + ") does not correspond to a valid OpenGL shader object." );
	}

	inc_entry( m_shaderRefCount, shaderId );
}

void gl_retained_object_manager::remove_ref_to_shader( const GLuint shaderId ) {
	if( m_shaderRefCount.find( shaderId ) == m_shaderRefCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_shader: Failed to remove reference to shader because the shader id("
			+ boost::lexical_cast<std::string>( shaderId ) + ") does not correspond to a valid OpenGL shader object." );
	}

	dec_entry( m_shaderRefCount, shaderId );
	
	if( m_shaderRefCount.find( shaderId )->second == 0 )
		glDeleteShader( shaderId );
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
	GLuint newProgId = glCreateProgram();

	if( newProgId == 0 || GL_NO_ERROR != glGetError() )
		throw std::runtime_error( "gl_retained_object_manager.get_new_shader_prog: Failed to get new shader program because an error occured in OpenGL." );

	inc_entry( m_shaderProgRefCount, newProgId );

	return newProgId;
}

void gl_retained_object_manager::add_ref_to_shader_prog( const GLuint shaderProgId ) {
	if( m_shaderProgRefCount.find( shaderProgId ) == m_shaderProgRefCount.end() || m_shaderProgRefCount[shaderProgId] == 0 ) {
		throw std::runtime_error( "gl_retained_object_manager.add_ref_to_shader_prog: Failed to add reference to shader program because the id("
			+ boost::lexical_cast<std::string>( shaderProgId ) + ") does not correspond to a valid OpenGL shader program object." );
	}

	inc_entry( m_shaderProgRefCount, shaderProgId );
}

void gl_retained_object_manager::remove_ref_to_shader_prog( const GLuint shaderProgId ) {
	if( m_shaderProgRefCount.find( shaderProgId ) == m_shaderProgRefCount.end() ) {
		throw std::runtime_error( "gl_retained_object_manager.remove_ref_to_shader_prog: Failed to remove reference to shader program because " +
			std::string( "the shader program id(" ) + boost::lexical_cast<std::string>( shaderProgId ) 
			+ ") does not correspond to a valid OpenGL shader program object." );
	}

	dec_entry( m_shaderProgRefCount, shaderProgId );
	
	if( m_shaderProgRefCount.find( shaderProgId )->second == 0 )
		glDeleteProgram( shaderProgId );
}

const bool gl_retained_object_manager::check_valid_shader_prog_id( const GLuint shaderProgId ) const {
	bool isShaderProg = false;

	if( shaderProgId == 0 )
		throw std::runtime_error( "gl_retained_object_manager.check_valid_shader_prog_id: The invalid shader program id 0 was passed as a parameter." );

	if( m_shaderProgRefCount.find( shaderProgId ) != m_shaderProgRefCount.end() && m_shaderProgRefCount.find( shaderProgId )->second != 0 )
		isShaderProg = true;

	return isShaderProg;
}


// Private Member Functions

gl_retained_object_manager::gl_retained_object_manager():
	m_vaoRefCount(),
	m_vboRefCount(),
	m_shaderRefCount(),
	m_shaderProgRefCount()
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

void gl_retained_object_manager::delete_shader_programs() {
	for( std::map<const GLuint, unsigned int>::iterator it = m_shaderProgRefCount.begin(); it != m_shaderProgRefCount.end(); ++it ) {
		if( it->second != 0 ) {
			glDeleteProgram( it->second );

			assert( GL_NO_ERROR == glGetError() );

			it->second = 0;
		}
	}
}

void gl_retained_object_manager::inc_entry( std::map<const GLuint, unsigned int>& refCounter, const GLuint id ) {
	assert( id != 0 );

	if( id != 0 ) {
		if( refCounter.find( id ) == refCounter.end() )
			refCounter.insert( std::pair<const GLuint, unsigned int>( id, 1 ) );
		else
			refCounter[id] += 1;
	}
}

void gl_retained_object_manager::dec_entry( std::map<const GLuint, unsigned int>& refCounter, const GLuint id ) {
	assert( id != 0 && refCounter.find( id ) != refCounter.end() );

	if( id != 0 && refCounter.find( id ) != refCounter.end() ) {
		if( refCounter[id] == 0 ) {
			throw std::runtime_error( "gl_retained_object_manager.dec_vao_entry: Failed to decrement the ref count for vao id(" + 
				boost::lexical_cast<std::string>( id ) + ") is already 0." );
		}

		refCounter[id] -= 1;
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

// Static Functions

gl_retained_object_manager& gl_retained_object_manager::get_manager() {
	return object_manager;
}

gl_retained_object_manager gl_retained_object_manager::object_manager;

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
