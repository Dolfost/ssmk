#include <iostream>
#include <ssmk/context.hpp>

#include <ssmk/sprite.hpp>

#include <calgo/opt/packing2D.hpp>

#include <png.h>

#include <ostream>

namespace ssmk {

const std::unordered_map<std::string, context::config::output::packing::algorithm> 
	context::config::output::packing::algorithm_text {
	{ "firstFit", context::config::output::packing::algorithm::first_fit },
	{ "nextFit",  context::config::output::packing::algorithm::next_fit },
	{ "treeFit",  context::config::output::packing::algorithm::tree_fit },
};

const std::unordered_map<std::string, context::config::output::packing::ordering> 
	context::config::output::packing::order_text {
	{ "none",       context::config::output::packing::ordering::none },
	{ "decreasing", context::config::output::packing::ordering::decreasing },
	{ "increasing", context::config::output::packing::ordering::increasing },
};

const std::unordered_map<std::string, context::config::output::packing::sorting_metric> 
	context::config::output::packing::metric_text {
	{ "width",     context::config::output::packing::sorting_metric::width },
	{ "height",    context::config::output::packing::sorting_metric::height },
	{ "maxSide",   context::config::output::packing::sorting_metric::max_side },
	{ "minSide",   context::config::output::packing::sorting_metric::min_side },
	{ "perimeter", context::config::output::packing::sorting_metric::perimeter },
	{ "area",      context::config::output::packing::sorting_metric::area },
};

const std::unordered_map<std::string, context::config::output::png_info::interlacing> 
	context::config::output::png_info::interlacing_text {
	{ "none",     context::config::output::png_info::interlacing::none },
	{ "adam7",     context::config::output::png_info::interlacing::adam7 },
};

context::intermediate::~intermediate() {
	//  TODO: fix memory dealocation
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
