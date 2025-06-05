#include <ssmk/writer.hpp>

#include <ssmk/exceptions.hpp>
#include <ssmk/sprite.hpp>

#include <filesystem>

namespace ssmk {

#define SSMK_ADD_SPRITE(P) \
	sprite* sprt = new sprite;  \
	sprt->set_path(P);  \
	m_context.im.sprites.push_back(sprt);  \
	if (m_file_found_callback)  \
		m_file_found_callback(m_context); \

void writer::find_files() {
	for (auto ptr: cntx.im.sprites) {
		delete static_cast<sprite*>(ptr);
	}
	cntx.im.sprites.clear();
	for (const auto& in: cntx.conf.in.files) {
		if (std::filesystem::is_directory(in)) {
			for (const auto& entry: std::filesystem::recursive_directory_iterator(in)) {
				if (entry.is_regular_file() and entry.path().extension() == ".png") {
					SSMK_ADD_SPRITE(entry.path())
				}
			}
		} else if (std::filesystem::is_regular_file(in) and in.extension() == ".png") {
			SSMK_ADD_SPRITE(in)
		}
	}
	if (cntx.im.sprites.empty())
		SM_EX_THROW(error, no_sprites_found)
	if (m_files_found_callback)
		m_files_found_callback(m_context);
}

#undef SSMK_ADD_SPRITE

}
