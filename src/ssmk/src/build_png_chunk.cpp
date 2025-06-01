#include <ssmk/ssmk.hpp>

#include <ssmk/sprite.hpp>

#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cstdint>

#include <png.h>

#ifdef _WIN32 // for htonl
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

namespace sm {

void ssmk::build_png_chunk() {
	m_context.im.chunk_size = sizeof(std::uint32_t); // n | ...
	std::vector<std::string> paths;
	paths.reserve(m_context.im.sprites.size());
	for (const auto& p: m_context.im.sprites) {
		paths.push_back(
			std::filesystem::path(
				static_cast<sprite*>(p)->path().lexically_relative(
					m_context.conf.directory
				)
			).replace_extension("").generic_string()
		);
		// ... | x | y | w | h | strlen | str | ...
		m_context.im.chunk_size += paths.back().length() + 1 + 4*sizeof(std::uint32_t);
	}

	m_context.im.chunk = (std::uint8_t*)std::malloc(m_context.im.chunk_size);

	const bool call = (bool)m_png_chunk_entry_written_callback;
	size_type at = sizeof(std::uint32_t);
	std::uint32_t x, y, w, h, slen, n = m_context.im.sprites.size();
	std::uint8_t* chunk = m_context.im.chunk;
	*(std::uint32_t*)chunk = htonl(n);
	for (std::vector<std::string>::size_type i = 0; i < paths.size(); i++) {
		const sprite& s = *static_cast<sprite*>(m_context.im.sprites[i]);

		x = htonl(s.x()), y = htonl(s.y()), w = htonl(s.size().width()), h = htonl(s.size().height());
		std::memcpy(chunk + at, &x, sizeof(std::uint32_t)); // x
		at += sizeof(std::uint32_t);
		std::memcpy(chunk + at, &y, sizeof(std::uint32_t)); // y
		at += sizeof(std::uint32_t);
		std::memcpy(chunk + at, &w, sizeof(std::uint32_t)); // w
		at += sizeof(std::uint32_t);
		std::memcpy(chunk + at, &h, sizeof(std::uint32_t)); // h
		at += sizeof(std::uint32_t);

		slen = paths[i].length() + 1;

		std::memcpy(chunk + at, paths[i].c_str(), slen);
		at += slen;
		
		if (call)
			m_png_chunk_entry_written_callback(m_context, i);
	}

	// I guess that libpng frees it by itself
	png_unknown_chunkp png_chunk = (png_unknown_chunkp)std::malloc(sizeof(png_unknown_chunk));
	std::memcpy(png_chunk->name, chunk_name, 5);
	png_chunk->data = (png_bytep)chunk;
	png_chunk->size = m_context.im.chunk_size;
	png_chunk->location = PNG_HAVE_PLTE; // write chunk before IDAT
	png_set_unknown_chunks((png_structp)m_context.im.png, (png_infop)m_context.im.info, png_chunk, 1);
	png_set_keep_unknown_chunks(
		(png_structp)m_context.im.png, 
		PNG_HANDLE_CHUNK_ALWAYS, 
		(png_bytep)chunk_name, 1
	);

	if (m_png_chunk_built_callback) 
		m_png_chunk_built_callback(m_context);
}

}
