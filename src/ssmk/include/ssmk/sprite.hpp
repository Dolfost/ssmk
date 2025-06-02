#ifndef _SSMK_SSMK_SPRITE_HPP_
#define _SSMK_SSMK_SPRITE_HPP_

#include <calgo/opt/packing2D.hpp>

#include <png.h>

#include <filesystem>
#include <iosfwd>

namespace sm {

class sprite: public ca::opt::Box2D<std::size_t> {
public:
	struct png_info {
		png_structp image = nullptr;
		png_infop info = nullptr;
		std::size_t pos = 0;
		~png_info();
	};
public:
	using ca::opt::Box2D<std::size_t>::Box2D;

	 void set_path(const std::filesystem::path& path) {
		m_path = path;
	}
	const std::filesystem::path& path() const {
		return m_path;
	}
	png_info& png() {
		return m_png;
	}

	friend std::ostream& operator<<(std::ostream& os, const sprite& s) {
		return os << static_cast<ca::opt::Box2D<std::size_t>>(s)
		<< " in " << s.m_path;
	};
private:
	std::filesystem::path m_path;
	png_info m_png;
};

}

#endif // !_SSMK_SSMK_IMAGE_HPP_
