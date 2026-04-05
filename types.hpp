#pragma once

#include <nana/gui.hpp>


struct theme_t
{
	double shade {0}; // value that determines the contrast for some bg colors
	nana::color nimbus, fmbg, Label_fg, Text_fg, Text_fg_error, cbox_fg, btn_bg, btn_fg, path_fg, path_link_fg, lbfg,
		sep_bg, tbfg, tbkw, tbkw_id, tbkw_special, tbkw_warning, tbkw_error, gpbg, lb_headerbg, title_fg, overlay_fg, border,
		tb_selbg, tb_selbg_unfocused, expcol_fg, tree_selbg, tree_selfg, tree_hilitebg, tree_hilitefg, tree_selhilitebg, tree_selhilitefg,
		tree_parent_node, tree_expander, tree_expander_hovered, tree_bg, tree_key_fg, tree_val_fg, list_check_highlight_fg,
		list_check_highlight_bg, msg_label_fg, fmbg_raw, btn_bg_raw, lb_headerbg_raw, lbbg, gpfg_clr, lbselbg, lbselfg, lbhilite;
	std::string gpfg;

	bool operator==(theme_t const &) const = default;

	theme_t(bool dark = false) { make_default(dark); }
	void make_default(bool dark = false);
	void contrast(double factor, bool dark);
};