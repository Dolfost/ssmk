#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/projects/short_RGB_array");
	} catch (ssmk::ex::config_wrong_field_type& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": "  << ex.path() << std::endl;
		return ssmk::ex::code::good;
	} catch (ssmk::ex::config_field_error& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": " << ex.field() << std::endl;
		return ssmk::ex::code::bad;
	} catch (ssmk::ex::error& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": " << std::endl;
		return ssmk::ex::code::bad;
	}

	std::cout << "No exceptions";

	return ssmk::ex::code::bad;
}
