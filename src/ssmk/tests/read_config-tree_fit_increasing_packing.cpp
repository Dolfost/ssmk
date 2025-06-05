#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/projects/tree_fit_increasing_packing");
	} catch (ssmk::ex::config_exclusive_field_values& ex) {
		return ex.code() == ssmk::ex::code::config_increasing_tree_fit_packing ? ssmk::ex::code::good : ssmk::ex::code::bad;
	}

	std::cout << "General";

	return ssmk::ex::code::bad;
}
