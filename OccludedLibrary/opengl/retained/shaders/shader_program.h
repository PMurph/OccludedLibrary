#pragma once

#include <GL/glew.h>

#include "shader.h"

namespace occluded { namespace opengl { namespace retained { namespace shaders {

class shader_program
{
private:
	GLuint m_id;
	
	bool m_linked;

	std::vector<shader> m_shaders;

	std::string m_errorLog;

public:
	shader_program( const std::string& vertShaderSrc, const std::string& tessControlShaderSrc, const std::string& tessEvalShaderSrc,
		const std::string& geoShaderSrc, const std::string& fragShaderSrc, const std::string& computeShaderSrc );
	~shader_program();

	void link_shaders();

	void use_program();

	const GLuint get_id() const;

	const bool is_linked() const;

	const std::string& get_error_log() const;

private:
	void init_shaders( const std::string& vertShaderSrc, const std::string& tessControlShaderSrc, const std::string& tessEvalShaderSrc,
		const std::string& geoShaderSrc, const std::string& fragShaderSrc, const std::string& computerShaderSrc );

	void init_vert_shaders( const std::string& vertShaderSrc );

	void init_tess_control_shader( const std::string& tessControlShaderSrc );

	void init_tess_eval_shader( const std::string& tessEvalShaderSrc );

	void init_geo_shader( const std::string& geoShaderSrc );

	void init_frag_shader( const std::string& fragShadersrc );

	void init_comp_shader( const std::string& computeShaderSrc );

	void attach_shaders();

	void handle_link_errors();
};

} // end of shaders namespace
} // end of retained namespace
} // end of opengl namespace

typedef opengl::retained::shaders::shader_program shader_program;

} // end of occluded namespace
