#include "main.h"

static GLint projMatPtr, viewMatPtr;
static SDL_Window* win = NULL;
static GLuint vao;

static void init_SDL_window();
static void init_opengl_context_attributes();
static void init_opengl( SDL_GLContext& glContext );
static void init_shader_program( std::auto_ptr<const occluded::shader_program>& shaderProg );
static void init_boxes( std::vector<box>& boxes, const std::auto_ptr<const occluded::shader_program>& shaderProg );
static void init_box_map( occluded::buffers::attributes::attribute_map& map );
static void program_loop( std::vector<box>& boxes );
static void process_input();
static void draw_boxes( std::vector<box>& boxes );
static void update_boxes( std::vector<box>& boxes );

int main( int argc, char** argv ) {
	SDL_GLContext ctxt;
	std::auto_ptr<const occluded::shader_program> shaderProg;
	std::vector<box> boxes;

	init_SDL_window();
	init_opengl( ctxt );
	init_shader_program( shaderProg );
	init_boxes( boxes, shaderProg );

	running = true;

	program_loop( boxes );

	SDL_GL_DeleteContext( ctxt );
	SDL_DestroyWindow( win );

	win = NULL;

	return 0;
}

void program_loop( std::vector<box>& boxes ) {
	while( running ) {
		// All code goes in here
		update_boxes( boxes );
		draw_boxes( boxes );
		process_input();
	}
}

// Initialization Functions

void init_SDL_window() {
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) != 0 ) {
		exit( -1 );
	}
	
	init_opengl_context_attributes();

	win = SDL_CreateWindow( WINDOW_TITLE.c_str(), INIT_X, INIT_Y, INIT_W, INIT_H, SDL_WINDOW_OPENGL );

	if( win == NULL ) {
		exit( -1 );
	}
}

void init_opengl_context_attributes() {
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
}

void init_opengl( SDL_GLContext& glContext ) {
	glContext = SDL_GL_CreateContext( win );

	glewExperimental = GL_TRUE;

	if( glewInit() != GLEW_OK ) {
		exit(-1);
	}

	assert( GL_NO_ERROR == glGetError() );

	glClearColor( 0.1f, 0.1f, 0.1f, 0.1f );
	assert( GL_NO_ERROR == glGetError() );

	glEnableClientState( GL_VERTEX_ARRAY );
	assert( GL_NO_ERROR == glGetError() );

	glEnable( GL_CULL_FACE );
	assert( GL_NO_ERROR == glGetError() );

	glEnable( GL_DEPTH_TEST );
	assert( GL_NO_ERROR == glGetError() );

	glCullFace( GL_BACK );
	assert( GL_NO_ERROR == glGetError() );

	glFrontFace( GL_CCW );
	assert( GL_NO_ERROR == glGetError() );

	glDepthFunc( GL_LESS );
	assert( GL_NO_ERROR == glGetError() );
}

void init_shader_program( std::auto_ptr<const occluded::shader_program>& shaderProg ) {
	const std::string vertShaderSrc( occluded::utilities::files::file_reader::get_string_from_file( VERTEX_SHADER_PATH ) );
	const std::string fragShaderSrc( occluded::utilities::files::file_reader::get_string_from_file( FRAG_SHADER_PATH ) );
	std::vector< const boost::shared_ptr<const occluded::shader> > shaders;
	boost::shared_ptr<const occluded::shader> vertShader( new occluded::shader( vertShaderSrc, occluded::opengl::retained::shaders::vert_shader ) );
	boost::shared_ptr<const occluded::shader> fragShader( new occluded::shader( fragShaderSrc, occluded::opengl::retained::shaders::frag_shader ) );

	shaders.push_back( vertShader );
	shaders.push_back( fragShader );

	shaderProg.reset( new occluded::shader_program( shaders ) );

	assert( GL_NO_ERROR == glGetError() );

	shaderProg->use_program();

	projMatPtr = glGetUniformLocation( shaderProg->get_id(), "vProjection" );
	assert( GL_NO_ERROR == glGetError() );
	glUniformMatrix4fv( projMatPtr, 1, GL_FALSE, glm::value_ptr( PROJECTION_MATRIX ) );
	assert( GL_NO_ERROR == glGetError() );
	
	viewMatPtr = glGetUniformLocation( shaderProg->get_id(), "vView" );
	assert( GL_NO_ERROR == glGetError() );
	glUniformMatrix4fv( viewMatPtr, 1, GL_FALSE, glm::value_ptr( VIEW_MATRIX ) );

	glBindFragDataLocation( shaderProg->get_id(), 0, "finalColor" );
	assert( GL_NO_ERROR == glGetError() );
}

void init_boxes( std::vector<box>& boxes, const std::auto_ptr<const occluded::shader_program>& shaderProg ) {
	occluded::buffers::attributes::attribute_map boxMap( true );
	init_box_map( boxMap );

	glGenVertexArrays( 1, &vao );
	assert( GL_NO_ERROR == glGetError() );

	glBindVertexArray( vao );
	assert( GL_NO_ERROR == glGetError() );

	for( int i = 0; i < 5; ++i ) {
		boxes.push_back( box( *shaderProg, boxMap, vao ) );
	}

	boxes[0].set_pos( 2.f, 0.f, 0.f );
	boxes[1].set_pos( 0.f, 2.f, 0.f );
	boxes[2].set_pos( -2.f, 0.f, 0.f );
	boxes[3].set_pos( 0.f, -2.f, 0.f );
	boxes[4].set_pos( 0.f, 0.f, 0.f );
}

void init_box_map( occluded::buffers::attributes::attribute_map& map ) {
	map.add_attribute( occluded::buffers::attributes::attribute( "position", 3, occluded::buffers::attributes::attrib_float ) );
	map.add_attribute( occluded::buffers::attributes::attribute( "color", 3, occluded::buffers::attributes::attrib_float, true ) );
	map.end_definition();
}

void process_input() {
	SDL_Event windowEvent;

	if( SDL_PollEvent( &windowEvent ) == 1 ) {
		if( windowEvent.type == SDL_QUIT ) {
			running = false;
		}
	}
}

void draw_boxes( std::vector<box>& boxes ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	assert( GL_NO_ERROR == glGetError() );

	for( std::vector<box>::iterator it = boxes.begin(); it != boxes.end(); ++it ) {
		it->draw();
	}

	SDL_GL_SwapWindow(win);
}

void update_boxes( std::vector<box>& boxes ) {
	float DELTA_ANGLE = 0.01f;

	for( std::vector<box>::iterator it = boxes.begin(); it != boxes.end(); ++it ) {
		it->rotate( 0.01f );
	}
}