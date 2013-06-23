#include "Shader.h"

#include "common/log.h"

#include "libraries/uthash.h"
#include "libraries/kazmath/include/kazmath/GL/matrix.h"

#ifdef LOVE_ANDROID_JNI
#include <malloc.h>
#endif

namespace love
{
namespace graphics
{
namespace opengl
{
	//////////////////////////////////////////////////////////////////////////
	// class Shader
	//////////////////////////////////////////////////////////////////////////

	typedef struct _hashUniformEntry
	{
		GLvoid*         value;       // value
		unsigned int    location;    // Key
		UT_hash_handle  hh;          // hash entry
	} tHashUniformEntry;

	Shader::Shader(const char* pVertexSource, const char* pFragmentSource)
		: m_program(0)
		, m_pHashForUniforms(NULL)
	{
		m_program = createProgram(pVertexSource, pFragmentSource);

		memset(m_uniforms, 0, sizeof(m_uniforms));
		initUniforms();
	}

	Shader::~Shader()
	{
		if (m_program)
			glDeleteProgram(m_program);

		tHashUniformEntry *current_element, *tmp;

		// Purge uniform hash
		HASH_ITER(hh, m_pHashForUniforms, current_element, tmp)
		{
			HASH_DEL(m_pHashForUniforms, current_element);
			free(current_element->value);
			free(current_element);
		}
	}

	void Shader::attach()
	{
		glUseProgram(m_program);
	}

	void Shader::detach()
	{
		glUseProgram(0);
	}

	void Shader::setUniformMatrix()
	{
		kmMat4 matrixP;
		kmMat4 matrixMV;
		kmMat4 matrixMVP;

		kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
		kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);

		kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);

		setUniformLocationWithMatrix4fv(m_uniforms[e_UniformPMatrix], matrixP.mat, 1);
		setUniformLocationWithMatrix4fv(m_uniforms[e_UniformMVMatrix], matrixMV.mat, 1);
		setUniformLocationWithMatrix4fv(m_uniforms[e_UniformMVPMatrix], matrixMVP.mat, 1);
	}

	void Shader::setUniformLocationWith1f(GLint location, GLfloat f1)
	{
		bool updated = updateUniformLocation(location, &f1, sizeof(f1)*1);

		if( updated )
		{
			glUniform1f( (GLint)location, f1);
		}
	}

	void Shader::setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
	{
		bool updated =  updateUniformLocation(location, floats, sizeof(float)*4*numberOfArrays);

		if( updated )
		{
			glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
		}
	}

	void Shader::setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices)
	{
		bool updated = updateUniformLocation(location, matrixArray, sizeof(float)*16*numberOfMatrices);

		if( updated )
		{
			glUniformMatrix4fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
		}
	}

	GLuint Shader::loadShader(GLenum shaderType, const char* pSource)
	{
		GLuint shader = glCreateShader(shaderType);
		if (shader) {
			const GLchar *sources[] = {
				"precision mediump float;\n"
				"uniform mat4 PMatrix;\n"
				"uniform mat4 MVMatrix;\n"
				"uniform mat4 MVPMatrix;\n",
				pSource,
			};

			glShaderSource(shader, sizeof(sources)/sizeof(*sources), sources, NULL);
			glCompileShader(shader);
			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
				if (infoLen) {
					char* buf = (char*) malloc(infoLen);
					if (buf) {
						glGetShaderInfoLog(shader, infoLen, NULL, buf);
						log("Could not compile shader %d:\n%s\n",
							shaderType, buf);
						free(buf);
					}
					glDeleteShader(shader);
					shader = 0;
				}
			}
		}
		return shader;
	}

	static void checkGlError(const char* op) 
	{
		for (GLint error = glGetError(); error; error = glGetError()) 
		{
			log("after %s() glError (0x%x)\n", op, error);
		}
	}

	GLuint Shader::createProgram(const char* pVertexSource, const char* pFragmentSource)
	{
		GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
		if (!vertexShader) {
			return 0;
		}

		GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
		if (!pixelShader) {
			return 0;
		}

		GLuint program = glCreateProgram();
		if (program) {
			glAttachShader(program, vertexShader);
			checkGlError("glAttachShader");
			glAttachShader(program, pixelShader);
			checkGlError("glAttachShader");
			glLinkProgram(program);
			GLint linkStatus = GL_FALSE;
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE) {
				GLint bufLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
				if (bufLength) {
					char* buf = (char*) malloc(bufLength);
					if (buf) {
						glGetProgramInfoLog(program, bufLength, NULL, buf);
						log("Could not link program:\n%s\n", buf);
						free(buf);
					}
				}
				glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}

	void Shader::initUniforms()
	{
		m_uniforms[e_UniformPMatrix] = glGetUniformLocation(m_program, "PMatrix");
		m_uniforms[e_UniformMVMatrix] = glGetUniformLocation(m_program, "MVMatrix");
		m_uniforms[e_UniformMVPMatrix] = glGetUniformLocation(m_program, "MVPMatrix");
	}

	// Uniform cache

	bool Shader::updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes)
	{
		if (location < 0)
		{
			return false;
		}

		bool updated = true;
		tHashUniformEntry *element = NULL;
		HASH_FIND_INT(m_pHashForUniforms, &location, element);

		if (! element)
		{
			element = (tHashUniformEntry*)malloc( sizeof(*element) );

			// key
			element->location = location;

			// value
			element->value = malloc( bytes );
			memcpy(element->value, data, bytes );

			HASH_ADD_INT(m_pHashForUniforms, location, element);
		}
		else
		{
			if (memcmp(element->value, data, bytes) == 0)
			{
				updated = false;
			}
			else
			{
				memcpy(element->value, data, bytes);
			}
		}

		return updated;
	}

	//////////////////////////////////////////////////////////////////////////
	// class ShapeShader
	//////////////////////////////////////////////////////////////////////////

	const char ShapeShader::gVertexShader[] = 
		"attribute vec4 a_position;						\n"
		"uniform vec4 u_color;							\n"
		"uniform float u_pointSize;						\n"
		"varying vec4 v_fragmentColor;					\n"
		"												\n"
		"void main()									\n"
		"{												\n"
		"	gl_Position = MVPMatrix * a_position;		\n"
		"	gl_PointSize = u_pointSize;					\n"
		"	v_fragmentColor = u_color;					\n"
		"}												\n";

	const char ShapeShader::gFragmentShader[] = 
		"varying vec4 v_fragmentColor;					\n"
		"												\n"
		"void main()									\n"
		"{												\n"
		"	gl_FragColor = v_fragmentColor;				\n"
		"}												\n";

	ShapeShader::ShapeShader()
		: Shader(gVertexShader, gFragmentShader)
		, positionLoc(0)
		, colorLoc(0)
		, pointSizeLoc(0)
	{
		if (m_program)
		{
			positionLoc = glGetAttribLocation(m_program, "a_position");
			checkGlError("glGetAttribLocation");
			log("glGetAttribLocation(\"a_position\") = %d\n", positionLoc);

			colorLoc = glGetUniformLocation(m_program, "u_color");
			checkGlError("glGetUniformLocation");
			log("glGetUniformLocation(\"u_color\") = %d\n", colorLoc);

			pointSizeLoc = glGetUniformLocation(m_program, "u_pointSize");
			checkGlError("glGetUniformLocation");
			log("glGetUniformLocation(\"u_pointSize\") = %d\n", pointSizeLoc);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// class TextureShader
	//////////////////////////////////////////////////////////////////////////

	const char TextureShader::gVertexShader[] =  
		"attribute vec4 a_position;						\n"
		"attribute vec2 a_texCoord;						\n"
		"attribute vec4 a_color;						\n"
		"												\n"
		"varying vec2 v_texCoord;						\n"
		"varying vec4 v_fragmentColor;					\n"
		"												\n"
		"void main()									\n"
		"{												\n"
		"   gl_Position = MVPMatrix * a_position;		\n"
		"	v_fragmentColor = a_color / 255.0;			\n"
		"   v_texCoord = a_texCoord;					\n"
		"}												\n";

	const char TextureShader::gFragmentShader[] =  
		"varying vec4 v_fragmentColor;												\n"
		"varying vec2 v_texCoord;													\n"
		"uniform sampler2D s_texture;												\n"
		"																			\n"
		"void main()																\n"
		"{																			\n"
		"  gl_FragColor = v_fragmentColor * texture2D(s_texture, v_texCoord);		\n"
		"}																			\n";

	TextureShader::TextureShader()
		: Shader(gVertexShader, gFragmentShader)
		, positionLoc(0)
		, texCoordLoc(0)
		, colorLoc(0)
		, samplerLoc(0)
	{
		// Get the attribute locations
		positionLoc = glGetAttribLocation(m_program, "a_position");
		texCoordLoc = glGetAttribLocation(m_program, "a_texCoord");
		colorLoc = glGetAttribLocation(m_program, "a_color");

		// Get the sampler location
		samplerLoc = glGetUniformLocation (m_program, "s_texture");
	}

	//////////////////////////////////////////////////////////////////////////
	// class ShaderCache
	//////////////////////////////////////////////////////////////////////////
	
	ShaderCache* ShaderCache::m_instance = NULL;

	ShaderCache::ShaderCache()
	{
		m_shape = NULL;
		m_tex = NULL;
	}

	ShaderCache::~ShaderCache()
	{
	}

	ShapeShader* ShaderCache::getShapeShader()
	{
		if (!m_shape)
			m_shape = new ShapeShader;
		return m_shape;
	}

	TextureShader* ShaderCache::getTexShader()
	{
		if (!m_tex)
			m_tex = new TextureShader;
		return m_tex;
	}

	ShaderCache* ShaderCache::Instance()
	{
		if (!m_instance)
		{
			m_instance = new ShaderCache();
		}
		return m_instance;
	}

} // opengl
} // graphics
} // love