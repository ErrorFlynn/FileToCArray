#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/msgbox.hpp>

#include <fstream>
#include <thread>
#include <atomic>


class ftca_form : public nana::form
{
	nana::label l_in, l_inpath, l_out, l_outpath;
	nana::button btn_in, btn_out, btn_gen;
	nana::progress prog;

	std::filesystem::path inpath, outpath;
	int line_chars_max{120};
	std::atomic<bool> working{false}, abort{false};
	const char *gen_text{"Generate C array"};

public:

	ftca_form() : nana::form(nana::API::make_center(600, 185), appear::decorate<appear::minimize>())
	{
		nana::place &plc{get_place()};
		const auto bg{nana::colors::white};

		caption("File To C Array v1.1");
		bgcolor(bg);

		div("vert margin=15 < weight=20 <l_in weight=70><weight=15><l_inpath><weight=15><btn_in weight=20> >"
			"<weight=15> < weight=20 <l_out weight=70><weight=15><l_outpath><weight=15><btn_out weight=20> >"
			"<weight=15> <prog weight=30> <weight=15> <btn_gen weight=40>"
		);

		l_in.create(handle(), true);
		l_in.caption("Input file:");
		l_in.bgcolor(bg);
		l_in.text_align(nana::align::right, nana::align_v::center);
		plc["l_in"] << l_in;

		l_inpath.create(handle(), true);
		l_inpath.bgcolor(nana::color{"#eef6ee"});
		l_inpath.text_align(nana::align::center, nana::align_v::center);
		l_inpath.caption("Select a file you want to make a C array out of.");
		plc["l_inpath"] << l_inpath;

		btn_in.create(handle(), true);
		btn_in.caption("...");
		btn_in.bgcolor(bg);
		plc["btn_in"] << btn_in;

		l_out.create(handle(), true);
		l_out.caption("Output file:");
		l_out.bgcolor(bg);
		l_out.text_align(nana::align::right, nana::align_v::center);
		plc["l_out"] << l_out;

		l_outpath.create(handle(), true);
		l_outpath.bgcolor(nana::color{"#eef6ee"});
		l_outpath.text_align(nana::align::center, nana::align_v::center);
		l_outpath.caption("Choose which file you want to save the C array to.");
		plc["l_outpath"] << l_outpath;

		btn_out.create(handle(), true);
		btn_out.caption("...");
		btn_out.bgcolor(bg);
		plc["btn_out"] << btn_out;

		btn_gen.create(handle(), true);
		btn_gen.caption(gen_text);
		btn_gen.bgcolor(nana::colors::white);
		btn_gen.fgcolor(nana::color{"#406080"});
		btn_gen.typeface({typeface().name(), 15, nana::paint::font::font_style{800}});
		btn_gen.events().mouse_enter([this] { btn_gen.fgcolor(nana::color{"#557799"}); });
		btn_gen.events().mouse_leave([this] { btn_gen.fgcolor(nana::color{"#406080"}); });
		btn_gen.enabled(false);
		plc["btn_gen"] << btn_gen;

		prog.create(handle(), true);
		prog.scheme().gradient_bgcolor = nana::color{"#f5f5f5"};
		prog.scheme().gradient_fgcolor = nana::color{"#b7d3db"};
		prog.scheme().background = nana::color{"#e4e4e4"};
		prog.scheme().foreground = nana::color{"#64a3b5"};
		prog.typeface({typeface().name(), 11, nana::paint::font::font_style{800}});
		nana::drawing{prog}.draw([this] (nana::paint::graphics &graph)
		{
			auto caption{prog.caption()};
			if(!caption.empty())
			{
				auto tsize{graph.text_extent_size(caption)};
				auto gsize{graph.size()};
				nana::point pos{0,0};
				if(tsize.width < gsize.width)
					pos.x = gsize.width/2 - tsize.width/2;
				if(tsize.height < gsize.height)
					pos.y = gsize.height/2 - tsize.height/2;
				graph.string(pos, caption, nana::colors::white);
			}
		});
		plc["prog"] << prog;

		collocate();

		btn_in.events().click([this]
		{
			nana::filebox fb{handle(), true};
			fb.title("Select a file you want to make a C array out of");
			auto res = fb.show();
			if(!res.empty())
			{
				inpath = res.front();
				if(inpath == outpath)
				{
					outpath.replace_filename("arr_" + outpath.filename().u8string());
					outpath.replace_extension("hpp");
					label_path_caption(l_outpath, outpath);
				}
				label_path_caption(l_inpath, inpath);
				btn_gen.enabled(!inpath.empty() && !outpath.empty());
			}
		});

		btn_out.events().click([this]
		{
			nana::filebox fb{handle(), false};
			fb.title("Choose which file you want to save the C array to.");
			fb.init_file("out.hpp");
			auto res = fb.show();
			if(!res.empty())
			{
				outpath = res.front();
				if(inpath == outpath)
				{
					outpath.replace_filename("arr_" + outpath.filename().u8string());
					outpath.replace_extension("hpp");
					label_path_caption(l_outpath, outpath);
				}
				label_path_caption(l_outpath, outpath);
				btn_gen.enabled(!inpath.empty() && !outpath.empty());
			}
		});

		btn_gen.events().click([this]
		{
			if(working)
			{
				abort = true;
				while(working) std::this_thread::sleep_for(std::chrono::milliseconds{75});
				abort = false;
				std::filesystem::remove(outpath);
				btn_gen.caption(gen_text);
				prog.value(0);
			}
			else
			{
				btn_gen.caption("Abort");
				std::thread([this]
				{
					try { file_to_c_array(); }
					catch(std::exception &e)
					{
						btn_gen.caption(gen_text);
						working = false;
						nana::msgbox mbox{handle(), "Error"};
						mbox << e.what();
						mbox.icon(nana::msgbox::icon_t::icon_error);
						mbox.show();
					}
				}).detach();
			}
		});

		events().unload([this]
		{
			if(working)
			{
				abort = true;
				while(working) std::this_thread::sleep_for(std::chrono::milliseconds{75});
			}
		});
	}

private:

	std::string make_valstr(char *buf, unsigned width) const
	{
		union uint_t
		{
			uint8_t uint8;
			uint16_t uint16;
			uint32_t uint32;
			uint64_t uint64;
		};

		auto pval = reinterpret_cast<uint_t*>(buf);
		std::stringstream ss;

		switch(width)
		{
		case 8:
			if(pval->uint64 > 999'999'999'999u)
				ss << std::hex << std::showbase << pval->uint64;
			else ss << pval->uint64;
			break;

		case 4:
			ss << pval->uint32;
			break;

		case 2:
			ss << pval->uint16;
			break;

		case 1:
			ss << std::to_string(pval->uint8);
			break;
		}

		return ss.str();
	}

	void file_to_c_array()
	{
		working = true;
		if(!std::filesystem::exists(inpath))
			throw(std::exception{"Input file does not exist!"});
		auto insize = std::filesystem::file_size(inpath);
		unsigned width = sizeof(uint64_t);
		while(insize % width) width /= 2;
		auto buf{std::make_unique<char[]>(width)};
		const int prog_steps{568};
		prog.caption("");
		prog.amount(prog_steps);
		prog.value(0);
		uintmax_t prog_step{0}, step_treshold{0};
		if(insize >= 568)
		{
			prog_step = insize / prog_steps;
			step_treshold = prog_step;
		}

		std::ifstream infile{inpath, std::ios::binary};
		if(!infile.good()) throw(std::exception{"Failed to open the input file!"});
		std::ofstream outfile{outpath};
		if(!outfile.good()) throw(std::exception{"Failed to open the output file!"});

		auto name{"arr_" + inpath.filename().u8string()};
		for(auto &c : name) if(!isalnum(c)) c = '_';
			
		outfile << "#include <cstdint>\n\n";
		if(width > 1) outfile << "// length in bytes: " << insize << "\n";
		outfile << "const uint" << std::to_string(width*8) << "_t " << name << '[' << std::to_string(insize/width) << "] = {";
		if(line_chars_max) outfile << "\n\t";
		size_t line_chars{0};

		while(infile.read(buf.get(), width))
		{
			auto elstr{make_valstr(buf.get(), width) + ','};
			if(line_chars_max)
			{
				line_chars += elstr.size();
				if(line_chars > line_chars_max)
				{
					line_chars = elstr.size();
					outfile << "\n\t";
				}
			}
			outfile << elstr;
			if(prog_step && infile.tellg() >= step_treshold)
			{
				step_treshold += prog_step;
				prog.inc();
			}
			if(abort) { working = false; return; }
			if(outfile.bad()) throw(std::exception{"Failed trying to write to the output file!"});
		}
		
		if(infile.bad()) throw(std::exception{"Failed trying to read the input file!"});

		outfile.seekp(static_cast<std::streamoff>(outfile.tellp())-1);
		if(line_chars_max) outfile << '\n';
		outfile << "};";
		btn_gen.caption(gen_text);
		prog.caption("Operation completed.");
		prog.value(prog_steps);
		working = false;
	}

	void label_path_caption(nana::label &l, const std::filesystem::path &p)
	{
		l.caption(p);
		int offset = 0;
		while(l.measure(1234).width > l.size().width)
		{
			offset += 6;
			l.caption(L"..." + std::wstring{p.c_str()}.substr(offset));
		}
	}
};