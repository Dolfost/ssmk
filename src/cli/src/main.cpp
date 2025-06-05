#include <ssmk/cli/application.hpp>
#include <ssmk/cli/version.hpp>

int main(int argc, const char** argv) {
	ssmk::cli::Application app("ssmk");
	app.setVersion(ssmk::cli::version.full);
	app.setDescription("Sprite sheet make program");
	return app.run(argc, argv);
}
