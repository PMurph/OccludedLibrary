#pragma once

#include <string>

#include <GL/glew.h>
#include <SDL.h>

#include "utilities/files/file_reader.h"
#include "box.h"

#include <glm/gtc/matrix_transform.hpp>

const int INIT_X = 50, INIT_Y = 50, INIT_W = 640, INIT_H = 480;
const std::string WINDOW_TITLE( "Occluded Library SDL Boxes Test v0.1" );
const std::string VERTEX_SHADER_PATH( "./shaders/vertex_shader.glsl");
const std::string FRAG_SHADER_PATH( "./shaders/fragment_shader.glsl" );
const glm::mat4 PROJECTION_MATRIX = glm::perspectiveFov( 1.047f, static_cast<float>( INIT_W ), static_cast<float>( INIT_H ), 1.f, 25.f );
const glm::mat4 VIEW_MATRIX = glm::mat4( 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, -10.f, 1.f );
//const glm::mat4 PROJECTION_MATRIX = glm::mat4( 1.f );

bool running = false;