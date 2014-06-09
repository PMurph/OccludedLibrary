#include "shader_uniform_store.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader_uniform_store::shader_uniform_store( const shader_program& shaderProg ):
	m_shaderProg( shaderProg )
{
	if( !shaderProg.is_linked() )
		throw std::runtime_error( "shader_uniform_store: Failed to initialize shader uniform store because shader program is not linked." );
}


shader_uniform_store::~shader_uniform_store()
{
}

void shader_uniform_store::add_uniform( const std::string& name, const uniform_value& value ) {
	GLint uniformId = -1;
	std::string convertedName = convert_to_uniform_name( name );

	// The linked status of the shader program should have already been checked in the constructor
	assert( m_shaderProg.is_linked() );
	
	// Check to make sure uniform doesn't already exist
	if( m_store.find( convertedName ) != m_store.end() ) {
		throw std::runtime_error( "shader_uniform_store.add_uniform: Failed to add uniform because it already exists in the store." );
	}

	uniformId = glGetUniformLocation( m_shaderProg.get_id(), convertedName.c_str() );

	// Check to make sure the call has not caused OpenGL to enter an error state
	if( GL_NO_ERROR == glGetError() )
		throw std::runtime_error( "shader_uniform_store.add_uniform_value: OpenGL entered an error state after attempting to get location of uniform(" + convertedName + ")." );

	m_store.insert( std::pair< const std::string, std::pair<GLint, uniform_value> >( convertedName, std::pair<GLint, uniform_value>( uniformId, value ) ) );
}

// Private Member Function

const std::string shader_uniform_store::convert_to_uniform_name( const std::string& name ) const {
	std::string convertedName;
	
	if( name.size() == 0 ) {
		throw std::runtime_error( "shader_uniform_store.convert_to_uniform_name: Failed to convert the name because it is an empty string." );
	}

	if( name.size() == 1 ) {
		convertedName = std::string( "u" ) + static_cast<char>( toupper( name[0] ) );
	} else {
		convertedName = std::string( "u" ) + static_cast<char>( toupper( name[0] ) ) + name.substr( 1 );
	}

	return convertedName;
}

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
