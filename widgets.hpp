#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/scroll.hpp>
#include <nana/paint/text_renderer.hpp>

#include <variant>
#include <iostream>
#include <map>
#include <Windows.h>

#include "progress_ex.hpp"
#include "types.hpp"

#pragma warning(disable : 4267)
#ifdef small
	#undef small
#endif

#ifdef min
	#undef min
#endif


namespace widgets
{
	class theme
	{
		static bool dark;
		static double shade;

	public:

		theme() { make_light({}); }

		static nana::color nimbus, fmbg, Label_fg, Text_fg, Text_fg_error, cbox_fg, btn_bg, btn_fg, /*path_bg, */path_fg, path_link_fg, 
			sep_bg, tbfg, tbkw, tbkw_id, tbkw_special, tbkw_warning, tbkw_error, gpbg, lb_headerbg, title_fg, overlay_fg, border,
			tb_selbg, tb_selbg_unfocused, expcol_fg, tree_selbg, tree_selfg, tree_hilitebg, tree_hilitefg, tree_selhilitebg, tree_selhilitefg,
			tree_parent_node, tree_expander, tree_expander_hovered, tree_bg, tree_key_fg, tree_val_fg, list_check_highlight_fg,
			list_check_highlight_bg, msg_label_fg, lbbg, lbselfg, lbselbg, lbhilite, lbfg;
		static std::string gpfg;

		static void import(const theme_t &src);
		static void make_light(const theme_t &src);
		static void make_dark(const theme_t &src);
		static void copy(theme_t &dest);

		static bool is_dark() { return dark; }
		static void contrast(double factor, theme_t &src);
		static double contrast() { return shade; }
	};


	class Label : public nana::label
	{

	public:

		Label() : label() {}

		Label(nana::window parent, std::string_view text, bool dpi_adjust = false)
		{
			create(parent, text, dpi_adjust);
		}

		void create(nana::window parent, std::string_view text, bool dpi_adjust = false);
	};


	class path_label : public nana::label
	{
		using variant = std::variant<std::filesystem::path*, std::wstring*>;
		variant v;		
		bool is_path {false};

	public:

		path_label() : label() {}
		path_label(nana::window parent, const variant var) { create(parent, var); }

		void create(nana::window parent, const variant var);
		void source_path(const variant var) { v = var; update_caption(); }
		void update_caption();
		void refresh_theme();
	};


	class Button : public nana::button
	{
		nana::paint::image img, img_disabled;

		struct btn_bg : nana::element::element_interface
		{
			bool draw(nana::paint::graphics &g, const nana::color &bg,
				const nana::color &fg, const nana::rectangle &r, nana::element_state state) override;
		};

	public:

		Button() : button() {}

		Button(nana::window parent, std::string_view text = "", bool small = false)
		{
			create(parent, text, small);
		}

		void create(nana::window parent, std::string_view text = "", bool small = false);
		void refresh_theme();
		void image(const void *data, unsigned size);
		void image_disabled(const void *data, unsigned size);
		void enable(bool state);
	};


	class Progress : public nana::progress_ex
	{
	public:

		Progress() : progress_ex() {}

		Progress(nana::window parent, nana::form *pform = nullptr) : progress_ex {parent, {}, true, pform}
		{
			create(parent);
		}

		void create(nana::window parent);
		void refresh_theme();
	};
}