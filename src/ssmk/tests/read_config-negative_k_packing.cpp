#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/projects/negative_k_packing");
	} catch (ssmk::ex::config_unexpected_field_value& ex) {
		std::cout << ex.path() <<  ": " << ex.what() 
			<< ": " << ex.description() << ": " << ex.field() 
			<< " = " << ex.value() << ", expected " << ex.expected() << std::flush;
		return ssmk::ex::code::good;
	} catch (ssmk::ex::config_field_error& ex) {
		std::cout << ex.path() <<  ": " << ex.what() 
			<< ": " << ex.description() << ": " << ex.field() << std::endl;
	}

	std::cout << "General";

	return ssmk::ex::code::bad;
}
