#include "ImageData.h"

// STD
#include <cstring>
#include <iostream>
 
// LOVE
#include <common/Exception.h>
#include <common/math.h>
#include <filesystem/null/File.h>

#ifdef LOVE_WINDOWS
#	include <libpng/png.h>
extern "C" {
#	include <libjpeg/jpeglib.h>
}
#elif defined LOVE_ANDROID_NATIVE
#	include <png.h>
extern "C" {
#	include <libjpeg/jpeglib.h>
}
#	include <android_native_app_glue.h>
#elif defined LOVE_ANDROID_JNI
#	include <png.h>
extern "C" {
#	include <libjpeg/jpeglib.h>
}
#endif

namespace love
{
namespace image
{
namespace null
{
	ImageData::ImageData(filesystem::File * file)
		: m_type(e_unknown)
		, m_initFile(file)
	{
		this->width = this->height = 0;
		this->data = NULL;
	}

	ImageData::ImageData(int width, int height)
		: m_type(e_unknown)
		, m_initFile(NULL)
	{
		this->data = NULL;

		this->width = width;
		this->height = height;
		create(width, height);

		// Set to black.
		memset(data, 0, width*height*4);
	}

	ImageData::ImageData(int width, int height, void * data)
		: m_type(e_unknown)
		, m_initFile(NULL)
	{
		this->data = NULL;

		this->width = width;
		this->height = height;
		create(width, height, data);
	}

	ImageData::~ImageData()
	{
		if (m_initFile) delete m_initFile;

		delete[] data;
	}

	void ImageData::create(int width, int height, void * data)
	{
		try
		{
			this->data = new unsigned char[width*height*sizeof(pixel)];
		}
		catch (std::bad_alloc &)
		{
			throw love::Exception("Out of memory");
		}

		if (data)
			memcpy(this->data, data, width*height*sizeof(pixel));
	}

	void ImageData::encode(love::filesystem::File * f, ImageData::Format format)
	{
	}

	void ImageData::load()
	{
		if (!m_initFile) return;

		m_initFile->open(love::filesystem::File::READ);
		
		m_filepath = m_initFile->getFilename();
		if (m_initFile->getExtension() == "png")
		{
			m_type = e_png;
			void* data = const_cast<void*>
				(static_cast<love::filesystem::null::File*>(m_initFile)->bufferize());
			loadByLibPng(data, m_initFile->getSize());
		}
		else if (m_initFile->getExtension() == "jpg")
		{
			m_type = e_jpg;
			void* data = const_cast<void*>
				(static_cast<love::filesystem::null::File*>(m_initFile)->bufferize());
			loadByLibJpeg(data, m_initFile->getSize());
		}
		else
		{
			m_type = e_unknown;
			width = height = 0;
			data = NULL;
		}

		m_initFile->close();
	}

	int offset = 0;

	void callback_read(png_structp png, png_bytep data, png_size_t size)
	{
		char* raw = (char*) png_get_io_ptr(png);
		memcpy(data, raw + offset, size);
		offset += size;
	}

	void ImageData::loadByLibPng(void* data, int size)
	{
		offset = 0;

		png_byte lHeader[8];
		png_structp lPngPtr = NULL; png_infop lInfoPtr = NULL;
		png_byte* lImageBuffer = NULL; png_bytep* lRowPtrs = NULL;
		png_int_32 lRowSize; bool lTransparency;

		do
		{
			// 		if (m_resource.read(lHeader, sizeof(lHeader)) == 0)
			// 			break;
			memcpy(lHeader, (char*)data + offset, sizeof(lHeader));
			offset += sizeof(lHeader);
			if (png_sig_cmp(lHeader, 0, 8) != 0) break;

			lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!lPngPtr) break;
			lInfoPtr = png_create_info_struct(lPngPtr);
			if (!lInfoPtr) break;
			png_set_read_fn(lPngPtr, data, callback_read);
			if (setjmp(png_jmpbuf(lPngPtr))) break;

			png_set_sig_bytes(lPngPtr, 8);
			png_read_info(lPngPtr, lInfoPtr);
			png_int_32 lDepth, lColorType;
			png_uint_32 lWidth, lHeight;
			png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight, &lDepth, &lColorType, NULL, NULL, NULL);
			width = lWidth; height = lHeight;

 			// Creates a full alpha channel if transparency is encoded as
 			// an array of palette entries or a single transparent color.
 			lTransparency = false;
 			if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS)) 
 			{
 				png_set_tRNS_to_alpha(lPngPtr);
 				lTransparency = true;

				// for read pngquant's 256 color image
// 				break;
 			}
			// Expands PNG with less than 8bits per channel to 8bits.
			if (lDepth < 8) 
			{
				png_set_packing (lPngPtr);
			} 
			// Shrinks PNG with 16bits per color channel down to 8bits.
			else if (lDepth == 16) 
			{
				png_set_strip_16(lPngPtr);
			}
			// Indicates that image needs conversion to RGBA if needed.
			switch (lColorType) 
			{
			case PNG_COLOR_TYPE_PALETTE:
				png_set_palette_to_rgb(lPngPtr);
				m_format = lTransparency ? GL_RGBA : GL_RGB;
				break;
			case PNG_COLOR_TYPE_RGB:
				m_format = lTransparency ? GL_RGBA : GL_RGB;
				break;
			case PNG_COLOR_TYPE_RGBA:
				m_format = GL_RGBA;
				break;
			case PNG_COLOR_TYPE_GRAY:
				png_set_expand_gray_1_2_4_to_8(lPngPtr);
				m_format = lTransparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
				break;
			case PNG_COLOR_TYPE_GA:
				png_set_expand_gray_1_2_4_to_8(lPngPtr);
				m_format = GL_LUMINANCE_ALPHA;
				break;
			}
			png_read_update_info(lPngPtr, lInfoPtr);

			lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
			if (lRowSize <= 0) break;
			lImageBuffer = new png_byte[lRowSize * lHeight];
			if (!lImageBuffer) break;
			lRowPtrs = new png_bytep[lHeight];
			if (!lRowPtrs) break;
			for (uint32 i = 0; i < lHeight; ++i) 
			{
				lRowPtrs[lHeight - (i + 1)] = lImageBuffer + i * lRowSize;
			}
			png_read_image(lPngPtr, lRowPtrs);

			png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
			delete[] lRowPtrs;

			this->data = lImageBuffer;

			return;
		} while (0);

		// error
//ERROR:
//		Log::error("Error while reading PNG file");
		delete[] lRowPtrs; delete[] lImageBuffer;
		if (lPngPtr != NULL) 
		{
			png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr : NULL;
			png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
		}
		this->data = NULL;
	}

	struct my_error_mgr {
		struct jpeg_error_mgr pub;	/* "public" fields */

		jmp_buf setjmp_buffer;	/* for return to caller */
	};

	typedef struct my_error_mgr * my_error_ptr;

	METHODDEF(void)
		my_error_exit (j_common_ptr cinfo)
	{
		/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
		my_error_ptr myerr = (my_error_ptr) cinfo->err;

		/* Always display the message. */
		/* We could postpone this until after returning, if we chose. */
		(*cinfo->err->output_message) (cinfo);

		/* Return control to the setjmp point */
		longjmp(myerr->setjmp_buffer, 1);
	}

	/* Read JPEG image from a memory segment */
	static void init_source (j_decompress_ptr cinfo) {}
	static boolean fill_input_buffer (j_decompress_ptr cinfo)
	{
		//	ERREXIT(cinfo, JERR_INPUT_EMPTY);
		return TRUE;
	}
	static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
	{
		struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;

		if (num_bytes > 0) {
			src->next_input_byte += (size_t) num_bytes;
			src->bytes_in_buffer -= (size_t) num_bytes;
		}
	}
	static void term_source (j_decompress_ptr cinfo) {}
	static void jpeg_mem_src (j_decompress_ptr cinfo, void* buffer, long nbytes)
	{
		struct jpeg_source_mgr* src;

		if (cinfo->src == NULL) {   /* first time for this JPEG object? */
			cinfo->src = (struct jpeg_source_mgr *)
				(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(jpeg_source_mgr));
		}

		src = (struct jpeg_source_mgr*) cinfo->src;
		src->init_source = init_source;
		src->fill_input_buffer = fill_input_buffer;
		src->skip_input_data = skip_input_data;
		src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
		src->term_source = term_source;
		src->bytes_in_buffer = nbytes;
		src->next_input_byte = (JOCTET*)buffer;
	}

	void ImageData::loadByLibJpeg(void* data, int size)
	{
	   /* This struct contains the JPEG decompression parameters and pointers to
		* working space (which is allocated as needed by the JPEG library).
		*/
	   struct jpeg_decompress_struct cinfo;
	   /* We use our private extension JPEG error handler.
		* Note that this struct must live as long as the main JPEG parameter
		* struct, to avoid dangling-pointer problems.
		*/
	   struct my_error_mgr jerr;
	   /* More stuff */
	//   FILE * infile;		/* source file */
	   JSAMPARRAY buffer;		/* Output row buffer */
	   int row_stride;		/* physical row width in output buffer */
	 
	   /* In this example we want to open the input file before doing anything else,
		* so that the setjmp() error recovery below can assume the file is open.
		* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
		* requires it in order to read binary files.
		*/

	//    if ((infile = fopen(m_resource.getPath().c_str(), "rb")) == NULL) {
	// 	   fprintf(stderr, "can't open %s\n", m_resource.getPath());
	// 	   return 0;
	//    }
	 
	   /* Step 1: allocate and initialize JPEG decompression object */
	 
	   /* We set up the normal JPEG error routines, then override error_exit. */
	   cinfo.err = jpeg_std_error(&jerr.pub);
	   jerr.pub.error_exit = my_error_exit;
	   /* Establish the setjmp return context for my_error_exit to use. */
	   if (setjmp(jerr.setjmp_buffer)) {
		 /* If we get here, the JPEG code has signaled an error.
		  * We need to clean up the JPEG object, close the input file, and return.
		  */
		 jpeg_destroy_decompress(&cinfo);
	//     fclose(infile);

		 this->data = NULL;
		 return;
	   }
	   /* Now we can initialize the JPEG decompression object. */
	   jpeg_create_decompress(&cinfo);
	 
	   /* Step 2: specify data source (eg, a file) */
	 
	//   jpeg_stdio_src(&cinfo, infile);
	   jpeg_mem_src(&cinfo, data, size);
	 
	   /* Step 3: read file parameters with jpeg_read_header() */
	 
	   (void) jpeg_read_header(&cinfo, TRUE);
	   /* We can ignore the return value from jpeg_read_header since
		*   (a) suspension is not possible with the stdio data source, and
		*   (b) we passed TRUE to reject a tables-only JPEG file as an error.
		* See libjpeg.doc for more info.
		*/
	 
	   /* Step 4: set parameters for decompression */
	 
	   /* In this example, we don't need to change any of the defaults set by
		* jpeg_read_header(), so we do nothing here.
		*/
	 
	   /* Step 5: Start decompressor */
	 
	   (void) jpeg_start_decompress(&cinfo);
	   /* We can ignore the return value since suspension is not possible
		* with the stdio data source.
		*/
	 
	   /* We may need to do some setup of our own at this point before reading
		* the data.  After jpeg_start_decompress() we have the correct scaled
		* output image dimensions available, as well as the output colormap
		* if we asked for color quantization.
		* In this example, we need to make an output work buffer of the right size.
		*/ 
	   /* JSAMPLEs per row in output buffer */
	   row_stride = cinfo.output_width * cinfo.output_components;
	   /* Make a one-row-high sample array that will go away when done with image */
	   buffer = (*cinfo.mem->alloc_sarray)
 			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	 
	   width = cinfo.output_width;
	   height = cinfo.output_height;
	   int channels = cinfo.output_components;
	   m_format = GL_RGB;
	   unsigned char* pixels = new unsigned char[width * height * channels];

	   /* Step 6: while (scan lines remain to be read) */
	   /*           jpeg_read_scanlines(...); */
	 
	   /* Here we use the library's state variable cinfo.output_scanline as the
		* loop counter, so that we don't have to keep track ourselves.
		*/
	   long counter = 0;
	   while (cinfo.output_scanline < cinfo.output_height) {
		 /* jpeg_read_scanlines expects an array of pointers to scanlines.
		  * Here the array is only one element long, but you could ask for
		  * more than one scanline at a time if that's more convenient.
		  */
		 (void) jpeg_read_scanlines(&cinfo, buffer, 1);
		 /* Assume put_scanline_someplace wants a pointer and sample count. */
	//     put_scanline_someplace(buffer[0], row_stride);
		 memcpy(pixels + counter, buffer[0], row_stride);
		 counter += row_stride;
	   }
	 
	   /* Step 7: Finish decompression */
	 
	   (void) jpeg_finish_decompress(&cinfo);
	   /* We can ignore the return value since suspension is not possible
		* with the stdio data source.
		*/
	 
	   /* Step 8: Release JPEG decompression object */
	 
	   /* This is an important step since it will release a good deal of memory. */
	   jpeg_destroy_decompress(&cinfo);
	 
	   /* After finish_decompress, we can close the input file.
		* Here we postpone it until after no more JPEG errors are possible,
		* so as to simplify the setjmp error logic above.  (Actually, I don't
		* think that jpeg_destroy can do an error exit, but why assume anything...)
		*/
	 //  fclose(infile);
	 
	   /* At this point you may want to check to see whether any corrupt-data
		* warnings occurred (test whether jerr.pub.num_warnings is nonzero).
		*/

	   /* And we're done! */
	   this->data = pixels;
	}

} // null
} // image
} // love
