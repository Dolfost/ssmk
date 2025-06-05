#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/" TESTFILE);
	} catch (ssmk::ex::file_error& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": "  << ex.path() << std::endl;
		return ssmk::ex::code::good;
	}

	return ssmk::ex::code::bad;
}
