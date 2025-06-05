#include <ssmk/sprite.hpp>
  
#include <png.h>

namespace ssmk {

sprite::png_info::~png_info() {
	png_destroy_read_struct(
		&image, &info, nullptr
	);
}

}
