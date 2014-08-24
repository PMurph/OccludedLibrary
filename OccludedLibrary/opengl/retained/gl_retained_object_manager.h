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
 * \class gl_retained_object_manager
 * \brief 
 */
class gl_retained_object_manager
{
private:
	static gl_retained_object_manager object_manager;

	std::map<const GLuint, unsigned int> m_vaoCount;
	std::map< const std::pair<const GLuint, const GLuint>, unsigned int > m_vboCount;

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
	 * \fn add_ref_to_vao
	 * \brief Adds a reference to a vao.
	 *
	 * \param vaoId A constant GLuint representing an id for a vao.
	 *
	 * Incremenets the reference count to the OpenGL vertex array object specified by the vaoId parameter. Throws an exception if there is no
	 * vao associated with the vaoId parameter.
	 */
	void add_ref_to_vao( const GLuint vaoId );

	/**
	 * \fn remove_ref_to_vao
	 * \brief Removes a reference to a vao.
	 *
	 * \param vaoId A constant GLuint representing an id for a vao.
	 *
	 * Decremenents the refrence count to the OpenGL vertex array object specified by the vaoId parameter. If the reference count is reduced to
	 * 0, the OpenGL vertex array object is removed so that memory can be freed. Throws an exception if there is no vao associated with the 
	 * vaoId parameter.
	 */
	void remove_ref_to_vao( const GLuint vaoId );

	/**
	 * \fn check_valid_vao_id
	 * \brief Checks to see if a vao id is corresponds to a valid vao.
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
	 * \fn add_ref_to_vbo
	 * \brief Adds a reference to a vbo.
	 *
	 * \param vaoId A constant GLuint that represents the vao's id that the vbo is associated with.
	 * \param vboId A constant GLuint that represents the vbo's id
	 *
	 * Incremenets the reference count to the OpenGL vertex buffer object specified by the vaoId and vboId parameters. Throws an exception 
	 * if there is no vao associated with the vaoId parameter. Throws an exception if there is no vbo associated with the vboId parameter.
	 */
	void add_ref_to_vbo( const GLuint vaoId, const GLuint vboId );

	/**
	 * \fn remove_ref_to_vbo
	 * \brief Removes a reference to a vbo.
	 *
	 * \param vaoId A constant GLuint that represents the vao's id that the vbo is associated with.
	 * \param vboId A constant GLuint that represents the vbo's id
	 *
	 * Decremenents the refrence count to the OpenGL vertex buffer object specified by the vaoId and vboId parameter. If the reference count is 
	 * reduced to 0, the OpenGL vertex buffer object is removed so that memory can be freed. Throws an exception if there is no vbo associated with the 
	 * vboId parameter. Throw an exception if there is no vbo associated with the vboId parameter.
	 */
	void remove_ref_to_vbo( const GLuint vaoId, const GLuint vboId );

	/**
	 * \fn check_valid_vbo_id
	 * \brief Checks to see if a vao id and vbo id corresponds to a valid vbo.
	 *
	 * \param vaoId A constant GLuint representing an id for a vao.
	 * \param vboId A constant GLuint representing an id for a vbo.
	 * \return True if the vba and vbo ids correspond to a valid vao and false if it is not.
	 *
	 * Checks to see if the OpenGL vertex buffer object associated with the vaoId and vboId parameters is a valid object. Throws an exception
	 * if either the vaoId or vboId parameters are 0.
	 */
	const bool check_valid_vbo_id( const GLuint vaoId, const GLuint vboId ) const;

	/**
	 * \fn get_state_manager
	 * \brief Gets a reference to the object manager object.
	 *
	 * \return A reference to the gl_retained_object_manager object.
	 *
	 * Gets a reference to the gl_retained_object_manager object that manages OpenGL objects.
	 */
	static gl_retained_object_manager& get_manager();

private:
	gl_retained_object_manager();
	~gl_retained_object_manager();
	gl_retained_object_manager( const gl_retained_object_manager& other ) { /* Disabled */ }

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
	 * Decrements the vao specified by the vaoId parameter ref count by 1. Throws an exception if the entry corresponding to the vaoId parameter is
	 * 0.
	 */
	void dec_vao_entry( const GLuint vaoId );

	/**
	 * \fn inc_vbo_entry
	 * \brief Increments a vbo ref count by 1.
	 *
	 * \param key A reference to a constant pair of constant GLuints representing the vao id and vbo id that the reference will be added to.
	 */
	void inc_vbo_entry( const std::pair<const GLuint, const GLuint>& key );

	/**
	 * \fn dec_vbo_entry
	 * \brief Decrements the vbo ref count by 1.
	 *
	 * \param key A reference to a constant pair of constant GLuints representing the vao id and vbo id that the reference will be removed from.
	 *
	 * Decrements the vbo ref count associated with vaoId and vboId parameters by 1. Throws an exception if the ref count is equal to 0.
	 */
	void dec_vbo_entry( const std::pair<const GLuint, const GLuint>& key );
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace

