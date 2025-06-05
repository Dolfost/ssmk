#include <ssmk/cli/application.hpp>

#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

#include <cstdlib>

namespace ssmk::cli {

int Application::run(int argc, const char** argv) {
	int parseResult = parse(argc, argv);
	if (parseResult != EXIT_SUCCESS)
		return parseResult;

	std::cout << "CLI context:\n" << context << std::endl;

	int code = EXIT_SUCCESS;

	a_ssmk.set_images_packed_callback([](const writer& sm) {
		std::cout << "SSMK context (after packing):\n" << sm.cntx << std::endl;
	});

	try {
		a_ssmk.make_sheet(std::filesystem::absolute(context.sourceDirectory));
	} catch (ssmk::ex::config_field_error& ex) {
		std::cerr << ex.what() << ": " << ex.description() << ": " << ex.field() << std::endl;
		code = ex.code();
	} catch (ssmk::ex::error& ex) {
		std::cerr << ex.what() << ": " << ex.description() << std::endl;
		code = ex.code();
	} catch (std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		code = EXIT_FAILURE;
	}

	std::cout << "SSMK context:\n" << a_ssmk.cntx << std::endl;

	return code;
}

}
