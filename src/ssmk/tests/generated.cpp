#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.make_sheet(TESTPATH "/projects/generated");
	} catch (ssmk::ex::error& ex) {
		std::cout << ex.what() << ": " << ex.description();
		return ssmk::ex::code::bad;
	} catch (std::exception& ex) {
		std::cout << ex.what();
		return ssmk::ex::code::bad;
	}

	return ssmk::ex::code::good;
}
