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

#include "Graphics.h"

// STD
#include <iostream>

// LOVE
#include <common/Vector.h>
#include <graphics/Color.h>
#include <image/Image.h>
#include <libraries/kazmath/include/kazmath/GL/matrix.h>

#include "OpenGL.h"
#include "Font.h"
#include "Quad.h"
#include "SpriteBatch.h"
#include "ParticleSystem.h"
#include "PixelEffect.h"
#include "Shader.h"

namespace love
{
namespace graphics
{
namespace opengl
{
	Graphics::Graphics()
		: currentFont(0)
		, lineStyle(LINE_SMOOTH)
		, lineWidth(1)
		, matrixLimit(0)
		, userMatrices(0)
		, color(1.0f, 1.0f, 1.0f, 1.0f)
		, pointSize(1.0f)
	{
#ifdef LOVE_WINDOWS
		matrixLimit = 16;
#endif
		resetBoundTexture();
	}

	Graphics::~Graphics()
	{
 		if (currentFont != 0)
 			currentFont->release();
	}

	const char * Graphics::getName() const
	{
		return "love.graphics.opengl";
	}

	bool Graphics::checkMode(int width, int height, bool fullscreen)
	{
//		return currentWindow->checkWindowSize(width, height, fullscreen);
		return true;
	}

	DisplayState Graphics::saveState()
	{
		DisplayState s;

		s.color = getColor();
		s.backgroundColor = getBackgroundColor();

		s.blendMode = getBlendMode();
		s.colorMode = getColorMode();
		//get line style
		s.lineStyle = lineStyle;
		//get the point size
		glGetFloatv(GL_POINT_SIZE, &s.pointSize);
		//get point style
		s.pointStyle = (glIsEnabled(GL_POINT_SMOOTH) == GL_TRUE) ? Graphics::POINT_SMOOTH : Graphics::POINT_ROUGH;
		//get scissor status
		s.scissor = (glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE);
		//do we have scissor, if so, store the box
		if (s.scissor)
			glGetIntegerv(GL_SCISSOR_BOX, s.scissorBox);

		return s;
	}

	void Graphics::restoreState(const DisplayState & s)
	{
		setColor(s.color);
		setBackgroundColor(s.backgroundColor);
		setBlendMode(s.blendMode);
		setColorMode(s.colorMode);
		setLine(lineWidth, s.lineStyle);
		setPoint(s.pointSize, s.pointStyle);
		if (s.scissor)
			setScissor(s.scissorBox[0], s.scissorBox[1], s.scissorBox[2], s.scissorBox[3]);
		else
			setScissor();
	}

	bool Graphics::setMode(int width, int height, bool fullscreen, bool vsync, int fsaa)
	{
		////// This operation destroys the OpenGL context, so
		////// we must save the state.
		////DisplayState tempState;
		////if (isCreated())
		////	tempState = saveState();

		//// Unlad all volatile objects. These must be reloaded after
		//// the display mode change.
		//Volatile::unloadAll();

		//bool success = true;
		////bool success = currentWindow->setWindow(width, height, fullscreen, vsync, fsaa);
		////// Regardless of failure, we'll have to set up OpenGL once again.

		////width = currentWindow->getWidth();
		////height = currentWindow->getHeight();

		//// Okay, setup OpenGL.

		//// Enable blending
		//glEnable(GL_BLEND);

		//// "Normal" blending
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//// Enable line/point smoothing.
		//setLineStyle(LINE_SMOOTH);
		//glEnable(GL_POINT_SMOOTH);
		//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

		//// Enable textures
		//glEnable(GL_TEXTURE_2D);

		//// Set the viewport to top-left corner
		//glViewport(0, 0, width, height);

		//// Reset the projection matrix
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();

		//// Set up orthographic view (no depth)
		//glOrthof(0.0, width, height, 0.0, -1.0, 1.0);

		//// Reset modelview matrix
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

		//// Set pixel row alignment
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

		//// Reload all volatile objects.
		//if (!Volatile::loadAll())
		//	std::cerr << "Could not reload all volatile objects." << std::endl;

		////// Restore the display state.
		////restoreState(tempState);

		//// Get the maximum number of matrices
		//// subtract a few to give the engine some room.
		//glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &matrixLimit);
		//matrixLimit -= 5;

		//return success;

		//////////////////////////////////////////////////////////////////////////
		
		// Get the maximum number of matrices
		// subtract a few to give the engine some room.
		glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &matrixLimit);
		matrixLimit -= 5;

		return true;
	}

	void Graphics::getMode(int &width, int &height, bool &fullscreen, bool &vsync, int &fsaa)
	{
//		currentWindow->getWindow(width, height, fullscreen, vsync, fsaa);
	}

	void Graphics::reset()
	{
		DisplayState s;
		discardStencil();
// 		Canvas::bindDefaultCanvas();
// 		PixelEffect::detach();
		restoreState(s);
	}

	void Graphics::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		kmGLLoadIdentity();
	}

	void Graphics::present()
	{
	}

	int Graphics::getWidth()
	{
//		return currentWindow->getWidth();
		return 800;
	}

	int Graphics::getHeight()
	{
//		return currentWindow->getHeight();
		return 600;
	}

	void Graphics::setScissor(int x, int y, int width, int height)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, getHeight() - (y + height), width, height); // Compensates for the fact that our y-coordinate is reverse of OpenGLs.
	}

	void Graphics::setScissor()
	{
		glDisable(GL_SCISSOR_TEST);
	}

	int Graphics::getScissor(lua_State * L)
	{
		if (glIsEnabled(GL_SCISSOR_TEST) == GL_FALSE)
			return 0;

		GLint scissor[4];
		glGetIntegerv(GL_SCISSOR_BOX, scissor);

		lua_pushnumber(L, scissor[0]);
		lua_pushnumber(L, getHeight() - (scissor[1] + scissor[3])); // Compensates for the fact that our y-coordinate is reverse of OpenGLs.
		lua_pushnumber(L, scissor[2]);
		lua_pushnumber(L, scissor[3]);

		return 4;
	}

	void Graphics::defineStencil()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void Graphics::useStencil(bool invert)
	{
		glStencilFunc(GL_EQUAL, (int)(!invert), 1); // invert ? 0 : 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void Graphics::discardStencil()
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDisable(GL_STENCIL_TEST);
	}

	Image * Graphics::newImage(love::image::ImageData * data)
	{
		// Create the image.
		Image * image = new Image(data);
		bool success;
		try
		{
			success = image->load();
		}
		catch (love::Exception & e)
		{
			image->release();
			throw love::Exception(e.what());
		}
		if (!success)
		{
			image->release();
			return 0;
		}

		return image;
	}

	Quad * Graphics::newQuad(float x, float y, float w, float h, float sw, float sh)
	{
		Quad::Viewport v;
		v.x = x;
		v.y = y;
		v.w = w;
		v.h = h;
		return new Quad(v, sw, sh);
	}

	Font * Graphics::newFont(love::font::Rasterizer * r, const Image::Filter& filter)
	{
		Font * font = new Font(r, filter);

		// Load it and check for errors.
		if (!font)
		{
			delete font;
			return 0;
		}

		return font;
	}

	SpriteBatch * Graphics::newSpriteBatch(Image * image, int size, int usage)
	{
		SpriteBatch * t = NULL;
		try
		{
			t = new SpriteBatch(image, size, usage);
		}
		catch (love::Exception& e)
		{
			if (t) delete t;
			throw e;
		}
		return t;
	}

	ParticleSystem * Graphics::newParticleSystem(Image * image, int size)
	{
		return new ParticleSystem(image, size);
	}

	PixelEffect * Graphics::newPixelEffect(const std::string& code)
	{
		PixelEffect * effect = NULL;
		try
		{
			effect = new PixelEffect(code);
		}
		catch (love::Exception& e)
		{
			if (effect)
				delete effect;
			throw(e);
		}
		return effect;
	}

	void Graphics::setColor(const Color& c)
	{
		static const float f = 1 / 255.0f;
		color.r = c.r * f;
		color.g = c.g * f;
		color.b = c.b * f;
		color.a = c.a * f;
	}

	Color Graphics::getColor()
	{
 		Color t;
 		t.r = (unsigned char)(255.0f*color.r);
 		t.g = (unsigned char)(255.0f*color.g);
 		t.b = (unsigned char)(255.0f*color.b);
 		t.a = (unsigned char)(255.0f*color.a);
 
 		return t;
	}

	void Graphics::setBackgroundColor(const Color& c)
	{
		glClearColor((float)c.r/255.0f, (float)c.g/255.0f, (float)c.b/255.0f, (float)c.a/255.0f);
	}

	Color Graphics::getBackgroundColor()
	{
		float c[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, c);

		Color t;
		t.r = (unsigned char)(255.0f*c[0]);
		t.g = (unsigned char)(255.0f*c[1]);
		t.b = (unsigned char)(255.0f*c[2]);
		t.a = (unsigned char)(255.0f*c[3]);

		return t;
	}

	void Graphics::setFont( Font * font )
	{
 		if (currentFont != 0)
 			currentFont->release();
 
 		currentFont = font;
 
 		if (font != 0)
 			currentFont->retain();
	}

	Font * Graphics::getFont()
	{
		return currentFont;
	}

	void Graphics::setBlendMode( Graphics::BlendMode mode )
	{
		//glAlphaFunc(GL_GEQUAL, 0);

		//if (GLEE_VERSION_1_4 || GLEE_ARB_imaging)
		//{
		//	if (mode == BLEND_SUBTRACTIVE)
		//		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		//	else
		//		glBlendEquation(GL_FUNC_ADD);
		//}
		//else if (GLEE_EXT_blend_minmax && GLEE_EXT_blend_subtract)
		//{
		//	if (mode == BLEND_SUBTRACTIVE)
		//		glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);
		//	else
		//		glBlendEquationEXT(GL_FUNC_ADD_EXT);
		//}
		//else
		//{
		//	if (mode == BLEND_SUBTRACTIVE)
		//		throw Exception("This graphics card does not support the subtract blend mode!");
		//	// GL_FUNC_ADD is the default even without access to glBlendEquation, so that'll still work.
		//}

		//if (mode == BLEND_ALPHA)
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//else if (mode == BLEND_MULTIPLICATIVE)
		//	glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		//else if (mode == BLEND_PREMULTIPLIED)
		//	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		//else // mode == BLEND_ADDITIVE || mode == BLEND_SUBTRACTIVE
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void Graphics::setColorMode ( Graphics::ColorMode mode )
	{
		// for gles2
// 		if (mode == COLOR_MODULATE)
// 			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
// 		else if (mode == COLOR_COMBINE) {
// 			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
// 			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
// 			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
// 		}
// 		else // mode = COLOR_REPLACE
// 			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	void Graphics::setDefaultImageFilter(const Image::Filter& f)
	{
		Image::setDefaultFilter(f);
	}

	Graphics::BlendMode Graphics::getBlendMode ()
	{
// 		GLint dst, src, equation;
// 		glGetIntegerv(GL_BLEND_DST, &dst);
// 		glGetIntegerv(GL_BLEND_SRC, &src);
// 		glGetIntegerv(GL_BLEND_EQUATION, &equation);
// 
// 		if (equation == GL_FUNC_REVERSE_SUBTRACT) // && src == GL_SRC_ALPHA && dst == GL_ONE
// 			return BLEND_SUBTRACTIVE;
// 		else if (src == GL_SRC_ALPHA && dst == GL_ONE) // && equation == GL_FUNC_ADD
// 			return BLEND_ADDITIVE;
// 		else if (src == GL_SRC_ALPHA && dst == GL_ONE_MINUS_SRC_ALPHA) // && equation == GL_FUNC_ADD
// 			return BLEND_ALPHA;
// 		else if (src == GL_DST_COLOR && dst == GL_ONE_MINUS_SRC_ALPHA) // && equation == GL_FUNC_ADD
// 			return BLEND_MULTIPLICATIVE;
// 		else if (src == GL_ONE && dst == GL_ONE_MINUS_SRC_ALPHA) // && equation == GL_FUNC_ADD
// 			return BLEND_PREMULTIPLIED;

		return BLEND_MAX_ENUM; // Should never be reached.
	}

	Graphics::ColorMode Graphics::getColorMode()
	{
		// for gles2
// 		GLint mode;
// 		glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &mode);
// 
// 		if (mode == GL_MODULATE)
// 			return COLOR_MODULATE;
// 		else if (mode == GL_COMBINE)
// 			return COLOR_COMBINE;
// 		else // mode == GL_REPLACE
// 			return COLOR_REPLACE;

		return COLOR_MODULATE;
	}

	const Image::Filter& Graphics::getDefaultImageFilter() const
	{
		return Image::getDefaultFilter();
	}

	void Graphics::setLineWidth( float width )
	{
		lineWidth = width;
	}

	void Graphics::setLineStyle(Graphics::LineStyle style)
	{
		lineStyle = style;
	}

	void Graphics::setLine( float width, Graphics::LineStyle style )
	{
		setLineWidth(width);

		if (style == 0)
			return;
		setLineStyle(style);
	}

	float Graphics::getLineWidth()
	{
		return lineWidth;
	}

	Graphics::LineStyle Graphics::getLineStyle()
	{
		return lineStyle;
	}

	void Graphics::setPointSize( float size )
	{
		pointSize = size;
	}

	void Graphics::setPointStyle( Graphics::PointStyle style )
	{
		if ( style == POINT_SMOOTH )
			glEnable(GL_POINT_SMOOTH);
		else // love::POINT_ROUGH
			glDisable(GL_POINT_SMOOTH);
	}

	void Graphics::setPoint( float size, Graphics::PointStyle style )
	{
		if ( style == POINT_SMOOTH )
			glEnable(GL_POINT_SMOOTH);
		else // POINT_ROUGH
			glDisable(GL_POINT_SMOOTH);

		pointSize = size;
	}

	float Graphics::getPointSize()
	{
// 		GLfloat size;
// 		glGetFloatv(GL_POINT_SIZE, &size);
// 		return (float)size;

		return pointSize;
	}

	Graphics::PointStyle Graphics::getPointStyle()
	{
		if (glIsEnabled(GL_POINT_SMOOTH) == GL_TRUE)
			return POINT_SMOOTH;
		else
			return POINT_ROUGH;
	}

	int Graphics::getMaxPointSize()
	{
		GLint max;
		glGetIntegerv(GL_POINT_SIZE_MAX, &max);
		return (int)max;
	}

	void Graphics::print( const char * str, float x, float y , float angle, float sx, float sy, float ox, float oy, float kx, float ky)
	{
		if (currentFont != 0)
		{
			std::string text(str);
			currentFont->print(text, x, y, angle, sx, sy, ox, oy, kx, ky);
		}
	}

	void Graphics::printf( const char * str, float x, float y, float wrap, AlignMode align)
	{
		if (currentFont == 0)
			return;

		using namespace std;
		string text(str);
		vector<string> lines_to_draw = currentFont->getWrap(text, wrap);

		// now for the actual printing
		vector<string>::const_iterator line_iter, line_end = lines_to_draw.end();
		for (line_iter = lines_to_draw.begin(); line_iter != line_end; ++line_iter)
		{
			float width = static_cast<float>(currentFont->getWidth( *line_iter ));
			switch (align) {
				case ALIGN_RIGHT:
					currentFont->print(*line_iter, ceil(x + wrap - width), ceil(y));
					break;
				case ALIGN_CENTER:
					currentFont->print(*line_iter, ceil(x + (wrap - width) / 2), ceil(y));
					break;
				case ALIGN_LEFT:
				default:
					currentFont->print(*line_iter, ceil(x), ceil(y));
					break;
			}
			y += currentFont->getHeight() * currentFont->getLineHeight();
		}
	}

	/**
	* Primitives
	**/

	void Graphics::point( float x, float y )
	{
 		float vertex[2];
 		vertex[0] = x;
 		vertex[1] = y;

		if (PixelEffect::current)
		{
			PixelEffect::current->setUniformMatrix();
		}
		else
		{
			ShapeShader* shader = ShaderCache::Instance()->getShapeShader();

			shader->attach();
			shader->setUniformMatrix();

			shader->setUniformLocationWith4fv(shader->colorLoc, (GLfloat*)&color.r, 1);
			shader->setUniformLocationWith1f(shader->pointSizeLoc, pointSize);
		}

		glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)vertex);
		glEnableVertexAttribArray(e_VertexAttrib_Position);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	// Calculate line boundary points u1 and u2. Sketch:
	//              u1
	// -------------+---...___
	//              |         ```'''--  ---
	// p- - - - - - q- - . _ _           | w/2
	//              |          ` ' ' r   +
	// -------------+---...___           | w/2
	//              u2         ```'''-- ---
	//
	// u1 and u2 depend on four things:
	//   - the half line width w/2
	//   - the previous line vertex p
	//   - the current line vertex q
	//   - the next line vertex r
	//
	// u1/u2 are the intersection points of the parallel lines to p-q and q-r,
	// i.e. the point where
	//
	//    (p + w/2 * n1) + mu * (q - p) = (q + w/2 * n2) + lambda * (r - q)   (u1)
	//    (p - w/2 * n1) + mu * (q - p) = (q - w/2 * n2) + lambda * (r - q)   (u2)
	//
	// with n1,n2 being the normals on the segments p-q and q-r:
	//
	//    n1 = perp(q - p) / |q - p|
	//    n2 = perp(r - q) / |r - q|
	//
	// The intersection points can be calculated using cramers rule.
	static void pushIntersectionPoints(Vector *vertices, Vector* overdraw,
			int pos, int count, float hw, float overdraw_factor,
			const Vector& p, const Vector& q, const Vector& r)
	{
		// calculate line directions
		Vector s = (q - p);
		Vector t = (r - q);

		// calculate vertex displacement vectors
		Vector n1 = s.getNormal();
		Vector n2 = t.getNormal();
		n1.normalize();
		n2.normalize();
		float det_norm = n1 ^ n2; // will be close to zero if the angle between the normals is sharp
		n1 *= hw;
		n2 *= hw;

		// lines parallel -> assume intersection at displacement points
		if (fabs(det_norm) <= .03)
		{
			vertices[pos]   = q - n2;
			vertices[pos+1] = q + n2;
		}
		// real intersection -> calculate boundary intersection points with cramers rule
		else
		{
			float det = s ^ t;
			Vector d = n1 - n2;
			Vector b = s - d; // s = q - p
			Vector c = s + d;
			float lambda = (b ^ t) / det;
			float mu     = (c ^ t) / det;

			// ordering for GL_TRIANGLE_STRIP
			vertices[pos]   = p + s*mu - n1;     // u1
			vertices[pos+1] = p + s*lambda + n1; // u2
		}

		if (overdraw)
		{
			// displacement of the overdraw vertices
			Vector x = (vertices[pos] - q) * overdraw_factor;

			overdraw[pos]   = vertices[pos];
			overdraw[pos+1] = vertices[pos] + x;

			overdraw[2*count-pos-2] = vertices[pos+1];
			overdraw[2*count-pos-1] = vertices[pos+1] - x;
		}
	}

	// precondition:
	// glEnableClientState(GL_VERTEX_ARRAY);
	static void draw_overdraw(Vector* overdraw, size_t count, float pixel_size, bool looping)
	{
		// for gles2

// 		// if not looping, the outer overdraw vertices need to be displaced
// 		// to cover the line endings, i.e.:
// 		// +- - - - //- - +         +- - - - - //- - - +
// 		// +-------//-----+         : +-------//-----+ :
// 		// | core // line |   -->   : | core // line | :
// 		// +-----//-------+         : +-----//-------+ :
// 		// +- - //- - - - +         +- - - //- - - - - +
// 		if (!looping)
// 		{
// 			Vector s = overdraw[1] - overdraw[3];
// 			s.normalize();
// 			s *= pixel_size;
// 			overdraw[1] += s;
// 			overdraw[2*count-1] += s;
// 
// 			Vector t = overdraw[count-1] - overdraw[count-3];
// 			t.normalize();
// 			t *= pixel_size;
// 			overdraw[count-1] += t;
// 			overdraw[count+1] += t;
// 
// 			// we need to draw two more triangles to close the
// 			// overdraw at the line start.
// 			overdraw[2*count]   = overdraw[0];
// 			overdraw[2*count+1] = overdraw[1];
// 		}
// 
// 		// prepare colors:
// 		// even indices in overdraw* point to inner vertices => alpha = current-alpha,
// 		// odd indices point to outer vertices => alpha = 0.
// 		GLfloat c[4];
// 		glGetFloatv(GL_CURRENT_COLOR, c);
// 
// 		Color *colors = new Color[2*count+2];
// 		for (size_t i = 0; i < 2*count+2; ++i)
// 		{
// 			colors[i] = Color(GLubyte(c[0] * 255.f),
// 					GLubyte(c[1] * 255.f),
// 					GLubyte(c[2] * 255.f),
// 					// avoids branching. equiv to if (i%2 == 1) colors[i].a = 0;
// 					GLubyte(c[3] * 255.f) * GLubyte(i%2 == 0));
// 		}
// 
// 		// draw faded out line halos
// 		glEnableClientState(GL_COLOR_ARRAY);
// 		glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
// 		glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)overdraw);
// 		glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*count + 2 * int(!looping));
// 		glDisableClientState(GL_COLOR_ARRAY);
// 		// "if GL_COLOR_ARRAY is enabled, the value of the current color is
// 		// undefined after glDrawArrays executes"
// 		glColor4f(c[0], c[1], c[2], c[3]);
// 
// 		delete[] colors;
	}

	void Graphics::polyline(const float* coords, size_t count)
	{
		if (PixelEffect::current)
		{
			PixelEffect::current->setUniformMatrix();
		}
		else
		{
			ShapeShader* shader = ShaderCache::Instance()->getShapeShader();

			shader->attach();
			shader->setUniformMatrix();

			shader->setUniformLocationWith4fv(shader->colorLoc, (GLfloat*)&color.r, 1);
			shader->setUniformLocationWith1f(shader->pointSizeLoc, pointSize);
		}

		glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)coords);
		glEnableVertexAttribArray(e_VertexAttrib_Position);
		glDrawArrays(GL_LINE_STRIP, 0, count >> 1);
	}

	void Graphics::triangle(DrawMode mode, float x1, float y1, float x2, float y2, float x3, float y3 )
	{
  		if (mode == DRAW_LINE)
  		{
 			float coords[] = { x1,y1, x2,y2, x3,y3, x1,y1 };
  			polyline(coords, 4 * 2);
  		}
  		else
  		{
			GLfloat gTriangleVertices[6];
			gTriangleVertices[0] = x1;
			gTriangleVertices[1] = y1;
			gTriangleVertices[2] = x2;
			gTriangleVertices[3] = y2;
			gTriangleVertices[4] = x3;
			gTriangleVertices[5] = y3;

			if (PixelEffect::current)
			{
				PixelEffect::current->setUniformMatrix();
			}
			else
			{
				ShapeShader* shader = ShaderCache::Instance()->getShapeShader();

				shader->attach();
				shader->setUniformMatrix();

				shader->setUniformLocationWith4fv(shader->colorLoc, (GLfloat*)&color.r, 1);
				shader->setUniformLocationWith1f(shader->pointSizeLoc, pointSize);
			}

			glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
			glEnableVertexAttribArray(e_VertexAttrib_Position);
			glDrawArrays(GL_TRIANGLES, 0, 3);
  		}
	}

	void Graphics::rectangle(DrawMode mode, float x, float y, float w, float h)
	{
		quad(mode, x,y, x,y+h, x+w,y+h, x+w,y);
	}

	void Graphics::quad(DrawMode mode, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
	{
 		if (mode == DRAW_LINE)
 		{
 			float coords[] = { x1,y1, x2,y2, x3,y3, x4,y4, x1,y1 };
 			polyline(coords, 5 * 2);
 		}
 		else
 		{
 			float coords[] = { x1,y1, x2,y2, x4,y4, x3,y3 };

			if (PixelEffect::current)
			{
				PixelEffect::current->setUniformMatrix();
			}
			else
			{
				ShapeShader* shader = ShaderCache::Instance()->getShapeShader();

				shader->attach();
				shader->setUniformMatrix();

				shader->setUniformLocationWith4fv(shader->colorLoc, (GLfloat*)&color.r, 1);
				shader->setUniformLocationWith1f(shader->pointSizeLoc, pointSize);
			}

			glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, coords);
			glEnableVertexAttribArray(e_VertexAttrib_Position);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 		}
	}

	void Graphics::circle(DrawMode mode, float x, float y, float radius, int points)
	{
 		float two_pi = static_cast<float>(LOVE_M_PI * 2);
 		if (points <= 0) points = 1;
 		float angle_shift = (two_pi / points);
 		float phi = .0f;
 
 		if (mode == DRAW_LINE)
 		{
 			float *coords = new float[2 * (points + 1)];
 			for (int i = 0; i < points; ++i, phi += angle_shift)
 			{
 				coords[2*i]   = x + radius * cos(phi);
 				coords[2*i+1] = y + radius * sin(phi);
 			}
 
 			coords[2*points]   = coords[0];
 			coords[2*points+1] = coords[1];
 
 			polyline(coords, (points + 1) * 2);
 
 			delete[] coords;
 		}
 		else
 		{
 			float *coords = new float[2 * (points + 1)];
 			for (int i = 0; i < points; ++i, phi += angle_shift)
 			{
 				coords[2*i]   = x + radius * cos(phi);
 				coords[2*i+1] = y + radius * sin(phi);
 			}
 
 			coords[2*points]   = coords[0];
 			coords[2*points+1] = coords[1];
 
 			//////////////////////////////////////////////////////////////////////////
 
 // 			float *coords = new float[2 * (points + 2)];
 // 			coords[0] = x;
 // 			coords[1] = y;
 // 
 // 			for (int i = 0; i < points; ++i, phi += angle_shift)
 // 			{
 // 				coords[2*i + 1]   = x + radius * cos(phi);
 // 				coords[2*i + 2] = y + radius * sin(phi);
 // 			}
 // 
 // 			coords[2*points + 1]   = coords[2];
 // 			coords[2*points + 2] = coords[3];
 

			if (PixelEffect::current)
			{
				PixelEffect::current->setUniformMatrix();
			}
			else
			{
				ShapeShader* shader = ShaderCache::Instance()->getShapeShader();

				shader->attach();
				shader->setUniformMatrix();

				shader->setUniformLocationWith4fv(shader->colorLoc, (GLfloat*)&color.r, 1);
				shader->setUniformLocationWith1f(shader->pointSizeLoc, pointSize);
			}

			glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)coords);
			glEnableVertexAttribArray(e_VertexAttrib_Position);
			glDrawArrays(GL_TRIANGLE_FAN, 0, points + 1);

 			delete[] coords;
 		}
	}

	void Graphics::arc(DrawMode mode, float x, float y, float radius, float angle1, float angle2, int points)
	{
 		// Nothing to display with no points or equal angles. (Or is there with line mode?)
 		if (points <= 0 || angle1 == angle2)
 			return;
 
 		// Oh, you want to draw a circle?
 		if (fabs(angle1 - angle2) >= 2.0f * (float) LOVE_M_PI)
 		{
 			circle(mode, x, y, radius, points);
 			return;
 		}
 
 		float angle_shift = (angle2 - angle1) / points;
 		// Bail on precision issues.
 		if (angle_shift == 0.0)
 			return;
 
 		float phi = angle1;
 		int num_coords = (points + 3) * 2;
 		float * coords = new float[num_coords];
 		coords[0] = coords[num_coords - 2] = x;
 		coords[1] = coords[num_coords - 1] = y;
 
 		for (int i = 0; i <= points; ++i, phi += angle_shift)
 		{
 			coords[2 * (i+1)]     = x + radius * cos(phi);
 			coords[2 * (i+1) + 1] = y + radius * sin(phi);
 		}
 
 		// GL_POLYGON can only fill-draw convex polygons, so we need to do stuff manually here
 		if (mode == DRAW_LINE)
 		{
 			polyline(coords, num_coords); // Artifacts at sharp angles if set to looping.
 		}
 		else
 		{
			if (PixelEffect::current)
			{
				PixelEffect::current->setUniformMatrix();
			}
			else
			{
				ShapeShader* shader = ShaderCache::Instance()->getShapeShader();

				shader->attach();
				shader->setUniformMatrix();

				shader->setUniformLocationWith4fv(shader->colorLoc, (GLfloat*)&color.r, 1);
				shader->setUniformLocationWith1f(shader->pointSizeLoc, pointSize);
			}

			glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)coords);
			glEnableVertexAttribArray(e_VertexAttrib_Position);
			glDrawArrays(GL_TRIANGLE_FAN, 0, points + 2);


//  			glDisable(GL_TEXTURE_2D);
//  			glEnableClientState(GL_VERTEX_ARRAY);
//  			glVertexPointer(2, GL_FLOAT, 0, (const GLvoid *) coords);
//  			glDrawArrays(GL_TRIANGLE_FAN, 0, points + 2);
//  			glDisableClientState(GL_VERTEX_ARRAY);
//  			glEnable(GL_TEXTURE_2D);
 		}
 
 		delete[] coords;
	}

 	/// @param mode    the draw mode
 	/// @param coords  the coordinate array
 	/// @param count   the number of coordinates/size of the array
 	void Graphics::polygon(DrawMode mode, const float* coords, size_t count)
 	{
 		// coords is an array of a closed loop of vertices, i.e.
 		// coords[count-2] = coords[0], coords[count-1] = coords[1]
 		if (mode == DRAW_LINE)
 		{
 			polyline(coords, count);
 		}
 		//else
 		//{
 		//	glDisable(GL_TEXTURE_2D);
 		//	glEnableClientState(GL_VERTEX_ARRAY);
 		//	glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)coords);
 		//	glDrawArrays(GL_POLYGON, 0, count/2-1); // opengl will close the polygon for us
 		//	glDisableClientState(GL_VERTEX_ARRAY);
 		//	glEnable(GL_TEXTURE_2D);
 		//}
 	}

	love::image::ImageData * Graphics::newScreenshot(love::image::Image * image)
	{
		int w = getWidth();
		int h = getHeight();

		int row = 4*w;

		int size = row*h;

		GLubyte * pixels = new GLubyte[size];
		GLubyte * screenshot = new GLubyte[size];

		glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// OpenGL sucks and reads pixels from the lower-left. Let's fix that.

		GLubyte *src = pixels - row, *dst = screenshot + size;

		for (int i = 0; i < h; ++i)
		{
			memcpy(dst-=row, src+=row, row);
		}

		love::image::ImageData * img = image->newImageData(w, h, (void*)screenshot);

		delete [] pixels;
		delete [] screenshot;

		return img;
	}

	void Graphics::push()
	{
 		if (userMatrices == matrixLimit)
 			throw Exception("Maximum stack depth reached.");
 		kmGLPushMatrix();
 		++userMatrices;
	}

	void Graphics::pop()
	{
 		if (userMatrices < 1)
 			throw Exception("Minimum stack depth reached. (More pops than pushes?)");
 		kmGLPopMatrix();
 		--userMatrices;
	}

	void Graphics::rotate(float r)
	{
		kmGLRotatef(LOVE_TODEG(r), 0, 0, 1);
	}

	void Graphics::scale(float x, float y)
	{
		kmGLScalef(x, y, 1);
	}

	void Graphics::translate(float x, float y)
	{
		kmGLTranslatef(x, y, 0);
	}

	void Graphics::shear(float kx, float ky)
	{
 		Matrix t;
 		t.setShear(kx, ky);
		kmMat4 m;
		memcpy(m.mat, t.getElements(), sizeof(float) * 16);
 		kmGLMultMatrix(&m);
	}

	void Graphics::drawTest(Image * image, float x, float y, float a, float sx, float sy, float ox, float oy)
	{
 		image->bind();
 
 		// Buffer for transforming the image.
 		vertex buf[4];
 
 		Matrix t;
 		t.translate(x, y);
 		t.rotate(a);
 		t.scale(sx, sy);
 		t.translate(ox, oy);
 		t.transform(buf, image->getVertices(), 4);
 
 		const vertex * vertices = image->getVertices();
 
		if (PixelEffect::current)
		{
			PixelEffect::current->setUniformMatrix();
		}
		else
		{
			Shader* shader = ShaderCache::Instance()->getTexShader();

			shader->attach();
			shader->setUniformMatrix();
		}

		// Load the vertex position
		glVertexAttribPointer(e_VertexAttrib_Position, 2, GL_FLOAT, 
			GL_FALSE, sizeof(vertex), (GLvoid*)&buf[0].x);
		// Load the texture coordinate
		glVertexAttribPointer(e_VertexAttrib_TexCoords, 2, GL_FLOAT,
			GL_FALSE, sizeof(vertex), (GLvoid*)&vertices[0].s);
		// Load the color
		glVertexAttribPointer(e_VertexAttrib_Color, 4, GL_UNSIGNED_BYTE,
			GL_FALSE, sizeof(vertex), (GLvoid*)&vertices[0].r);

		glEnableVertexAttribArray(e_VertexAttrib_Position);
		glEnableVertexAttribArray(e_VertexAttrib_TexCoords);
		glEnableVertexAttribArray(e_VertexAttrib_Color);

 		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

} // opengl
} // graphics
} // love