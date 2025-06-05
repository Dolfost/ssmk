#ifndef _SSMK_SSMK_CONTEXT_HPP_
#define _SSMK_SSMK_CONTEXT_HPP_

#include <png.h>

#include <vector>
#include <array>
#include <iosfwd>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace ca::opt {

template<typename T> 
class Box2D;

}

namespace ssmk {

struct traits {
	using size_type = std::size_t;
	struct png {
		using number_type = std::uint32_t;
		using byte_type = std::uint8_t;
	};
};

struct context {
	std::filesystem::path directory;
	std::filesystem::path file;
	struct config {
		constexpr static const std::array config_filenames = {
			"ssmk.toml", "sprite.toml", "spritesheet.toml"
		};
		struct input {
			std::vector<std::filesystem::path> files;
		} in;
		struct output { 
			std::filesystem::path file;
			struct packing {
				enum class algorithm {
					none,
					first_fit,
					next_fit,
					tree_fit,
				} alg = algorithm::tree_fit;
				const static std::unordered_map<std::string, algorithm> algorithm_text;
				enum class ordering {
					none,
					decreasing,
					increasing
				} order = ordering::decreasing;
				const static std::unordered_map<std::string, ordering> order_text;
				enum class sorting_metric {
					none,
					width,
					height,
					max_side,
					min_side,
					perimeter,
					area,
				} metric = sorting_metric::min_side;
				const static std::unordered_map<std::string, sorting_metric> metric_text;
				traits::size_type k = 1;
			} pack;
			struct png_info {
				bool opaque = false;
				enum class interlacing {
					none, 
					adam7,
				} inter = interlacing::none;
				const static std::unordered_map<std::string, interlacing> interlacing_text;
				std::array<double, 3> background = {0, 0, 0};
				int compression = -1;
			} png;
		} out;
	} conf;

	struct png_chunk {
		constexpr static const char* name = "ssMk";
		constexpr static const char* key = "Software";
		constexpr static const char* version_key = "ssmk Version";
		constexpr static const char* text = "ssmk";
	};
	friend std::ostream& operator<<(std::ostream& os, const context& c);
};

class sm_base {
};

}

#endif // !_SSMK_SSMK_CONTEXT_HPP_
