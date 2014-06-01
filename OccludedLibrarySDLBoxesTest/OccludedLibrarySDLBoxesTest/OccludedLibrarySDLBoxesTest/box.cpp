#include "box.h"

// Public Member Functions

box::box( const occluded::shader_program& shaderProg, const occluded::buffers::attributes::attribute_map& map ):
	m_shaderProg( shaderProg ),
	m_buffer( map, occluded::opengl::retained::static_draw_usage ),
	m_indices( 0 ),
	m_posX( 0.0f ),
	m_posY( 0.0f ),
	m_posZ( 0.0f )
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

// Private Memmber Functions

void box::init_box() {
	populate_box_buffer( m_buffer );
	populate_box_indices( m_indices );
	setup_element_buffer();
}

void box::setup_element_buffer() {
	glGenBuffers( 1, &m_elemBufferId );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elemBufferId );

	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW );
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
	colorR = 0.333f; colorG = 0.333f; colorB = 0.333;

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

void box::populate_box_indices( std::vector<unsigned short>& indices ) {
	// top faces
	indices.push_back( static_cast<unsigned short> ( 0 ) );
	indices.push_back( static_cast<unsigned short>( 1 ) );
	indices.push_back( static_cast<unsigned short>( 3 ) );

	indices.push_back( static_cast<unsigned short>( 1 ) );
	indices.push_back( static_cast<unsigned short>( 2 ) );
	indices.push_back( static_cast<unsigned short>( 3 ) );

	// front faces
	indices.push_back( static_cast<unsigned short>( 4 ) );
	indices.push_back( static_cast<unsigned short>( 0 ) );
	indices.push_back( static_cast<unsigned short>( 7 ) );

	indices.push_back( static_cast<unsigned short>( 0 ) );
	indices.push_back( static_cast<unsigned short>( 3 ) );
	indices.push_back( static_cast<unsigned short>( 7 ) );

	// right faces
	indices.push_back( static_cast<unsigned short>( 5 ) );
	indices.push_back( static_cast<unsigned short>( 1 ) );
	indices.push_back( static_cast<unsigned short>( 0 ) );

	indices.push_back( static_cast<unsigned short>( 0 ) );
	indices.push_back( static_cast<unsigned short>( 4 ) );
	indices.push_back( static_cast<unsigned short>( 5 ) );

	// back faces
	indices.push_back( static_cast<unsigned short>( 6 ) );
	indices.push_back( static_cast<unsigned short>( 2 ) );
	indices.push_back( static_cast<unsigned short>( 1 ) );

	indices.push_back( static_cast<unsigned short>( 1 ) );
	indices.push_back( static_cast<unsigned short>( 5 ) );
	indices.push_back( static_cast<unsigned short>( 6 ) );

	// left faces
	indices.push_back( static_cast<unsigned short>( 7 ) );
	indices.push_back( static_cast<unsigned short>( 3 ) );
	indices.push_back( static_cast<unsigned short>( 2 ) );

	indices.push_back( static_cast<unsigned short>( 2 ) );
	indices.push_back( static_cast<unsigned short>( 6 ) );
	indices.push_back( static_cast<unsigned short>( 7 ) );

	// bottom faces
	indices.push_back( static_cast<unsigned short>( 7 ) );
	indices.push_back( static_cast<unsigned short>( 6 ) );
	indices.push_back( static_cast<unsigned short>( 5 ) );

	indices.push_back( static_cast<unsigned short>( 5 ) );
	indices.push_back( static_cast<unsigned short>( 4 ) );
	indices.push_back( static_cast<unsigned short>( 7 ) );
}