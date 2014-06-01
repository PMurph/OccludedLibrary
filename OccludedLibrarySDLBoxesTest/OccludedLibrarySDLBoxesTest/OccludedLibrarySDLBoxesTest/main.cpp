#include "main.h"

static void init_SDL_window( SDL_Window* window );
static void init_opengl_context_attributes();
static void init_opengl( SDL_Window* window, SDL_GLContext& glContext );
static void init_shader_program( occluded::shader_program& shaderProg );
static void init_boxes( std::vector<box>& boxes, const occluded::shader_program& shaderProg );
static void init_box_map( occluded::buffers::attributes::attribute_map& map );
static void program_loop();

int main( int argc, char** argv ) {
	SDL_Window* win = NULL;
	SDL_GLContext ctxt;
	occluded::shader_program shaderProg;
	std::vector<box> boxes;

	init_SDL_window( win );
	init_opengl( win, ctxt );
	init_shader_program( shaderProg );
	init_boxes( boxes, shaderProg );

	running = true;

	program_loop();

	SDL_GL_DeleteContext( ctxt );
	SDL_DestroyWindow( win );

	win = NULL;

	return 0;
}

void program_loop() {
	while( running ) {
		// All code goes in here
	}
}

// Initialization Functions

void init_SDL_window( SDL_Window* window ) {
	if( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) ) {
		exit( -1 );
	}
	
	init_opengl_context_attributes();

	window = SDL_CreateWindow( WINDOW_TITLE.c_str(), INIT_X, INIT_Y, INIT_W, INIT_H, SDL_WINDOW_OPENGL );

	if( window == NULL ) {
		exit( -1 );
	}
}

void init_opengl_context_attributes() {
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
}

void init_opengl( SDL_Window* window, SDL_GLContext& glContext ) {
	glContext = SDL_GL_CreateContext( window );

	glewExperimental = true;

	if( glewInit() != GLEW_OK ) {
		exit(-1);
	}
}

void init_shader_program( occluded::shader_program& shaderProg ) {
	const std::string vertShaderSrc( occluded::utilities::files::file_reader::get_string_from_file( VERTEX_SHADER_PATH ) );
	const std::string fragShaderSrc( occluded::utilities::files::file_reader::get_string_from_file( FRAG_SHADER_PATH ) );
	std::vector<occluded::shader> shaders;

	shaders.push_back( occluded::shader( vertShaderSrc, occluded::opengl::retained::shaders::vert_shader ) );
	shaders.push_back( occluded::shader( fragShaderSrc, occluded::opengl::retained::shaders::frag_shader ) );

	shaderProg = occluded::shader_program( shaders );
}

void init_boxes( std::vector<box>& boxes, const occluded::shader_program& shaderProg ) {
	occluded::buffers::attributes::attribute_map boxMap( true );
	init_box_map( boxMap );

	for( int i = 0; i < 5; ++i ) {
		boxes.push_back( box( shaderProg, boxMap ) );
	}


}

void init_box_map( occluded::buffers::attributes::attribute_map& map ) {
	map.add_attribute( occluded::buffers::attributes::attribute( "position", 3, occluded::buffers::attributes::attrib_float ) );
	map.add_attribute( occluded::buffers::attributes::attribute( "color", 3, occluded::buffers::attributes::attrib_float, true ) );
	map.end_definition();
}