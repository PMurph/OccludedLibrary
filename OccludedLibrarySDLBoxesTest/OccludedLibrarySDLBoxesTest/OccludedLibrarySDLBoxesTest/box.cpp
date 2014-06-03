#include "box.h"

// Static Constants

const float box::TWO_PI = 2.f * 3.14159f;

// Public Member Functions

box::box( const occluded::shader_program& shaderProg, const occluded::buffers::attributes::attribute_map& map, GLuint& vao ):
	m_shaderProg( shaderProg ),
	m_buffer( map, occluded::opengl::retained::static_draw_usage ),
	m_indices( 0 ),
	m_posX( 0.f ),
	m_posY( 0.f ),
	m_posZ( 0.f ),
	m_angle( 0.f ),
	m_vao( vao )
{
	init_box();
}


box::~box()
{
}

void box::set_pos( const float posX, const float posY, const float posZ ) {
	m_posX = posX;
	m_posY = posY;
	m_posZ = posZ;
}

void box::draw() {
	m_shaderProg.use_program();
	assert( GL_NO_ERROR == glGetError() );

	set_model_value();

	m_buffer.bind_buffer();
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elemBufferId );
	assert( GL_NO_ERROR == glGetError() );

	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW );
	assert( GL_NO_ERROR == glGetError() );
	
	set_attrib_pointers();

	glBindVertexArray( m_vao );
	assert( GL_NO_ERROR == glGetError() );

	glDrawElements( GL_TRIANGLES, static_cast<GLsizei>( m_indices.size() ) , GL_UNSIGNED_INT, 0 );
	assert( GL_NO_ERROR == glGetError() );
}

void box::rotate( const float angle ) {
	m_angle += angle;

	if( angle >= TWO_PI ) {
		m_angle = m_angle - TWO_PI;
	}
}

// Private Memmber Functions

void box::init_box() {
	populate_box_buffer( m_buffer );
	populate_box_indices( m_indices );
	setup_element_buffer();
	setup_attrib_pointers();
	setup_uniforms();
}

void box::setup_element_buffer() {
	glGenBuffers( 1, &m_elemBufferId );
	assert( GL_NO_ERROR == glGetError() );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elemBufferId );
	assert( GL_NO_ERROR == glGetError() );
}

void box::setup_attrib_pointers() {
	GLint posPtr, colorPtr;

	m_shaderProg.use_program();

	m_buffer.bind_buffer();

	posPtr = glGetAttribLocation( m_shaderProg.get_id(), "vPosition" );
	assert( GL_NO_ERROR == glGetError() );
	m_posAttribPtr = static_cast<GLuint>( posPtr );

	colorPtr = glGetAttribLocation( m_shaderProg.get_id(), "vColor" );
	assert( GL_NO_ERROR == glGetError() );
	m_colorAttribPtr = static_cast<GLuint>( colorPtr );
	
	glEnableVertexAttribArray( m_posAttribPtr );
	assert( GL_NO_ERROR == glGetError() );

	glEnableVertexAttribArray( m_colorAttribPtr );
	assert( GL_NO_ERROR == glGetError() );
}

void box::setup_uniforms() {
	GLint modelPtr;

	modelPtr = glGetUniformLocation( m_shaderProg.get_id(), "vModel" );
	assert( GL_NO_ERROR == glGetError() );
	m_modelUniformPtr = static_cast<GLuint>( modelPtr );
}

void box::set_model_value() {
	glm::mat4 modelMatrix( 1.f );

	modelMatrix = glm::rotate( modelMatrix, m_angle, glm::vec3( 0.f, 1.f, 0.f ) );
	modelMatrix = glm::translate( modelMatrix, glm::vec3( m_posX, m_posY, m_posZ ) );
	modelMatrix = glm::rotate( modelMatrix, m_angle, glm::vec3( 1.f, 0.f, 0.f ) );

	glUniformMatrix4fv( m_modelUniformPtr, 1, GL_FALSE, glm::value_ptr( modelMatrix ) );
	assert( GL_NO_ERROR == glGetError() );
}


void box::set_attrib_pointers() {
	glVertexAttribPointer( m_posAttribPtr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)0 );
	assert( GL_NO_ERROR == glGetError() );

	glVertexAttribPointer( m_colorAttribPtr, 3, GL_FLOAT, GL_TRUE, 6 * sizeof( float ),  (void*)( 3 * sizeof( float ) ) );
	assert( GL_NO_ERROR == glGetError() );
}

// Private Static Functions

void box::populate_box_buffer( occluded::opengl::retained::gl_attribute_buffer& buffer ) {
	std::vector<char> data( buffer.get_buffer_map().get_byte_size() );
	float vertexX, vertexY, vertexZ;
	float colorR, colorG, colorB;

	// comments assume x is towards you, y is to your right, and z is up

	//top-front-right vertex
	vertexX = 0.5f; vertexY = 0.5f; vertexZ = 0.5f;
	colorR = 0.0f; colorG = 0.0f; colorB = 0.0f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//top-back-right vertex
	vertexX = -0.5f; vertexY = 0.5f; vertexZ = 0.5f;
	colorR = 0.0f; colorG = 0.0f; colorB = 1.0f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//top-back-left vertex
	vertexX = -0.5f; vertexY = -0.5f; vertexZ = 0.5f;
	colorR = 0.0f; colorG = 1.0f; colorB = 0.0f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//top-front-left vertex
	vertexX = 0.5f; vertexY = -0.5f; vertexZ = 0.5f;
	colorR = 0.0f; colorG = 0.5f; colorB = 0.5f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//bottom-front-right vertex
	vertexX = 0.5f; vertexY = 0.5f; vertexZ = -0.5f;
	colorR = 1.0f; colorG = 0.0f; colorB = 0.0f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//bottom-back-right vertex
	vertexX = -0.5f; vertexY = 0.5f; vertexZ = -0.5f;
	colorR = 0.5f; colorG = 0.0f; colorB = 0.5f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//bottom-back-left vertex
	vertexX = -0.5f; vertexY = -0.5f; vertexZ = -0.5f;
	colorR = 0.5f; colorG = 0.5f; colorB = 0.0f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );

	//bottom-front-left vertex
	vertexX = 0.5f; vertexY = -0.5f; vertexZ = -0.5f;
	colorR = 0.333f; colorG = 0.333f; colorB = 0.333f;

	place_vertex_in_buffer( buffer, vertexX, vertexY, vertexZ, colorR, colorG, colorB );
}

void box::place_vertex_in_buffer( occluded::opengl::retained::gl_attribute_buffer& buffer, float vertX, float vertY, float vertZ, float colR, float colG, float colB ) {
	std::vector<char> data( buffer.get_buffer_map().get_byte_size() );

	memcpy( &data[0], &vertX, sizeof( float ) );
	memcpy( &data[sizeof(float)], &vertY, sizeof( float ) );
	memcpy( &data[2*sizeof(float)], &vertZ, sizeof( float ) );
	memcpy( &data[3*sizeof(float)], &colR, sizeof( float ) );
	memcpy( &data[4*sizeof(float)], &colG, sizeof( float ) );
	memcpy( &data[5*sizeof(float)], &colB, sizeof( float ) );

	buffer.insert_values( data );
}

void box::populate_box_indices( std::vector<unsigned int>& indices ) {
	// top faces
	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 3 );

	indices.push_back( 1 );
	indices.push_back( 2 );
	indices.push_back( 3 );

	// front faces
	indices.push_back( 4 );
	indices.push_back( 0 );
	indices.push_back( 7 );

	indices.push_back( 0 );
	indices.push_back( 3 );
	indices.push_back( 7 );

	// right faces
	indices.push_back( 5 );
	indices.push_back( 1 );
	indices.push_back( 0 );

	indices.push_back( 0 );
	indices.push_back( 4 );
	indices.push_back( 5 );

	// back faces
	indices.push_back( 6 );
	indices.push_back( 2 );
	indices.push_back( 1 );

	indices.push_back( 1 );
	indices.push_back( 5 );
	indices.push_back( 6 );

	// left faces
	indices.push_back( 7 );
	indices.push_back( 3 );
	indices.push_back( 2 );

	indices.push_back( 2 );
	indices.push_back( 6 );
	indices.push_back( 7 );

	// bottom faces
	indices.push_back( 7 );
	indices.push_back( 6 );
	indices.push_back( 5 );

	indices.push_back( 5 );
	indices.push_back( 4 );
	indices.push_back( 7 );
}