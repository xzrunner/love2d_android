/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "PixelEffect.h"

#include "libraries/kazmath/include/kazmath/GL/matrix.h"

namespace
{
	// temporarily attaches a shader program (for setting uniforms, etc)
	// reattaches the originally active program when destroyed
	struct TemporaryAttacher
	{
		TemporaryAttacher(love::graphics::opengl::PixelEffect* sp) : s(sp)
		{
			glGetIntegerv(GL_CURRENT_PROGRAM, &activeProgram);
			s->attach();
		}
		~TemporaryAttacher() { glUseProgram(activeProgram); }
		love::graphics::opengl::PixelEffect* s;
		GLint activeProgram;
	};
} // anonymous namespace

namespace love
{
namespace graphics
{
namespace opengl
{
	PixelEffect * PixelEffect::current = NULL;

	std::map<std::string, GLint> PixelEffect::_texture_unit_pool;
	GLint PixelEffect::_current_texture_unit = 0;
	GLint PixelEffect::_max_texture_units = 0;

	GLint PixelEffect::getTextureUnit(const std::string& name)
	{
		std::map<std::string, GLint>::const_iterator it = _texture_unit_pool.find(name);

		if (it != _texture_unit_pool.end())
			return it->second;

		if (++_current_texture_unit >= _max_texture_units)
			throw love::Exception("No more texture units available");

		_texture_unit_pool[name] = _current_texture_unit;
		return _current_texture_unit;
	}

	PixelEffect::PixelEffect(const std::string& code)
		: _program(0), _code(code)
	{
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_max_texture_units);
		loadVolatile();
	}

	bool PixelEffect::loadVolatile()
	{
		_program = glCreateProgram();
		// should only fail if this is called between a glBegin()/glEnd() pair
		if (_program == 0)
			throw love::Exception("Cannot create shader program object.");

		GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
		// should only fail if this is called between a glBegin()/glEnd() pair
		if (shader == 0)
		{
			glDeleteProgram(_program);
			throw love::Exception("Cannot create shader object.");
		}

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		// vShader only fail if this is called between a glBegin()/glEnd() pair
		if (vShader == 0)
		{
			glDeleteProgram(_program);
			throw love::Exception("Cannot create shader object.");
		}

		// compile fragment shader code
		//std::string fCode = "								\
		//	precision mediump float;							\
		//	void main()											\
		//	{													\
		//		gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );		\
		//	}													\
		//	";
		const char* src = _code.c_str();
		GLint strlen = _code.length();
		glShaderSource(shader, 1, (const GLchar**)&src, &strlen);
		glCompileShader(shader);

		// compile vertices shader code
		std::string vCode = "				\
			precision mediump float;		\
			uniform mat4 PMatrix;			\
			uniform mat4 MVMatrix;			\
			uniform mat4 MVPMatrix;			\
											\
			attribute vec4 a_position;		\
			attribute vec2 a_texCoord;		\
			attribute vec4 a_color;			\
											\
			varying vec2 v_texCoord;		\
			varying vec4 v_fragmentColor;	\
											\
			void main()						\
			{								\
				gl_Position = MVPMatrix * a_position;	\
				v_fragmentColor = a_color / 255.0;		\
				v_texCoord = a_texCoord;				\
			}											\
			";
		
		// v_fragmentColor = a_color / 255.0;	
		// gl_FrontColor = gl_Color;

		const char* vSrc = vCode.c_str();
		GLint vStrlen = vCode.length();
		glShaderSource(vShader, 1, (const GLchar**)&vSrc, &vStrlen);
		glCompileShader(vShader);

		GLint compile_ok;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
		if (GL_FALSE == compile_ok)
		{
			// get compiler error
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &strlen);
			char *error_str = new char[strlen];
			glGetShaderInfoLog(shader, strlen, NULL, error_str);
			std::string tmp(error_str);

			// cleanup before throw
			delete[] error_str;
			glDeleteShader(shader);
			glDeleteProgram(_program);

			// XXX: errorlog may contain escape sequences.
			throw love::Exception("Cannot compile shader:\n%s", tmp.c_str());
		}

		glGetShaderiv(vShader, GL_COMPILE_STATUS, &compile_ok);
		if (GL_FALSE == compile_ok)
		{
			// get compiler error
			glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &strlen);
			char *error_str = new char[strlen];
			glGetShaderInfoLog(vShader, strlen, NULL, error_str);
			std::string tmp(error_str);

			// cleanup before throw
			delete[] error_str;
			glDeleteShader(vShader);
			glDeleteProgram(_program);

			// XXX: errorlog may contain escape sequences.
			throw love::Exception("Cannot compile shader:\n%s", tmp.c_str());
		}

		// link fragment shader
		GLint link_ok;
		glAttachShader(_program, shader);
		glAttachShader(_program, vShader);
		glLinkProgram(_program);
		glGetProgramiv(_program, GL_LINK_STATUS, &link_ok);
		if (GL_FALSE == link_ok)
		{
			// this should not happen if compiling is ok, but one can never be too careful
			// get linker error
			std::string tmp(getWarnings());

			// cleanup before throw
			glDeleteShader(shader);
			glDeleteShader(vShader);
			glDeleteProgram(_program);
			throw love::Exception("Cannot compile shader:\n%s", tmp.c_str());
		}

		glDeleteShader(shader);
		glDeleteShader(vShader);

		//////////////////////////////////////////////////////////////////////////
		glBindAttribLocation(_program, e_VertexAttrib_Position, AttributeNamePosition);
 		glBindAttribLocation(_program, e_VertexAttrib_TexCoords, AttributeNameTexCoord);
 		glBindAttribLocation(_program, e_VertexAttrib_Color, AttributeNameColor);

		m_uniforms[e_UniformPMatrix] = glGetUniformLocation(_program, UniformPMatrix);
		m_uniforms[e_UniformMVMatrix] = glGetUniformLocation(_program, UniformMVMatrix);
		m_uniforms[e_UniformMVPMatrix] = glGetUniformLocation(_program, UniformMVPMatrix);

		m_uniforms[e_UniformSampler] = glGetUniformLocation(_program, "_tex0_");
		attach();
		glUniform1i( (GLint)m_uniforms[e_UniformSampler], 0);
		//////////////////////////////////////////////////////////////////////////		

		return true;
	}

	PixelEffect::~PixelEffect()
	{
		unloadVolatile();
	}

	void PixelEffect::unloadVolatile()
	{
		glDeleteProgram(_program);
	}

// 	std::string PixelEffect::getGLSLVersion()
// 	{
// 		// GL_SHADING_LANGUAGE_VERSION is not available in OpenGL < 2.1.
// 		// Be very pessimistic about the GLSL version in that case.
// 		if (!GLEE_VERSION_2_1)
// 			return "0.0";
// 
// 		// the version string always begins with a version number of the format
// 		//   major_number.minor_number
// 		// or
// 		//   major_number.minor_number.release_number
// 		std::string versionString((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
// 		size_t minorEndPos = versionString.find(" ");
// 		return versionString.substr(0, minorEndPos);
// 	}


// 	bool PixelEffect::isSupported()
// 	{
// 		return GLEE_VERSION_2_0 && GLEE_ARB_shader_objects && GLEE_ARB_fragment_shader && getGLSLVersion() >= "1.2";
// 	}

	std::string PixelEffect::getWarnings() const
	{
		GLint strlen, nullpos;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &strlen);
		char *temp_str = new char[strlen+1];
		// be extra sure that the error string will be 0-terminated
		memset(temp_str, '\0', strlen+1);
		glGetProgramInfoLog(_program, strlen, &nullpos, temp_str);
		temp_str[nullpos] = '\0';

		std::string warnings(temp_str);
		delete[] temp_str;
		return warnings;
	}

	void PixelEffect::attach()
	{
		glUseProgram(_program);
		current = this;

		setUniformMatrix();
	}

	void PixelEffect::detach()
	{
		glUseProgram(0);
		current = NULL;
	}

	void PixelEffect::sendFloat(const std::string& name, int size, const GLfloat* vec, int count)
	{
		TemporaryAttacher attacher(this);
		GLint location = getUniformLocation(name);

		if (size < 1 || size > 4)
		{
			throw love::Exception("Invalid variable size: %d (expected 1-4).", size);
		}

		switch (size) {
			case 4:
				glUniform4fv(location, count, vec);
				break;
			case 3:
				glUniform3fv(location, count, vec);
				break;
			case 2:
				glUniform2fv(location, count, vec);
				break;
			case 1:
			default:
				glUniform1fv(location, count, vec);
				break;
		}

		// throw error if needed
		checkSetUniformError();
	}

	void PixelEffect::sendMatrix(const std::string& name, int size, const GLfloat* m, int count)
	{
		TemporaryAttacher attacher(this);
		GLint location = getUniformLocation(name);

		if (size < 2 || size > 4)
		{
			throw love::Exception("Invalid matrix size: %dx%d "
					"(can only set 2x2, 3x3 or 4x4 matrices).", size,size);
		}

		switch (size) {
			case 4:
				glUniformMatrix4fv(location, count, GL_FALSE, m);
				break;
			case 3:
				glUniformMatrix3fv(location, count, GL_FALSE, m);
				break;
			case 2:
			default:
				glUniformMatrix2fv(location, count, GL_FALSE, m);
				break;
		}

		// throw error if needed
		checkSetUniformError();
	}

	void PixelEffect::sendImage(const std::string& name, const Image& image)
	{
		GLint texture_unit = getTextureUnit(name);

		TemporaryAttacher attacher(this);
		GLint location = getUniformLocation(name);

		glActiveTexture(GL_TEXTURE0 + texture_unit);
		bindTexture(image.getTextureName(), true); // guarantee it gets bound
		glUniform1i(location, texture_unit);

		// reset texture unit
		glActiveTexture(GL_TEXTURE0);

		// throw error if needed
		checkSetUniformError();
	}

// 	void PixelEffect::sendCanvas(const std::string& name, const Canvas& canvas)
// 	{
// 		GLint texture_unit = getTextureUnit(name);
// 
// 		TemporaryAttacher attacher(this);
// 		GLint location = getUniformLocation(name);
// 
// 		glActiveTexture(GL_TEXTURE0 + texture_unit);
// 		bindTexture(canvas.getTextureName(), true); // guarantee it gets bound
// 		glUniform1i(location, texture_unit);
// 
// 		// reset texture unit
// 		glActiveTexture(GL_TEXTURE0);
// 
// 		// throw error if needed
// 		checkSetUniformError();
// 	}

	void PixelEffect::setUniformMatrix()
	{
		kmMat4 matrixP;
		kmMat4 matrixMV;
		kmMat4 matrixMVP;

		kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
		kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);

		kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);

		glUniformMatrix4fv((GLint)m_uniforms[e_UniformPMatrix], 1, GL_FALSE, matrixP.mat);
		glUniformMatrix4fv((GLint)m_uniforms[e_UniformMVMatrix], 1, GL_FALSE, matrixMV.mat);
		glUniformMatrix4fv((GLint)m_uniforms[e_UniformMVPMatrix], 1, GL_FALSE, matrixMVP.mat);
	}

	GLint PixelEffect::getUniformLocation(const std::string& name)
	{
		std::map<std::string, GLint>::const_iterator it = _uniforms.find(name);
		if (it != _uniforms.end())
			return it->second;

		GLint location = glGetUniformLocation(_program, name.c_str());
		if (location == -1)
		{
			throw love::Exception(
					"Cannot get location of shader variable `%s'.\n"
					"A common error is to define but not use the variable.", name.c_str());
		}

		_uniforms[name] = location;
		return location;
	}

	void PixelEffect::checkSetUniformError()
	{
		GLenum error_code = glGetError();
		if (GL_INVALID_OPERATION == error_code)
		{
			throw love::Exception(
				"Invalid operation:\n"
				"- Trying to send the wrong value type to shader variable, or\n"
				"- Trying to send array values with wrong dimension, or\n"
				"- Invalid variable name.");
		}
	}
} // opengl
} // graphics
} // love
