#include <ssmk/ssmk.hpp>
#include <ssmk/exceptions.hpp>

#include <filesystem>

namespace sm {

#define SSMK_ADD_SPRITE(P) \
	Sprite sprite;  \
	sprite.setPath(P);  \
	context.sprites.push_back(std::move(sprite));  \
	if (s_fileFoundCallback)  \
		s_fileFoundCallback({*this, context.sprites.back()}); \

void Ssmk::findFiles() {
	for (const auto& in: context.inputFiles) {
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
	if (context.sprites.empty())
		SM_EX_THROW(Error, NoSpritesFound)
	if (s_filesFoundCallback)
		s_filesFoundCallback({*this, context.sprites});
}

#undef SSMK_ADD_SPRITE

}