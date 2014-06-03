#pragma once

#include <GL/glew.h>

#include "shader.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

class shader_program
{
private:
	GLuint m_id;
	bool m_linked;
	std::string m_errorLog;

public:
	shader_program();
	shader_program( const std::vector<shader>& shaders );
	~shader_program();

	void use_program() const;

	const GLuint get_id() const;

	const bool is_linked() const;

	const std::string& get_error_log() const;

private:
	void init_shader_program( const std::vector<shader>& shaders );

	void link_shaders();

	void attach_shaders( const std::vector<shader>& shaders );

	void handle_link_errors();
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace

typedef opengl::retained::shaders::shader_program shader_program;

} // end of occluded namespace
