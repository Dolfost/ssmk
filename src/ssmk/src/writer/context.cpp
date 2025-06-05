#include <iostream>
#include <ssmk/writer.hpp>

#include <ssmk/sprite.hpp>

#include <calgo/opt/packing2D.hpp>

#include <png.h>

#include <ostream>

namespace ssmk {

writer::context::intermediate::~intermediate() {
	for (auto ptr: sprites)
		delete static_cast<sprite*>(ptr);
	if (rows != nullptr)
		for (std::size_t i = 0; i < height; i++)
			delete[] rows[i];
	delete[] rows; 
	png_destroy_write_struct(&png, &info);
}

#define S(PROP) os << #PROP ": " << c.PROP << std::endl;
#define SE(PROP) os << #PROP ": " << static_cast<std::underlying_type<decltype(c.PROP)>::type>(c.PROP) << std::endl;
#define SV(PROP) \
os << #PROP ": \n"; \
for (const auto& r : c.PROP) { \
	os << "  " << r << '\n'; \
}

std::ostream& operator<<(std::ostream& os, const context& c) {
	S(directory);
	S(conf.out.file);

	SE(conf.out.pack.alg);
	SE(conf.out.pack.order);
	SE(conf.out.pack.metric);
	S(conf.out.pack.k);
	SE(conf.out.png.inter);
	S(conf.out.png.opaque);
	SV(conf.out.png.background);

	S(file);

	return os;
}

std::ostream& operator<<(std::ostream& os, const writer::context& c) {
	operator<<(os, static_cast<const context&>(c));

	os << "im.sprites" ": \n";
	for (const auto& r : c.im.sprites) {
		os << "  " << *static_cast<sprite*>(r) << '\n';
	}
	S(im.depth);
	S(im.color_present);
	S(im.width);
	S(im.height);

	return os;
}

#undef S
#undef SV
#undef SE

}
