#include <iostream>

#include <ssmk/writer.hpp>
#include <ssmk/sprite.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	// be aware that [](auto context, auto n) { ... } will lead to [](ssmk::context
	// context, auto n) and it will be copied by value and destroyed when lambdas
	// scope end is reached
	s.set_png_chunk_entry_written_callback(
		[](const auto& context, auto n) {
			ssmk::sprite& sprite = *static_cast<ssmk::sprite*>(context.im.sprites[n]);
			std::cout << sprite << "\n";
		}
	);

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

	return ssmk::ex::code::good;
}
