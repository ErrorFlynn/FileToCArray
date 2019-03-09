#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/paint/image.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/filebox.hpp>

#include <fstream>

#include "bmp_array_example.hpp"

int main()
{
	using namespace nana;
	using namespace std;

	form fm{API::make_center(1024, 768), appear::decorate<appear::minimize, appear::maximize, appear::sizable>()};
	fm.div("vert <text> < weight=283 <> <pic weight=283> <> > <weight=20> < weight=30 <> <btn weight=283> <> > <>");
	fm.bgcolor(color{"#fbf9f5"});

	label text{fm};
	text.caption("<size=16 green>If all went well and the image loaded, you should see a cat below, or a green square otherwise.</>");
	text.text_align(align::center, align_v::center);
	text.format(true);

	paint::image img;
	img.open(arr_cat_283x283_bmp, sizeof arr_cat_283x283_bmp);
	picture pic{fm};
	pic.load(img);
	pic.bgcolor(colors::light_green);

	button btn{fm, "Save array to bmp file"};
	btn.bgcolor(colors::white);
	btn.events().click([&]
	{
		filebox fb{fm, false};
		fb.title("Choose which bmp file you want to save the array to");
		fb.init_file("cat_283x283");
		fb.add_filter("Windows Bitmap (.bmp)", "*.bmp");
		auto res = fb.show();
		if(!res.empty())
		{
			ofstream file{res.front(), ios::binary};
			file.write(reinterpret_cast<const char*>(arr_cat_283x283_bmp), sizeof arr_cat_283x283_bmp);
		}
	});

	fm["text"] << text;
	fm["pic"] << pic;
	fm["btn"] << btn;
	fm.collocate();
	fm.show();
	exec();
}