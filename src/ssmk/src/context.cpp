#include <ssmk/context.hpp>

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

}
