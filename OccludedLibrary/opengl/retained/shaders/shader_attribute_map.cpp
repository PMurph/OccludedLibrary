#include "shader_attribute_map.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

shader_attribute_map::shader_attribute_map( const buffers::attributes::attribute_map& map, const shader_program& shaderProg ):
	m_attribMap( map ),
	m_shaderProg( shaderProg )
{
	if( m_attribMap.being_defined() )
		throw std::runtime_error( "shader_attribute_map.shader_attribute_map: Failed to create shader_attribute_map because attribute_map provided is still being defined." );

	if( !m_shaderProg.is_linked() )
		throw std::runtime_error( "shader_attribute_map.shader_attribute_map: Failed to create shader_attribute_map because shader_program provided has not been linked." );

	init_map();
}


shader_attribute_map::~shader_attribute_map()
{
}

void shader_attribute_map::set_attrib_pointers( const buffers::attribute_buffer& buffer ) const {
	unsigned int i = 0;
	std::vector<const buffers::attributes::attribute> attributes = m_attribMap.get_attributes();

	// Checks to make sure the buffer's attribute map is the same as the attribute map
	if( buffer.get_attribute_map() != m_attribMap ) {
		throw std::runtime_error( "shader_attribute_map.set_attrib_pointers: Failed to set attribute pointers because the attribute_buffer's attribute_map passed does not match"
			+ std::string( " the attribute_map contained byy the shader_attribute_map." ) );
	}

	// Ensure that the correct shader program is being used
	m_shaderProg.use_program();

	for( i = 0; i < attributes.size(); ++i ) {
		std::map< const std::string, std::pair<const std::string, GLint> >::const_iterator entry = m_map.find( attributes[i].get_name() );
		
		// Check to make sure that the name of the attribute is in the map
		if( entry == m_map.end() ) {
			throw std::runtime_error( "shader_attribute_map.set_attrib_pointers: Failed to set attribute pointers because attribute(" + attributes[i].get_name() + 
				") was found in attribute_map but not in shader_attribute_map." );
		}

		// If the location exists, setup the attribute pointer
		if( entry->second.second >= 0 ) {
			if( m_attribMap.is_interleaved() ) {
				glVertexAttribPointer( static_cast<GLuint>( entry->second.second ), static_cast<GLint>( attributes[i].get_arity() ), attributes[i].get_type(), 
					static_cast<GLboolean>( attributes[i].is_normalized() ), static_cast<GLsizei>( m_attribMap.get_byte_size() ), 
					reinterpret_cast<const GLvoid*>( buffer.get_attribute_data_offsets()[i] ) );
			} else {
				glVertexAttribPointer( static_cast<GLuint>( entry->second.second ), static_cast<GLint>( attributes[i].get_arity() ), attributes[i].get_type(),
					static_cast<GLboolean>( attributes[i].is_normalized() ), static_cast<GLsizei>( 0 ), 
					reinterpret_cast<const GLvoid*>( buffer.get_attribute_data_offsets()[i] ) );
			}

			if( GL_NO_ERROR == glGetError() )
				throw std::runtime_error( "shader_attribute_map.set_attrib_pointers: OpenGL entered an error state after a vertex attrib pointer was attempted to be setup." );
		}
	}
}

// Private Member Functions

void shader_attribute_map::init_map() {
	std::vector<const buffers::attributes::attribute> attributes = m_attribMap.get_attributes();

	// Get the location of each attribute in the shader program
	for( std::vector<const buffers::attributes::attribute>::const_iterator it = attributes.begin(); it != attributes.end(); ++it ) {
		GLint location = -1;
		const std::string shaderName = get_shader_attrib_name( it->get_name() );

		location = glGetAttribLocation( m_shaderProg.get_id(), static_cast<const GLchar *>( shaderName.c_str() ) );

		// Check to make sure OpenGL hasn't entered an error state
		if( GL_NO_ERROR != glGetError() )
			throw std::runtime_error( "shader_attribute_map.init_map: OpenGL entered an error state after attempting to get location of a attribute(" + shaderName + ")." );

		// Insert a mapping from the attribute name(in the attribute_map) to a pair of attribute name(shader program) and attrib location in shader program 
		m_map.insert( std::pair< const std::string, std::pair<const std::string, GLint> >( it->get_name(), std::pair<const std::string, GLint>( shaderName, location ) ) );

		// Enable vertex attrib arrays for the location
		if( location >= 0 ) {
			glEnableVertexAttribArray( static_cast<GLuint>( location ) );
		
			// Make sure the enabling of the vertex attrib array does no cause OpenGL to enter error state
			if( GL_NO_ERROR != glGetError() ) {
				throw std::runtime_error( "shader_attribute_map.init_map: OpenGL entered an error state after attempting to enable vertex attribute arrays for attribute(" 
					+ shaderName + ")." );
			}
		}

	}
}

const std::string shader_attribute_map::get_shader_attrib_name( const std::string& attribName ) const {
	std::string shaderAttribName;
	
	if( attribName.size() == 0 )
		throw std::runtime_error( "shader_attribute_map.get_shader_attrib_name: Failed to convert string because an empty string was passed as a parameter." );

	if( attribName.size() == 1 ) {
		shaderAttribName = std::string( std::string( "v" ) + static_cast<char>( toupper( attribName[0] ) ) );
	} else {
		shaderAttribName = std::string( std::string( "v" ) + static_cast<char>( toupper( attribName[0] ) ) + attribName.substr(1) );
	}

	return shaderAttribName;
}

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace