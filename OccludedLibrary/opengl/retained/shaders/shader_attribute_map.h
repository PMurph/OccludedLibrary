#pragma once

#include <map>
#include <ctype.h>

#ifndef UNIT_TESTING
#include <GL/glew.h>
#else
#include "opengl_mock.h"
#endif

#include "shader_program.h"
#include "../../../buffers/attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

/**
 * \class shader_attribute_map
 * \brief Handles the mapping of attributes in an attribute map to those in a shader program.
 *
 * This handles the mapping of attribute in an attribute map to those in a shader program. Its purpose is to reduce all the calls to
 * glVertexAttribPointer to a single a function call as well as to prevent calls to glVertexAttribPointer that would cause OpenGL to
 * enter an error state.
 */
class shader_attribute_map
{
private:
	const shader_program& m_shaderProg;
	const buffers::attributes::attribute_map m_attribMap;

	std::map< const std::string, std::pair<const std::string, GLint> > m_map;

public:
	/**
	 * \brief Initializes the shader attribute map.
	 *
	 * \param map A reference to an attribute map.
	 * \param shaderProg A reference to a shader program.
	 *
	 * Initializes the shader attribute map, by determining which attributes are active attributes in shader program and storing their
	 * a mapping from the name of the attribute to the id of the OpenGL attribute. A -1 will stored if the attribute does not exist in
	 * the shader program.
	 * \warning { The convention is for an attribute named "position" in the attribute map will search for "vPosition" attribute in 
	 * the shader program. This v is to indicate that the attribute is part of the vertex shader and meant to help in quickly locating it. }
	 */
	shader_attribute_map( const buffers::attributes::attribute_map& map, const shader_program& shaderProg );
	~shader_attribute_map();

	/**
	 * \fn set_attrib_pointers
	 * \brief Setups of all the vertex attrib pointers in preparation for a draw call.
	 * 
	 * \param buffer The attribute buffer the attrib pointers will be set for.
	 *
	 * Makes all the necessary calls to glVertexAttribPointer that are needed in order to make a glDraw call. Used the necessary preparation 
	 * for an OpenGL buffer object, thats data is organized according to the attribute_map, to be used by a glDraw call.
	 */
	void set_attrib_pointers( const buffers::attribute_buffer& buffer ) const;


private:
	/**
	 * \fn init_map
	 * \brief Populates the map with the locations of the attributes.
	 *
	 * Initializes the shader attribute map by populating the map from attribute_map attribute names to the shader_program attribute locations.
	 */
	void init_map();

	/**
	 * \fn get_shader_attrib_name
	 * \brief Converts the name of an attribute to its shader equivalent.
	 *
	 * \param attribName A reference to a string containing the name of attribute.
	 * \return A string containing the shader equivalent of the attribute name.
 	 *
	 * Takes the name of an attribute an converts it to its shader equivalent by capitalizing the first letter and adding a 'v' prefix.
	 */
	const std::string get_shader_attrib_name( const std::string& attribName ) const;
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace