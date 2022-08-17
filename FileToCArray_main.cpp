#include "FileToCArray.hpp"

int main(int argc, char** argv)
{
	ftca_form fm;
#if defined(__WIN32) || defined(_MSC_VER)
	nana::API::window_icon(fm.handle(), nana::paint::image(argv[0]));
#endif
	fm.show();
	nana::exec();
}