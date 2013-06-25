#ifndef LOVE_GRPAHICS_SHADER_H
#define LOVE_GRPAHICS_SHADER_H

#include "OpenGL.h"

#include "common/Object.h"

namespace love
{
namespace graphics
{
namespace opengl
{
	enum {
		e_VertexAttrib_Position,
		e_VertexAttrib_Color,
		e_VertexAttrib_TexCoords,

		e_VertexAttrib_MAX,
	};

	enum {
		e_UniformPMatrix,
		e_UniformMVMatrix,
		e_UniformMVPMatrix,
		e_UniformSampler,

		e_Uniform_MAX,
	};

// uniform names
#define UniformPMatrix			"PMatrix"
#define UniformMVMatrix			"MVMatrix"
#define UniformMVPMatrix		"MVPMatrix"
#define UniformSampler			"s_texture"

// Attribute names
#define AttributeNameColor		"a_color"
#define AttributeNamePosition	"a_position"
#define AttributeNameTexCoord	"a_texCoord"

	class Shader : public Object
	{
	public:
		Shader(const char* pVertexSource, const char* pFragmentSource);
		virtual ~Shader();

		void attach();
		void detach();

		void setUniformMatrix();

		/** calls glUniform1i only if the values are different than the previous call for this same shader program. */
		void setUniformLocationWith1i(GLint location, GLint i1);

		/** calls glUniform1f only if the values are different than the previous call for this same shader program. */
		void setUniformLocationWith1f(GLint location, GLfloat f1);

		/** calls glUniform4fv only if the values are different than the previous call for this same shader program. */
		void setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays);

		/** calls glUniformMatrix4fv only if the values are different than the previous call for this same shader program. */
		void setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices);

	protected:
		void addAttribute(const char* attributeName, GLuint index);

	private:
		GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

		GLuint loadShader(GLenum shaderType, const char* pSource);

		void initUniforms();

		bool updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes);

	protected:
		GLuint m_program;

		GLint m_uniforms[e_Uniform_MAX];
		struct _hashUniformEntry* m_pHashForUniforms;

	}; // Shader

	class ShapeShader : public Shader
	{
	public:
		ShapeShader();

	private:
		static const char gVertexShader[];
		static const char gFragmentShader[];

	public:
		GLuint colorLoc;
		GLuint pointSizeLoc;

	}; // ShapeShader

	class TextureShader : public Shader
	{
	public:
		TextureShader();

	private:
		static const char gVertexShader[];
		static const char gFragmentShader[];

	}; // TextureShader

	class ShaderCache
	{
	public:
		static ShaderCache* Instance();
		
		ShapeShader* getShapeShader();
		TextureShader* getTexShader();

	private:
		ShaderCache();
		~ShaderCache();

	private:
		ShapeShader* m_shape;
		TextureShader* m_tex;

		static ShaderCache* m_instance;

	}; // ShaderCache

} // opengl
} // graphics
} // love

#endif // LOVE_GRPAHICS_SHADER_H