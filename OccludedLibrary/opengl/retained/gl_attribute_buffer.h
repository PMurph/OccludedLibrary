#pragma once

#include <GL\glew.h>

#include "../../buffers/attribute_buffer_factory.h"

namespace occluded { namespace opengl { namespace retained {

typedef enum BUFFER_USAGE {
	static_draw_usage = GL_STATIC_DRAW,
	stream_draw_usage = GL_STREAM_DRAW,
	dynamic_draw_usage = GL_DYNAMIC_DRAW
} buffer_usage_t;

class gl_attribute_buffer
{
private:
	GLuint m_id;
	std::auto_ptr<buffers::attribute_buffer> m_buffer;
	buffer_usage_t m_usage;

public:
	gl_attribute_buffer( const buffers::attributes::attribute_map& map, const buffer_usage_t usage );
	~gl_attribute_buffer();

	void bind_buffer();
	
	void insert_values( const std::vector<char>& values );

	const GLuint get_id() const;
private:
	void init_buffer();
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace
