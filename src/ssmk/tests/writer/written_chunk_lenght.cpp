#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.make_sheet(TESTPATH "/projects/ex1");
	} catch (ssmk::ex::png_error& ex) {
		std::cout << ex.path() << ": " << ex.what() << ": " << ex.description();
		return ssmk::ex::code::bad;
	} catch (ssmk::ex::error& ex) {
		std::cout << ex.what() << ": " << ex.description();
		return ssmk::ex::code::bad;
	} catch (std::exception& ex) {
		std::cout << ex.what();
		return ssmk::ex::code::bad;
	}

	std::cout << "chunks size: " << s.cntx.im.chunks_size << std::endl;
	std::cout << "written " << s.cntx.im.chunks.size() << " chunks" << std::endl;

	return ssmk::ex::code::good;
}
