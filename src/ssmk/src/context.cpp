#include <iostream>
#include <ssmk/context.hpp>

#include <ssmk/sprite.hpp>

#include <calgo/opt/packing2D.hpp>

#include <png.h>

#include <ostream>

namespace ssmk {

const std::unordered_map<std::string, context::output::packing::algorithm> 
	context::output::packing::algorithm_text {
	{ "firstFit", context::output::packing::algorithm::first_fit },
	{ "nextFit",  context::output::packing::algorithm::next_fit },
	{ "treeFit",  context::output::packing::algorithm::tree_fit },
};

const std::unordered_map<std::string, context::output::packing::ordering> 
	context::output::packing::order_text {
	{ "none",       context::output::packing::ordering::none },
	{ "decreasing", context::output::packing::ordering::decreasing },
	{ "increasing", context::output::packing::ordering::increasing },
};

const std::unordered_map<std::string, context::output::packing::sorting_metric> 
	context::output::packing::metric_text {
	{ "width",     context::output::packing::sorting_metric::width },
	{ "height",    context::output::packing::sorting_metric::height },
	{ "maxSide",   context::output::packing::sorting_metric::max_side },
	{ "minSide",   context::output::packing::sorting_metric::min_side },
	{ "perimeter", context::output::packing::sorting_metric::perimeter },
	{ "area",      context::output::packing::sorting_metric::area },
};

const std::unordered_map<std::string, context::output::png_info::interlacing> 
	context::output::png_info::interlacing_text {
	{ "none",     context::output::png_info::interlacing::none },
	{ "adam7",     context::output::png_info::interlacing::adam7 },
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
	SV(in.files);

	S(conf.directory);
	S(out.file);

	SE(out.pack.alg);
	SE(out.pack.order);
	SE(out.pack.metric);
	S(out.pack.k);
	SE(out.png.inter);
	S(out.png.opaque);
	SV(out.png.background);

	S(conf.file);

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
