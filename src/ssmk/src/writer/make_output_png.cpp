#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>
#include <ssmk/version.hpp>

#ifdef CALGO_HAVE_VALGRIND
#include <valgrind/memcheck.h>
#else 
#define VALGRIND_MAKE_MEM_DEFINED(addr, len) ((void)0)
#define VALGRIND_MAKE_MEM_UNDEFINED(addr, len) ((void)0)
#endif

#include <png.h>

namespace ssmk {

void writer::make_output_png() {
	cntx.im.color = 0;
	// expand all to rgb if required
	if (cntx.im.color_present)
		cntx.im.color |= PNG_COLOR_MASK_COLOR;
	if ((cntx.im.alpha_present or cntx.im.tRNS_present) and not 
		cntx.conf.out.png.opaque)
		cntx.im.color |= PNG_COLOR_MASK_ALPHA;

	int interlacing;
	switch (cntx.conf.out.png.inter) {
		case context::config::output::png_info::interlacing::none:
			interlacing = PNG_INTERLACE_NONE;
			break;
		case context::config::output::png_info::interlacing::adam7:
			interlacing = PNG_INTERLACE_ADAM7;
			break;
	}

	cntx.im.png = png_create_write_struct(
		PNG_LIBPNG_VER_STRING,
		nullptr, nullptr, nullptr
	);

	if (not cntx.im.png) {
		SM_EX_THROW(error, png_could_not_create_write_structure);
	}

	cntx.im.info = png_create_info_struct(cntx.im.png);
	if (not cntx.im.info) {
		png_destroy_write_struct(&cntx.im.png, nullptr);
		SM_EX_THROW(error, png_could_not_create_info_structure);
	}

	png_set_IHDR(
		cntx.im.png, cntx.im.info, cntx.im.width, cntx.im.height,
		cntx.im.depth, cntx.im.color, interlacing,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT //  TODO: add filter setting to toml config
	);

	cntx.im.background = new png_color_16;
	if (not cntx.im.background)
		SM_EX_THROW(
			png_error, 
			png_could_not_allocate_background_color, 
			cntx.conf.out.file
		);
	const int max = 1 << cntx.im.depth; // max pixel component value
	if (cntx.im.color & PNG_COLOR_MASK_COLOR) { // if result is color image
		cntx.im.background->red   = max * cntx.conf.out.png.background[0];
		cntx.im.background->green = max * cntx.conf.out.png.background[1];
		cntx.im.background->blue  = max * cntx.conf.out.png.background[2];
	} else { // if output will be grayscale, convert color to grayscale
		cntx.im.background->gray = (
			6968  * max * cntx.conf.out.png.background[0] + 
			23434 * max * cntx.conf.out.png.background[1] + 
			2366  * max * cntx.conf.out.png.background[2]
		) / 32768;
	}
	if (not (cntx.im.color & PNG_COLOR_MASK_ALPHA)) // set background color only if output is transparent
		png_set_bKGD(
			cntx.im.png, cntx.im.info,
			cntx.im.background
		);

	png_set_compression_level(
		cntx.im.png, cntx.conf.out.png.compression
	);

	// put comments
	png_text text[2] { 
		{
			PNG_TEXT_COMPRESSION_NONE,
			(char*)context::png_chunk::key,     // sorry
			(char*)context::png_chunk::text // sorry
		},
		{
			PNG_TEXT_COMPRESSION_NONE,
			(char*)context::png_chunk::version_key,  // sorry
			(char*)ssmk::version.full.c_str() // sorry
		}
	};
	png_set_text(cntx.im.png, cntx.im.info, text, 2);

	// allocate result buffer
	cntx.im.rows = new png_bytep[cntx.im.height];
	png_bytepp& rows = cntx.im.rows;
	if (not rows) {
		png_destroy_write_struct(&cntx.im.png, &cntx.im.info);
		SM_EX_THROW(png_error, png_could_not_allocate_output_rows, cntx.conf.out.file);
	}
	const auto row_size = png_get_rowbytes(cntx.im.png, cntx.im.info);
	for (std::size_t i = 0; i < cntx.im.height; i++) {
		rows[i] = new png_byte[row_size];
		VALGRIND_MAKE_MEM_DEFINED(rows[i], row_size);
		
		if (not rows[i]) {
			png_destroy_write_struct(&cntx.im.png, &cntx.im.info);
			for (std::size_t j = 0; j < i; j++)
				delete rows[j];
			delete rows;
			rows = nullptr;
			SM_EX_THROW(png_error, png_could_not_allocate_output_rows, cntx.conf.out.file);
		}
	}

}

}
