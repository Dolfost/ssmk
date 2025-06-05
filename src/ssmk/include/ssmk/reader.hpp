#ifndef SSMK_READER_HPP
#define SSMK_READER_HPP

#include <ssmk/context.hpp>

#include <map>
#include <string>

#define CALLBACK(NAME, ...) \
	private: \
		std::function<void(const context_type& __VA_OPT__(,) __VA_ARGS__)> m_##NAME##_callback; \
	public: \
		void set_##NAME##_callback(const std::function<void(const context_type& __VA_OPT__(,) __VA_ARGS__)>& callback) { \
			m_##NAME##_callback = callback; \
		} \
		const std::function<void(const context_type& __VA_OPT__(,) __VA_ARGS__)>& NAME##_callback() const { \
			return m_##NAME##_callback; \
		} \
		std::function<void(const context_type& __VA_OPT__(,) __VA_ARGS__)>& NAME##_callback() { \
			return m_##NAME##_callback; \
		}

namespace ssmk {

class reader: public sm_base {
public:
	struct context: public ssmk::context {
		struct intermediate {
			std::vector<ca::opt::Box2D<traits::size_type>*> sprites;

			traits::size_type color_present;
			traits::size_type alpha_present;
			traits::size_type palette_present;
			traits::size_type tRNS_present;
			static const traits::size_type max_chunk_size = 80;
			std::vector<std::vector<traits::png::byte_type>> chunks;
			std::vector<traits::size_type> chunks_entry_count;

			traits::size_type width, height; ///< Output dimentions
			png_structp png;  ///< Output png data structure
			png_infop info; ///< Output png info structure
			png_color_16p background; ///< Output png background structure
			png_bytepp rows; ///< Output buffer
			int color; ///< Output color mode
			int depth; ///< Output color depth
			std::vector<png_unknown_chunk> png_chunks;
			traits::size_type chunks_size;
			~intermediate();
			intermediate() {
				color_present = 0; alpha_present = 0; palette_present = 0; 
				tRNS_present = 0; width = 0; height = 0; background = nullptr;
				rows = nullptr; color = 0; depth = 0;
			}
		} im;
	};
	using context_type = ssmk::reader::context;

public:
	reader(const context_type& context = {}): m_context(context) {};
	context_type& cntx = m_context;

public:
	struct sheet_entry {
		traits::size_type x, y, width, height;
	};
	class sheet: public std::map<std::string, sheet_entry> {
	protected:
		size_type m_width, m_height;
		int m_png_color, m_depth;
		bool m_color_present, m_alpha_present; 
		png_bytep* m_data;
		png_structp m_png;
		png_infop m_info;
	};

	void read_sheet_header();
	CALLBACK(
		sheet_header_read
	);

	void extract_png_chunk();
	CALLBACK(
		chunk_entry_extracted,
		traits::size_type i
	);
	CALLBACK(
		chunk_extracted,
	);

	void read_sheet_png_data();
	CALLBACK(
		image_row_read,
		traits::size_type row,
		traits::size_type pass,
		traits::size_type passes
	)
	CALLBACK(
		image_read
	)

	/*
	 * will use m_context.conf.file or find config file in
	 * m_context.conf.directory and extract from it the output.file field and 
	 * read the png and build the std::map<std::string, sprite>
	*/
	template<typename P> typename 
	std::enable_if<std::is_assignable<std::filesystem::path, P>::value>::type read_sheet(P&& path) {
		m_context = context_type();
		if (std::filesystem::is_regular_file(path))
			m_context.conf.out.file = std::filesystem::absolute(path);
		else 
			read_config(std::forward<P>(path)); // will throw at any file error
		read_sheet_header();
		extract_png_chunk();
		read_sheet_png_data();
	}

	/*
	 * will write read sheet tree to specified directory
	*/
	void extract_sheet() {
	}

private:
	context_type m_context;
};

}

#endif // !SSMK_READER_HPP
