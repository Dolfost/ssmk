#include <ssmk/writer.hpp>

#include <ssmk/exceptions.hpp>

#include <cstdio>
#include <cstring>

#include <png.h>

namespace ssmk {

void writer::read_sheet_header() {
	static const std::size_t sigLen = 8;
	png_byte signature[sigLen];
	std::memset(signature, 0, sigLen);

	std::FILE* file = nullptr;
	if (not (file = std::fopen(m_context.file.string().c_str(), "rb")))
		SM_EX_THROW(png_error, png_failed_to_open_for_reading, m_context.file);

	std::fread(signature, 1, sigLen, file);
	if (not png_check_sig(signature, 8)) {
		std::fclose(file);
		SM_EX_THROW(png_error, png_bad_signature, m_context.file);
	}

	cntx.im.png = png_create_read_struct(
		PNG_LIBPNG_VER_STRING,
		nullptr, nullptr, nullptr
	);

	if (not cntx.im.png) {
		SM_EX_THROW(error, png_could_not_create_read_structure);
	}

	cntx.im.info = png_create_info_struct(cntx.im.png);
	if (not cntx.im.info) {
		png_destroy_read_struct(&cntx.im.png, nullptr, nullptr);
		SM_EX_THROW(error, png_could_not_create_info_structure);
	}

	png_set_keep_unknown_chunks(
		cntx.im.png, 
		PNG_HANDLE_CHUNK_ALWAYS, 
		(const png_bytep)context::png_chunk::name, 
		1
	);

	png_init_io(m_context.im.png, file);
	png_set_sig_bytes(m_context.im.png, sigLen);
	png_read_info(m_context.im.png, m_context.im.info);

	png_uint_32 width = 0, height = 0;
	int depth = 0, color = 0;
	png_get_IHDR(
		m_context.im.png, m_context.im.info, 
		&width, &height, &depth, &color, 
		nullptr, nullptr, nullptr
	);

	// or's current image with all input
	cntx.im.color_present   = bool(color & PNG_COLOR_MASK_COLOR);
	cntx.im.palette_present = bool(color & PNG_COLOR_MASK_PALETTE);
	cntx.im.alpha_present   = bool(color & PNG_COLOR_MASK_ALPHA);
	cntx.im.tRNS_present    = bool(png_get_valid(m_context.im.png, m_context.im.info, PNG_INFO_tRNS));
	cntx.im.depth           = depth;
	cntx.im.color           = color;

	if (m_sheet_header_read_callback)
		m_sheet_header_read_callback(m_context);
}

}
