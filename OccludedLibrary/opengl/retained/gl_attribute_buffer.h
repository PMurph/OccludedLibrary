#pragma once

#include <GL\glew.h>

#include "../../buffers/attribute_buffer_factory.h"
#include "shaders/shader_attribute_map.h"

namespace occluded { namespace opengl { namespace retained {

/**
 * \enum buffer_usage_t
 * \brief An enumerable that determines tells OpenGL how a buffer can be used.
 *
 * An enumerable that determines tells OpenGL how a buffer can be used. This is to prevent the creator of the buffer from passing
 * an incorrect usage type to the constructor.
 */
typedef enum BUFFER_USAGE {
	static_draw_usage = GL_STATIC_DRAW,
	stream_draw_usage = GL_STREAM_DRAW,
	dynamic_draw_usage = GL_DYNAMIC_DRAW
} buffer_usage_t;

/**
 * \class gl_attribute_buffer
 * \brief A wrapper class for an OpenGL buffer.
 *
 * A wrapper class for an OpenGL buffer object. It stores information about the buffer to allow for quickly accessing that information without querying
 * the OpenGL buffer object. It also contains an attribute_buffer object that will allow for storage of data outside of OpenGL which makes inserting data
 * into the buffer significantly more easy as well as allowing for checking to make sure data being inserted is valid.
 */
class gl_attribute_buffer
{
private:
	GLuint m_id;
	std::auto_ptr<buffers::attribute_buffer> m_buffer;
	buffer_usage_t m_usage;
	shaders::shader_attribute_map m_shaderMap;

public:
	/**
	 * \brief Initialize the buffer.
	 *
	 * \param map A reference to an attribute map.
	 * \param shaderProg A reference to a shader program.
	 * \param usage A enumerable that will be used to tell OpenGL how the buffer will be used.
	 *
	 * Generate the an OpenGL buffer object, creates an attribute buffer to store data inserted into the buffer, and binds that buffer
	 * as an array buffer.
	 */
	gl_attribute_buffer( const buffers::attributes::attribute_map& map, const shaders::shader_program& shaderProg, const buffer_usage_t usage );
	~gl_attribute_buffer();
	
	/**
	 * \fn insert_values
	 * \brief Inserts data into the buffer.
	 *
	 * \param A reference to a vector of bytes containing the data to be inserted.
	 *
	 * Inserts data into the attribute buffer, then binds the buffer and sets the OpenGL data store to the data in the attribute buffer for the 
	 * OpenGL buffer object connected to this buffer.
	 */
	void insert_values( const std::vector<char>& values );

	/**
	 * \fn bind_buffer
	 * \brief Binds the buffer as an array buffer object.
	 *
	 * Binds the buffer as an array buffer object and sets the OpenGL buffer's data store to the contents of the attribute buffer. Should
	 * be called prior to using the buffer in an OpenGL draw call or after the data in the attribute buffer has changed.
	 */
	void bind_buffer() const;

	/**
	 * \fn get_id
	 * \brief Gets the id of the OpenGL buffer object.
	 *
	 * Gets the id of the OpenGL buffer object connected to this buffer. If this buffer is being used in a STL container, the value returned will
	 * change when the gl_attribute_buffer is inserted into the container.
	 * \warning Do not call glDeleteBuffer on the id returned from this function call. It will cause an OpenGL to enter an error state when the
	 * the gl_attribute_buffers destructor is called.
	 */
	const GLuint get_id() const;

	/**
	 * \fn get_buffer_map
	 * \brief Gets the attribute_map of the attribute_buffer.
	 * \return A reference to the attribute_map used in the attribute_buffer contained by the gl_attribute_buffer.
	 *
	 * Gets a reference to the attribute_map used by the attribute_buffer contained by the gl_attribute_buffer. Intended to be used for formatting data
	 * to be inserted into the gl_attribute_buffer as well as determining the inputs to a glVertexAttribPointer call.
	 */
	const buffers::attributes::attribute_map& get_buffer_map() const;

	/**
	 * \fn prepare_for_render
	 * \brief Sets up the buffer for rendering.
	 *
	 * Sets up the buffer for rendering by binding the buffer and setting up the vertex attribute pointers so that the data can be passed to the shader 
	 * program. Used for making so that a single call can be made prior to a glDraw call.
	 */
	void prepare_for_render() const;

private:
	/**
	 * \fn init_buffer
	 * \brief Initializes the buffer.
	 *
	 * Initializes the buffer by generating the OpenGL buffer object and checking to make sure no errors occured in the generation of that object.
	 */
	void init_buffer();
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
