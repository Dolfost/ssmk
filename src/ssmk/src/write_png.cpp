#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>
#include <ssmk/version.hpp>

#include <png.h>

namespace ssmk {

void writer::write_png() {
	std::FILE* ofile = 
		std::fopen(cntx.conf.out.file.string().c_str(), "wb");
	if (not ofile)
		SM_EX_THROW(png_error, png_failed_to_open_for_writting, cntx.conf.out.file);

	png_init_io(cntx.im.png, ofile);

	png_write_info(cntx.im.png, cntx.im.info);

	if (m_image_row_written_callback) {
		int passes = png_set_interlace_handling(cntx.im.png);
		for (std::size_t p = 0; p < passes; p++)
			for (std::size_t r = 0; r < cntx.im.height; r++) {
				png_write_rows(
					cntx.im.png,
					cntx.im.rows,
					1
				);
				// unfortunatetly we cannot use png_set_write_status_fn...
				m_image_row_written_callback(
					m_context, r, p, passes
				);
			}
	} else
		png_write_image(cntx.im.png, cntx.im.rows);

	png_write_end(cntx.im.png, nullptr);

	// cleanup
	for (std::size_t i = 0; i < cntx.im.height; i++)
		delete[] cntx.im.rows[i];
	delete[] cntx.im.rows; 
	cntx.im.rows = nullptr;
	delete (png_color_16p)cntx.im.background;
	cntx.im.background = nullptr;
	png_destroy_write_struct(&cntx.im.png, &cntx.im.info);
	cntx.im.png = nullptr; cntx.im.info = nullptr;
	std::fclose(ofile);

	if (m_png_written_callback) 
		m_png_written_callback(m_context);
}

}
