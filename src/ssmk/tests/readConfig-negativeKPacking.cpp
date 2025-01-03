#include <iostream>
#include <ssmk/ssmk.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	sm::Ssmk s;
	s.context.config.directory = TESTPATH "/projects/negativeKPacking";
	try {
		s.readConfig();
	} catch (sm::ex::ConfigUnexpectedFieldValue& ex) {
		std::cout << ex.path() <<  ": " << ex.what() 
			<< ": " << ex.description() << ": " << ex.field() 
			<< " = " << ex.value() << ", expected " << ex.expected();
		return sm::ex::code::Good;
	}

	std::cout << "General";

	return sm::ex::code::Bad;
}
