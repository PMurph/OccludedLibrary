#include "shader_uniform_store.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

void shader_uniform_store::add_uniform( const std::string& name, const uniform_value& value ) {
	GLint uniformId = -1;
	std::string convertedName = convert_to_uniform_name( name );
	boost::shared_ptr<shader_program> shaderProg;

	// The shaderProgId should have been set by the shader_program and therefore not be 0
	assert( m_shaderProgId != 0 );
	
	// Check to make sure uniform doesn't already exist
	if( m_store.find( convertedName ) != m_store.end() ) {
		throw std::runtime_error( "shader_uniform_store.add_uniform: Failed to add uniform because it already exists in the store." );
	}

	uniformId = glGetUniformLocation( m_shaderProgId, convertedName.c_str() );

	// Check to make sure the call has not caused OpenGL to enter an error state
	if( GL_NO_ERROR != glGetError() ) {
		throw std::runtime_error( "shader_uniform_store.add_uniform_value: OpenGL entered an error state after attempting to get location of uniform(" 
		+ convertedName + ")." );
	}

	m_store.insert( std::pair< const std::string, std::pair<GLint, uniform_value> >( convertedName, std::pair<GLint, uniform_value>( uniformId, value ) ) );
}

void shader_uniform_store::set_uniform_value( const std::string& name, const uniform_value& value ) {
	std::map< const std::string, std::pair<GLint, uniform_value> >::iterator currValue;

	// The shaderProgId should have been set by the shader_program and therefore not be 0
	assert( m_shaderProgId != 0 );

	currValue = m_store.find( convert_to_uniform_name( name ) );

	// Check to make sure the uniform exists
	if( currValue == m_store.end() ) {
		throw std::runtime_error( "shader_uniform_store.set_uniform_value: Failed to set uniform value because the uniform(" 
			+ name + ") does not exist in the store." );
	}

	// Check to make sure the type of the uniform value matches the stored uniform
	if( value.type() != currValue->second.second.type() ) {
		throw std::runtime_error( "shader_uniform_store.set_uniform_value: Failed to set uniform value because the type of the uniform value " +
			std::string( "passed is not the same as the type of the uniform value found." ) );
	}

	// Update the value stored
	currValue->second.second = value;
}

const bool shader_uniform_store::has_uniform( const std::string& name ) const {
	bool hasValue = false;

	if( m_store.find( convert_to_uniform_name( name ) ) != m_store.end() )
		hasValue = true;

	return hasValue;
}

// Private Member Function

shader_uniform_store::shader_uniform_store():
	m_shaderProgId( 0 )
{
}

shader_uniform_store::shader_uniform_store( const shader_uniform_store& toCopy ):
	m_shaderProgId( toCopy.m_shaderProgId ),
	m_store( toCopy.m_store )
{
}

shader_uniform_store::~shader_uniform_store()
{
	m_shaderProgId = 0;
}

void shader_uniform_store::pass_to_shader() const {
	// The shaderProgId should have been set by the shader_program and therefore not be 0
	assert( m_shaderProgId != 0 );

	for( std::map< const std::string, std::pair<GLint, uniform_value> >::const_iterator it = m_store.begin(); it != m_store.end(); ++it ) {
		// Check to make sure the uniform has a valid location
		if( it->second.first != -1 ) {
			gl_uniform_visitor uniformVis( it->second.first );

			// Make the necessary OpenGL call
			it->second.second.apply_visitor( uniformVis );
		}
	}
}

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
