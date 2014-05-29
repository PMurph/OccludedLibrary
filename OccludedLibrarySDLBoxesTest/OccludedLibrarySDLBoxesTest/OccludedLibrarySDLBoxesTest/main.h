#pragma once

#include <string>

#include <GL/glew.h>
#include <SDL.h>

#include "utilities/files/file_reader.h"
#include "box.h"

const int INIT_X = 0, INIT_Y = 0, INIT_W = 640, INIT_H = 480;
const std::string WINDOW_TITLE( "Occluded Library SDL Boxes Test v0.1" );
const std::string VERTEX_SHADER_PATH( "shaders/vertex_shader.glsl");
const std::string FRAG_SHADER_PATH( "shaders/frag_shader.glsl" );

bool running = false;