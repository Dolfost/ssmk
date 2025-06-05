#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/projects/ex1");
	} catch (std::exception& ex) {
		std::cout << ex.what();
		return ssmk::ex::code::bad;
	}

	return ssmk::ex::code::good;
}
