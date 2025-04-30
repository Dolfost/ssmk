#include <iostream>

#include <ssmk/ssmk.hpp>
#include <ssmk/sprite.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	sm::ssmk s;
	s.set_png_chunk_entry_written_callback(
		[](auto context, auto n) {
			sm::sprite& sprite = *static_cast<sm::sprite*>(context.im.sprites[n]);
			std::cout << sprite << "\n";
		}
	);
	try {
		s.make_sheet(TESTPATH "/projects/ex1");
	} catch (sm::ex::png_error& ex) {
		std::cout << ex.path() << ": " << ex.what() << ": " << ex.description();
		return sm::ex::code::bad;
	} catch (sm::ex::error& ex) {
		std::cout << ex.what() << ": " << ex.description();
		return sm::ex::code::bad;
	} catch (std::exception& ex) {
		std::cout << ex.what();
		return sm::ex::code::bad;
	}

	return sm::ex::code::good;
}
