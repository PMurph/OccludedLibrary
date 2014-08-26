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

namespace shaders {

/**
 * \enum shader_type_t
 * \brief An enum that stores the type of shader.
 *
 * An enum representing the type of shader. It is used to prevent the passing of an invalid shader type to the constructor.
 */
typedef enum SHADER_TYPE {
	vert_shader = GL_VERTEX_SHADER,
	tess_control_shader = GL_TESS_CONTROL_SHADER,
	tess_eval_shader = GL_TESS_EVALUATION_SHADER,
	geo_shader = GL_GEOMETRY_SHADER,
	frag_shader = GL_FRAGMENT_SHADER,
	compute_shader = GL_COMPUTE_SHADER
} shader_type_t;

} // end of shader namespace

/**
 * \class gl_retained_object_manager
 * \brief 
 */
class gl_retained_object_manager
{
private:
	static gl_retained_object_manager object_manager;

	std::map<const GLuint, unsigned int> m_vaoRefCount;
	std::map< const std::pair<const GLuint, const GLuint>, unsigned int > m_vboRefCount;
	std::map<const GLuint, unsigned int> m_shaderRefCount;

public:

	/**
	 * \fn delete_objects
	 * \brief Deletes all the OpenGL objects managed by the object manager.
	 */
	void delete_objects();

	// ==== Vertex Array Object Functions ====

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

	// ==== Vertex Buffer Object functions ====

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

	// ==== Shader Functions ====

	/**
	 * \fn get_new_shader
	 * \brief Creates a new shader object and gets its id
	 *
	 * \param A constant shader_type_t representing the type of the shader to be created.
	 * \return A constant GLuint representing the id of the newly created shader.
	 *
	 * Creates a new OpenGL shader object and returns the id representing that object. Throws an exception if an error occurs with OpenGL.
	 */
	const GLuint get_new_shader( const shaders::shader_type_t shaderType );

	/**
	 * \fn add_ref_to_shader
	 * \brief Adds a reference a shader.
	 *
	 * \param shaderId A constant GLuint representing the id of the shader.
	 *
	 * Increments the reference count to the OpenGL shader corresponding to the shaderId paramater. Throws an exception if the shaderId parameter does
	 * not correspond to a valid OpenGL shader object.
	 */
	void add_ref_to_shader( const GLuint shaderId );

	/**
	 * \fn remove_ref_to_shader
	 * \brief Removes a reference to a shader.
	 *
	 * \param shaderId A constant GLuint representing the id of the shader.
	 *
	 * Decrements the reference count to an OpenGL shader object associated the shaderId paramater. If the reference count of the shader is reduced to
	 * 0 the shader object will be deleted so memory can be freed. Throws an exception if the shaderId does not correspond to a valid OpenGL shader 
	 * object.
	 */
	void remove_ref_to_shader( const GLuint shaderId );

	/**
	 * \fn check_valid_shader_id
	 * \brief Check to see if a shader id corresponds to a valid shader.
	 *
	 * \param shaderId A constant GLuint representing the id of the shader.
	 * \return True if the shader id corresponds to a valid shader and false otherwise.
	 *
	 * Checks to see if a valid OpenGL shader object corresponds to the id passed via the shaderId parameter. Throws an exception if 0 is passed as the
	 * shader id.
	 */
	const bool check_valid_shader_id( const GLuint shaderId ) const;

	// ==== Shader Program Functions ====

	const GLuint get_new_shader_prog();
	void add_ref_to_shader_prog( const GLuint shaderProgId );
	void remove_ref_to_shader_prog( const GLuint shaderProgId );
	const bool check_valid_shader_prog_id( const GLuint shaderProgId ) const;

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
	 * \fn delete_vaos
	 * \brief Deletes all the vaos managed.
	 *
	 * Deletes all the OpenGL vertex array objects monitored by the object manager.
	 */
	void delete_vaos();

	/**
	 * \fn delete_vbos
	 * \brief Deletes all the vbos managed.
	 *
	 * Deletes all the OpenGL vertex buffer objects monitored by the object manager.
	 */
	void delete_vbos();

	/**
	 * \fn delete_shaders
	 * \brief Deletes all the shaders managed.
	 *
	 * Deletes all the OpenGL shader objects monitored by the object manager.
	 */
	void delete_shaders();

	/**
	 * \fn inc_vao_entry
	 * \brief Increments the vao's ref count by 1.
	 *
	 * \param vaoId A constant GLuint representing the id of the entry to increment.
	 */
	void inc_vao_entry( const GLuint vaoId );

	/**
	 * \fn dec_vao_entry
	 * \brief Decrements the vao's ref count by 1.
	 *
	 * \param vaoId A constant GLuint representing the id of the entry to decrement.
	 *
	 * Decrements the vao specified by the vaoId parameter ref count by 1 and deletes the vao if the reference count reaches 0. Throws an exception 
	 * if the entry corresponding to the vaoId parameter is 0.
	 */
	void dec_vao_entry( const GLuint vaoId );

	/**
	 * \fn inc_vbo_entry
	 * \brief Increments a vbo's ref count by 1.
	 *
	 * \param key A reference to a constant pair of constant GLuints representing the vao id and vbo id that the reference will be added to.
	 */
	void inc_vbo_entry( const std::pair<const GLuint, const GLuint>& key );

	/**
	 * \fn dec_vbo_entry
	 * \brief Decrements the vbo's ref count by 1.
	 *
	 * \param key A reference to a constant pair of constant GLuints representing the vao id and vbo id that the reference will be removed from.
	 *
	 * Decrements the vbo ref count associated with vaoId and vboId parameters by 1 and deletes the vbo if the reference count reaches 0. Throws 
	 * an exception if the ref count is equal to 0.
	 */
	void dec_vbo_entry( const std::pair<const GLuint, const GLuint>& key );

	/**
	 * \fn inc_shader_entry
	 * \brief Increments a shader's ref count by 1.
	 *
	 * \param shaderId A constant GLuint representing the id of the shader.
	 */
	void inc_shader_entry( const GLuint shaderId );

	/**
	 * \fn dec_shader_entry
	 * \brief Decrements a shader's ref count by 1.
	 *
	 * \param shaderId A constant GLuint representng the id of the shader.
	 *
	 * Decrements the shader's reference count associated with the shader id by 1 and deletes the shader if the reference count reaches 0. Throws an
	 * exception if the reference count already equals 0.
	 */
	void dec_shader_entry( const GLuint shaderId );
};

} // end of retained namespace
} // end of opengl namespace
} // end of occluded namespace

