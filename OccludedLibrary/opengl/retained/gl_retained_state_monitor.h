#pragma once

#include <map>
#include <cassert>

#include <boost/lexical_cast.hpp>

#ifndef UNIT_TESTING
#include <GL\glew.h>
#else
#include "opengl_mock.h"
#endif

namespace occluded { namespace opengl { namespace retained {

/**
 * \class gl_retained_state_monitor
 * \brief 
 */
class gl_retained_state_monitor
{
private:
	static gl_retained_state_monitor state_monitor;

	std::map<GLuint, unsigned int> m_vaoCount;

public:

	/**
	 * \fn get_new_vao
	 * \brief Creates a new vao object and gets its id.
	 *
	 * \return A constant GLuint representing the new vao's id.
	 *
	 * Generates a new OpenGL vertex array object and returns its id. Throws an exception if OpenGL enters an error state when generating the object.
	 */
	const GLuint get_new_vao();

	/**
	 * \fn check_valid_vao_id
	 * \brief Checks to see if a vao id is corresponds to a valid vao
	 *
	 * \param vaoId A constant GLuint representing an id for a vao.
	 * \return True if the id corresponds to a valid vao and false if it is not.
	 *
	 * Checks to see if the OpenGL vertex array object associated with vaoId parameter is a valid object. Throws an exception if the vaoId parameter is 0.
	 */
	const bool check_valid_vao_id( const GLuint vaoId ) const;

	/**
	 * \fn get_new_vbo_ref
	 * \brief Gets a reference to a new vbo id.
	 *
	 * \param vaoId A constant GLuint that represent the vao's id that the vbo should be generated for.
	 * \return A constant GLuint representing the new vbo's id.
	 *
	 * Generate a new OpenGL vertex buffer object associated with the OpenGL vertex array object specified by the voaId parameter and returns a constant
	 * reference to the buffer object's id. Throws an exception if OpenGL enters an error state when generating the object. Throws an exception if there is
	 * no vertex array object corresponding to vaoId parameter. 
	 */
	const GLuint get_new_vbo( const GLuint vaoId );
	
	/**
	 * \fn inc_vao_ref
	 */
	//void inc_vao_ref( const GLuint& vaoId );
	
	/**
	 * \fn dec_vao_ref
	 */
	//void dec_vao_ref( const GLuint& vaoId );

	/**
	 * \fn inc_vbo_ref
	 */
	//void inc_vbo_ref( const GLuint& vaoId, const GLuint& vboId );

	/**
	 * \fn dec_vbo_ref
	 */
	//void dec_vbo_ref( const GLuint& vaoId, const GLuint& vboId );


	/**
	 * \fn get_state_monitor
	 * \brief Gets a reference to the state monitor object.
	 *
	 * \return A reference to the gl_retained_state_monitor object.
	 *
	 * Gets a reference to the gl_retained_state_monitor object that monitor and manipulate the opengl state machine.
	 */
	static gl_retained_state_monitor& get_monitor();

private:
	gl_retained_state_monitor();
	~gl_retained_state_monitor();

	/**
	 * \fn inc_vao_entry
	 * \brief Increments the vao id ref count by 1.
	 *
	 * \param vaoId A constant GLuint representing the id of the entry to increment.
	 */
	void inc_vao_entry( const GLuint vaoId );

	/**
	 * \fn dec_vao_entry
	 * \brief Decrements the vao id ref count by 1.
	 *
	 * \param vaoId A constant GLuint representing the id of the entry to decrement.
	 *
	 * Decrements the vao id specified by the vaoId parameter ref count by 1. Throws an exception if the entry corresponding to the vaoId parameter is
	 * 0. Throws an exception if the entry correpsonding to the vaoId parameter doesn't exist.
	 */
	void dec_vao_entry( const GLuint vaoId );

};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace

