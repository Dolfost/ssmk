#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/projects/not_RGB");
	} catch (ssmk::ex::config_unexpected_field_value& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": "  << ex.path() << std::endl;
		return ex.code() == ssmk::ex::code::config_not_RGB ? ssmk::ex::code::good : ssmk::ex::code::bad;
	} catch (ssmk::ex::error& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": " << std::endl;
		return ssmk::ex::code::bad;
	}

	std::cout << "No exceptions";

	return ssmk::ex::code::bad;
}
