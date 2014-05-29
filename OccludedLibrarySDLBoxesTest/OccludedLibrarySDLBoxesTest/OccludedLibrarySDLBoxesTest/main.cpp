#include "main.h"

static void init_SDL_window( SDL_Window* window );
static void init_opengl_context_attributes();
static void init_opengl( SDL_Window* window, SDL_GLContext& glContext );
static void init_shader_program();
static void program_loop();

int main( int argc, char** argv ) {
	SDL_Window* win = NULL;
	SDL_GLContext ctxt;

	init_SDL_window( win );
	init_opengl( win, ctxt );

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