#pragma once

#include <GL\glew.h>

#include "..\..\buffers\attribute_buffer.h"

namespace occluded { namespace opengl { namespace retained {

typedef enum BUFFER_TARGET {
	target_array_buffer = GL_ARRAY_BUFFER,
	target_element_array_buffer = GL_ELEMENT_ARRAY_BUFFER,
	target_pixel_pack_buffer = GL_PIXEL_PACK_BUFFER,
	target_pixel_unpack_buffer = GL_PIXEL_UNPACK_BUFFER
	target_invalid
} buffer_target_t;

class gl_attribute_buffer
{
private:
	GLuint m_id;
	buffers::attribute_buffer m_buffer;

public:
	gl_attribute_buffer( const buffers::attributes::attribute_map& map );
	~gl_attribute_buffer();

	void bind_buffer();
	void setup_data_store();

	const GLuint get_id() const;

private:
	void init_buffer();
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
