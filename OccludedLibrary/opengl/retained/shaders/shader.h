#pragma once

#include <cassert>
#include <string>
#include <vector>

#include  <GL/glew.h>

namespace occluded { 

namespace opengl { namespace retained { namespace shaders {

typedef enum SHADER_TYPE {
	vert_shader = GL_VERTEX_SHADER,
	tess_control_shader = GL_TESS_CONTROL_SHADER,
	tess_eval_shader = GL_TESS_EVALUATION_SHADER,
	geo_shader = GL_GEOMETRY_SHADER,
	frag_shader = GL_FRAGMENT_SHADER,
	compute_shader = GL_COMPUTE_SHADER,
	invalid_shader = 0
} shader_type_t;

typedef enum SHADER_STATUS {
	shader_initialized,
	shader_compiled,
	shader_error
} shader_status_t;

class shader 
{
private:
	std::string m_shaderSrc;
	shader_type_t m_type;

	GLuint m_id;
	shader_status_t m_status;

	std::string m_compileLog;

public:
	shader( const std::string& shaderSrc, const shader_type_t type );
	~shader();

	const GLuint get_id() const;

	const bool is_compiled() const;

	const shader_type_t get_type() const;

	const std::string& get_compile_log() const;

private:
	void compile_shader();

	void handle_compile_error();
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace

typedef opengl::retained::shaders::shader shader;

} // end of occluded namespace
