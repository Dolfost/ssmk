#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

#include <filesystem>
#include <optional>

#include <toml++/toml.hpp>

namespace ssmk {

#define RETHROW_TOML_EX() \
throw ssmk::ex::toml_error( \
	context.file, \
	{ ex.source().begin.line, ex.source().begin.column }, \
	{ ex.source().begin.line, ex.source().begin.column }, \
	std::string(ex.description()) \
);

#define THROW_TOML(E, OBJ) \
throw ssmk::ex::E( \
	*OBJ->source().path, \
	{ OBJ->source().begin.line, OBJ->source().begin.column }, \
	{ OBJ->source().begin.line, OBJ->source().begin.column } \
);

void sm_base::read_config(ssmk::context& context) {
	context.directory = 
		std::filesystem::absolute(context.directory);
	if (not std::filesystem::is_directory(context.directory))
		SM_EX_THROW(file_error, source_directory_not_found, context.directory)

	for (const auto& filename: context::config::config_filenames) {
		if (std::filesystem::is_regular_file(context.directory / filename)) {
			context.file = context.directory / filename;
			break;
		}
	}
	if (context.file.empty())
		SM_EX_THROW(file_error, config_not_found, context.file)

	toml::table table;
	try {
		table = toml::parse_file(context.file.string());
	} catch (const toml::parse_error& ex) {
		RETHROW_TOML_EX()
	}

	toml::table* inputTable = table["input"].as_table();
	if (not inputTable)
		SM_EX_THROW(config_field_error, config_no_input_table, context.file, "input")

	toml::array* filesArray = (*inputTable)["files"].as_array();
	if (not filesArray)
		SM_EX_THROW(config_field_error, config_no_input_file_array, context.file, "input.files")
	filesArray->for_each([&context](auto&& e) {
		if constexpr (toml::is_string<decltype(e)>) {
			std::filesystem::path path = e.as_string()->get();
			if (path.is_relative())
				path = context.directory / path;
			if (not (std::filesystem::is_regular_file(path) or std::filesystem::is_directory(path)))
				SM_EX_THROW(config_field_error, not_a_file_or_directory, path, "input.files")

			context.conf.in.files.push_back(path);
		} else {
			SM_EX_THROW(config_wrong_field_type, config_wrong_field_type, context.file, "input.files", "array", "array<string>")
		}
	});

	toml::table* outputTable = table["output"].as_table();
	if (not outputTable)
		SM_EX_THROW(config_field_error, config_no_output_table, context.file, "output")

	std::optional<std::string> outputFile = (*outputTable)["file"].value<std::string>();
	if (not outputFile)
		SM_EX_THROW(config_field_error, config_no_output_file, context.file, "output.file")

	context.conf.out.file = context.directory / *outputFile;

	// output.packing
	toml::table* packingTable = (*outputTable)["packing"].as_table();
	if (packingTable) {
		std::optional<std::string> algorithm = (*packingTable)["algorithm"].value<std::string>();
		if (algorithm) {
			context.conf.out.pack.alg = 
				context::config::output::packing::algorithm::none;
			for (const auto& [k, v]: context::config::output::packing::algorithm_text) {
				if (k == *algorithm) {
					context.conf.out.pack.alg = v;
					break;
				}
			}
			if (context.conf.out.pack.alg == context::config::output::packing::algorithm::none)
				SM_EX_THROW(
					config_unexpected_field_value, config_unknown_packing_algorithm,
					context.file, "output.packing.algorithm", *algorithm,
					context::config::output::packing::algorithm_text
				)
		}
		std::optional<std::string> order = (*packingTable)["order"].value<std::string>();
		if (order) {
			for (const auto& [k, v]: context::config::output::packing::order_text) {
				if (k == *order) {
					context.conf.out.pack.order = v;
					break;
				}
			}
		}
		std::optional<std::string> metric = (*packingTable)["metric"].value<std::string>();
		if (metric) {
			context.conf.out.pack.metric = 
				context::config::output::packing::sorting_metric::none;
			for (const auto& [k, v]: context::config::output::packing::metric_text) {
				if (k == *metric) {
					context.conf.out.pack.metric = v;
					break;
				}
			}
			if (context.conf.out.pack.metric == context::config::output::packing::sorting_metric::none)
				SM_EX_THROW(
					config_unexpected_field_value, config_unknown_packing_metric,
					context.file, "output.packing.metric", *metric,
					context::config::output::packing::metric_text
				)
		}
		if (context.conf.out.pack.alg == context::config::output::packing::algorithm::tree_fit and 
			context.conf.out.pack.order != context::config::output::packing::ordering::decreasing)
				SM_EX_THROW(
					config_exclusive_field_values, config_increasing_tree_fit_packing,
					context.file, "output.packing.algorithm", "treeFit",
					"output.packing.order", *order
				)
		std::optional<long long> k = (*packingTable)["k"].value<long long>();
		if (k) {
			if (*k <= 0)
				SM_EX_THROW(
					config_unexpected_field_value, 
					config_unexpected_field_value, 
					context.file,
					"output.packing.k",
					std::to_string(*k),
					std::string(">0")
				);
			context.conf.out.pack.k = *k;
		}
	}

	// output.png
	toml::table* pngTable = (*outputTable)["png"].as_table();
	if (pngTable) {
		std::optional<bool> opaque = (*pngTable)["opaque"].value<bool>();
		if (opaque) 
			context.conf.out.png.opaque = *opaque;
		std::optional<std::string> interlacing = (*pngTable)["interlacing"].value<std::string>();
		if (interlacing) {
			for (const auto& [k, v]: context::config::output::png_info::interlacing_text) {
				if (k == *interlacing) {
					context.conf.out.png.inter = v;
					break;
				}
			}
		}
		toml::array* backgroundArray = (*pngTable)["background"].as_array();
		if (backgroundArray) {
			if (backgroundArray->size() != 3)
				SM_EX_THROW(
					config_wrong_field_type, 
					config_wrong_field_type,
					context.file,
					"output.png.background",
					"array[" + std::to_string(backgroundArray->size()) + "]",
					"array<double>[3]"
				)
			std::size_t idx = 0;
			backgroundArray->for_each([&context, &idx](auto&& e) {
				if constexpr (toml::is_number<decltype(e)>) {
					if (e.get() < 0 or e.get() > 1) {
						SM_EX_THROW(
							config_unexpected_field_value, 
							config_not_RGB, 
							context.file, 
							"output.png.background", 
							std::to_string(e.get()), std::string("[0;1]")
						)
					}
					context.conf.out.png.background[idx++] = e.get();
				} else {
					SM_EX_THROW(config_wrong_field_type, config_wrong_field_type, context.file, "output.png.background", "array", "array<doublg>")
				}
			});
		}
		std::optional<int> compression = (*pngTable)["compression"].value<int>();
		if (compression) {
			if (*compression < 0 or *compression > 9)
				SM_EX_THROW(
					config_unexpected_field_value, 
					config_unknown_compression_level, 
					context.file, 
					"output.png.compression", 
					std::to_string(*compression), std::string("[0;9]")
				)
					context.conf.out.png.compression = *compression;
		}

	}
}

#undef THROW_TOML 
#undef RETHROW_TOML_EX

}
