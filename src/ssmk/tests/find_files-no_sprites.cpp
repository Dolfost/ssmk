#include <iostream>
#include <ssmk/writer.hpp>
#include <ssmk/exceptions.hpp>

int main(int argc, char** argv) {
	ssmk::writer s;
	try {
		s.read_config(TESTPATH "/projects/no_sprites" );
		s.find_files();
	} catch (ssmk::ex::config_field_error& ex) {
		std::cout << ex.what() << ": " << ex.description() << ": " << ex.path() << ": " << ex.field() << std::endl;
		return ex.code() == ssmk::ex::code::no_sprites_found ? ssmk::ex::code::good : ssmk::ex::code::bad;
	} catch (ssmk::ex::error& ex) {
		std::cout << ex.what() << ": " << ex.description() << std::endl;
		return ex.code() == ssmk::ex::code::no_sprites_found ? ssmk::ex::code::good : ssmk::ex::code::bad;
	} catch (std::exception& ex) {
		return ssmk::ex::code::bad;
	}

	std::cout << "sprites:\n";
	for (auto const& x : s.context.im.sprites) {
		std::cout << '\t' << x << '\n';
	}
	

	return ssmk::ex::code::bad;
}
