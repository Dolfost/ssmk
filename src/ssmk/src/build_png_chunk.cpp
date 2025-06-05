#include <ssmk/writer.hpp>

#include <ssmk/sprite.hpp>

#include <cstring>
#include <cstdint>

#include <png.h>

#ifdef _WIN32 // for htonl
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace ssmk {

void writer::build_png_chunk() {
	std::vector<std::string> paths;
	paths.reserve(m_context.im.sprites.size());

	std::size_t cur_chunk_size = 0, entries_processed = 0;
	m_context.im.chunks.clear();
	m_context.im.chunks_size = sizeof(std::uint32_t); // n | ...
	for (const auto& p: m_context.im.sprites) {
		paths.push_back(
			std::filesystem::path(
				static_cast<sprite*>(p)->path().lexically_relative(
					m_context.directory
				)
			).replace_extension("").generic_string()
		);

		// ... | x | y | w | h | strlen | str | ...
		std::size_t size = paths.back().length() + 1 + 4*sizeof(std::uint32_t);
		m_context.im.chunks_size += size;
		if (size + cur_chunk_size > m_context.im.max_chunk_size) {
			m_context.im.chunks.push_back(std::vector<std::uint8_t>(cur_chunk_size));
			m_context.im.chunks_entry_count.push_back(entries_processed);
			cur_chunk_size = sizeof(std::uint32_t);
			entries_processed = 0;
		} else {
			cur_chunk_size += size;
			entries_processed++;
		}
	}
	m_context.im.chunks.push_back(std::vector<std::uint8_t>(cur_chunk_size));
	m_context.im.chunks_entry_count.push_back(entries_processed);

	const bool call = (bool)m_png_chunk_entry_written_callback;
	std::uint32_t x, y, w, h, slen, n = m_context.im.sprites.size();
	entries_processed = 0;
	for (std::size_t chunk_no = 0; chunk_no < m_context.im.chunks_entry_count.size(); chunk_no++) {
		std::size_t at = sizeof(std::uint32_t);
		*(std::uint32_t*)m_context.im.chunks[chunk_no].data() = htonl(n);
		std::size_t entries = entries_processed;
		for (std::size_t e = entries_processed; e < entries + m_context.im.chunks_entry_count[chunk_no]; e++) {
			const sprite& s = *static_cast<sprite*>(m_context.im.sprites[e]);
			const auto cur_size = paths[e].length() + 1 + 4*sizeof(std::uint32_t);
			slen = paths[e].length() + 1;

			x = htonl(s.x()), y = htonl(s.y()), w = htonl(s.size().width()), h = htonl(s.size().height());
			std::memcpy(m_context.im.chunks[chunk_no].data() + at, &x, sizeof(std::uint32_t)); // x
			at += sizeof(std::uint32_t);
			std::memcpy(m_context.im.chunks[chunk_no].data() + at, &y, sizeof(std::uint32_t)); // y
			at += sizeof(std::uint32_t);
			std::memcpy(m_context.im.chunks[chunk_no].data() + at, &w, sizeof(std::uint32_t)); // w
			at += sizeof(std::uint32_t);
			std::memcpy(m_context.im.chunks[chunk_no].data() + at, &h, sizeof(std::uint32_t)); // h
			at += sizeof(std::uint32_t);
			std::memcpy(m_context.im.chunks[chunk_no].data() + at, paths[e].c_str(), slen); // str
			at += slen;

			entries_processed++;

			if (call)
				m_png_chunk_entry_written_callback(m_context, e);
		}
	}

	m_context.im.png_chunks.clear();
	m_context.im.png_chunks.resize(m_context.im.chunks.size());
	for (std::size_t i = 0; i < m_context.im.png_chunks.size(); i++) {
		std::memcpy(m_context.im.png_chunks[i].name, chunk_name, 5);
		m_context.im.png_chunks[i].data = (png_bytep)m_context.im.chunks[i].data();
		m_context.im.png_chunks[i].size = m_context.im.chunks[i].size();
		m_context.im.png_chunks[i].location = PNG_HAVE_PLTE; // write chunk before IDAT
	}
	png_set_unknown_chunks(
		(png_structp)m_context.im.png, 
		(png_infop)m_context.im.info, 
		(png_unknown_chunkp)m_context.im.png_chunks.data(), m_context.im.png_chunks.size()
	);
	png_set_keep_unknown_chunks(
		(png_structp)m_context.im.png, 
		PNG_HANDLE_CHUNK_ALWAYS, 
		(png_bytep)chunk_name, 1
	);

	if (m_png_chunk_built_callback) 
		m_png_chunk_built_callback(m_context);
}

}
