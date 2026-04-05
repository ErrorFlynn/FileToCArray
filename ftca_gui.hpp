#include <nana/gui/filebox.hpp>

#include <thread>
#include <atomic>
#include <functional>

#include "msgbox.hpp"
#include "file_to_c_array.hpp"


class ftca_form : public themed_form
{
	widgets::Label l_in {*this, "Input file(s):"}, l_out {*this, "Output file(s):"};
	widgets::path_label l_inpath {*this, &inpath}, l_outpath {*this, &outpath};
	widgets::Button btn_gen {*this};
	widgets::Progress prog {*this, this};
	file_to_c_array ftca;

	std::filesystem::path inpath, outpath;
	std::vector<std::filesystem::path> inpaths;
	const char *gen_text {"Generate C array"};
	int shadow_value {0};

	// these operations must be done in the main thread, to prevent the GUI library from deadlocking
	const unsigned WM_MSGBOX {0x8001}, WM_PROG_CAPTION {0x8002};

public:

	ftca_form() : themed_form {std::bind(&ftca_form::apply_theme, this, std::placeholders::_1), nullptr, {}, appear::decorate<appear::minimize>{}}
	{
		subclass_after(WM_MSGBOX, [&](UINT, WPARAM icon, LPARAM title_and_text, LRESULT*)
		{
			auto p_title_and_text {reinterpret_cast<std::pair<std::string, std::string>*>(title_and_text)};
			(msgbox {*this, p_title_and_text->first}.icon(icon) << p_title_and_text->second)();
			delete p_title_and_text;
			return true;
		});

		subclass_after(WM_PROG_CAPTION, [&](UINT, WPARAM prog, LPARAM utf8, LRESULT *)
		{
			reinterpret_cast<nana::progress_ex*>(prog)->caption(*reinterpret_cast<std::string*>(utf8));
			return true;
		});

		center(600, 225);
		caption("File To C Array v2.0");

		div(R"(
			vert margin=20
			< weight=30 <l_in weight=100> <weight=15> <l_inpath> > <weight=20>
			< weight=30 <l_out weight=100> <weight=15> <l_outpath> > <weight=20>
			<prog weight=30> <weight=20> <weight=35 <> <btn_gen weight=180> <>>
		)");

		nana::place &plc {get_place()};

		plc["l_in"] << l_in;
		plc["l_inpath"] << l_inpath;
		plc["l_out"] << l_out;
		plc["l_outpath"] << l_outpath;
		plc["btn_gen"] << btn_gen;
		plc["prog"] << prog;

		collocate();

		btn_gen.caption(gen_text);
		btn_gen.enabled(false);

		ftca.progress_callback([this]
		{
			thread_local std::string prog_caption;
			if(inpaths.size() > 1)
			{
				thread_local std::string xofy;
				if(shadow_value == 0)
				{
					auto it {std::find(inpaths.begin(), inpaths.end(), inpath)};
					if(it != inpaths.end())
						xofy = '[' + std::to_string(it - inpaths.begin() + 1) + " of " + std::to_string(inpaths.size()) + "]   ";
				}
				prog.shadow_progress(100, ++shadow_value);
				prog_caption = xofy + inpath.filename().string() + "   " + std::to_string(shadow_value) + '%';
				SendMessage(hwnd, WM_PROG_CAPTION, reinterpret_cast<WPARAM>(&prog), reinterpret_cast<LPARAM>(&prog_caption));
			}
			else
			{
				prog.inc();
				prog_caption = inpath.filename().string() + "   " + std::to_string(prog.value()) + '%';
				SendMessage(hwnd, WM_PROG_CAPTION, reinterpret_cast<WPARAM>(&prog), reinterpret_cast<LPARAM>(&prog_caption));
			}
		});

		l_inpath.events().click([this]
		{
			nana::filebox fb {handle(), true};
			fb.title("Select one or more files you want to make a C array out of");
			fb.allow_multi_select(true);
			inpaths = fb.show();
			if(inpaths.size() == 1)
			{
				inpath = outpath = inpaths.front();
				outpath.replace_filename("arr_" + outpath.filename().string());
				outpath.replace_extension("hpp");
				l_outpath.update_caption();
				l_inpath.update_caption();
			}
			else if(inpaths.size() > 1)
			{
				inpath = inpaths.front();
				outpath.clear();
				l_inpath.caption(std::to_string(inpaths.size()) + " files");
				l_outpath.caption("arr_<filename>.hpp");
			}
			if(!inpath.empty())
				btn_gen.enabled(true);
		});

		l_outpath.events().click([this]
		{
			nana::filebox fb {handle(), false};
			fb.title("Choose which file you want to save the C array(s) to");
			fb.init_file("out.hpp");
			auto res = fb.show();
			if(!res.empty())
			{
				outpath = res.front();
				if(inpath == outpath)
				{
					outpath.replace_filename("arr_" + outpath.filename().string());
					outpath.replace_extension("hpp");
				}
				l_outpath.update_caption();
				btn_gen.enabled(!inpath.empty() && !outpath.empty());
			}
		});

		btn_gen.events().click([this]
		{
			if(btn_gen.caption() == "Abort")
			{
				ftca.abort();
				btn_gen.caption(gen_text);
				if(inpaths.size() > 1)
				{
					auto it {std::find(inpaths.begin(), inpaths.end(), inpath)};
					if(it == inpaths.end())
						prog.caption("Operation aborted");
					else prog.caption("Operation aborted, " + std::to_string(it - inpaths.begin()) + " of " + std::to_string(inpaths.size()) + " files processed");
				}
				else prog.caption(outpath.filename().string() + " aborted at " + std::to_string(prog.value()) + '%');
			}
			else
			{
				prog.value(0);
				prog.caption("");
				btn_gen.caption("Abort");
				std::thread([this]
				{
					try
					{
						if(inpaths.size() > 1)
						{
							prog.value(0);
							prog.amount(inpaths.size());
							for(const auto &path : inpaths)
							{
								shadow_value = 0;
								inpath = path;
								if(outpath.empty())
								{
									std::filesystem::path outpath {path};
									outpath.replace_filename("arr_" + outpath.filename().string()).replace_extension("hpp");
									ftca.generate(path, outpath);
								}
								else ftca.generate(path, outpath);
								if(!ftca.completed())
									break;
								prog.inc();
							}
							btn_gen.caption(gen_text);
							if(ftca.completed())
								prog.caption(std::to_string(inpaths.size()) + " files processed successfully");
						}
						else
						{
							prog.amount(100);
							ftca.generate(inpath, outpath);
							btn_gen.caption(gen_text);
							if(ftca.completed())
								prog.caption(outpath.filename().string() + " complete");
						}
					}
					catch(std::exception &e)
					{
						btn_gen.caption(gen_text);
						auto p_title_and_text {new std::pair<std::string, std::string> {"Error", e.what()}};
						PostMessage(hwnd, WM_MSGBOX, MB_ICONERROR, reinterpret_cast<WPARAM>(p_title_and_text));
					}
				}).detach();
			}
		});

		events().unload([this]
		{
			if(ftca.working())
				ftca.abort();
		});

		enable_dropfiles(true);
		events().mouse_dropfiles([this](const nana::arg_dropfiles &arg)
		{
			inpaths = arg.files;
			inpath = arg.files.front();
			if(inpaths.size() == 1)
			{
				outpath = inpath;
				outpath.replace_filename("arr_" + outpath.filename().string());
				outpath.replace_extension("hpp");
				l_outpath.update_caption();
				l_inpath.update_caption();
			}
			else if(inpaths.size() > 1)
			{
				outpath.clear();
				l_inpath.caption(std::to_string(inpaths.size()) + " files");
				l_outpath.caption("arr_<filename>.hpp");
			}
			if(!inpath.empty())
				btn_gen.enabled(true);
		});

		l_inpath.caption("Click here to select input files, or drag and drop them onto the window");
		l_outpath.caption("Choose which file you want to save the C array to");

		if(system_supports_darkmode())
			system_theme(true);
	}

private:
	
	bool apply_theme(bool dark)
	{
		using widgets::theme;

		if(dark)
		{
			theme_t def {true};
			def.contrast(.1, true);
			theme::make_dark(def);
		}
		else
		{
			theme_t def {false};
			def.contrast(.1, false);
			theme::make_light(def);
		}

		bgcolor(theme::fmbg);
		refresh_widgets();

		return false; // don't refresh widgets a second time, already done once in this function
	}
};