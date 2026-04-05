#include "types.hpp"


void theme_t::make_default(bool dark)
{
	using namespace nana;

	if(dark)
	{
		nimbus = color {"#cde"};
		fmbg_raw = color {"#2c2b2b"};
		tbfg = lbfg  = color {"#f7f7f4"};
		btn_bg_raw = color {"#2e2d2d"};
		Label_fg = btn_fg = cbox_fg = color {"#e6e6e3"};
		Text_fg = color {"#def"};
		Text_fg_error = color {"#f99"};
		path_fg = colors::white;
		sep_bg = color {"#777"};
		expcol_fg = color {"#999"};
		gpfg = "0xE4D6BA";
		gpfg_clr = color {"#E4D6BA"};
		lb_headerbg_raw = color {"#525658"};
		title_fg = nana::color {"#cde"};
		path_link_fg = color {"#E4D6BA"};
		tbkw = color {"#b5c5d5"};
		tbkw_id = color {"#ccc"};
		tbkw_special = color {"#F0B0A0"};
		tbkw_warning = color {"#EEBF00"};
		tbkw_error = color {"#CA86E3"};
		overlay_fg = border = color {"#666"};
		tb_selbg = color {"#95443B"};
		tree_selbg = color {"#354F5C"};
		tree_selfg = color {"#569EBD"};
		tree_hilitebg = color {"#28353D"};
		tree_hilitefg = color {"#48606A"};
		tree_parent_node = color {"#ddd"};
		tree_expander = colors::white;
		tree_expander_hovered = color {"#ade"};
		tree_key_fg = color {"#ebebe3"};
		tree_val_fg = color {"#d8e8ff"};
		list_check_highlight_fg = color {"#bbb"};
		list_check_highlight_bg = color {"#6D4941"};
		msg_label_fg = Label_fg;
		lbselfg = color {"#AC4F44"};
		lbselbg = color {"#B05348"};
		lbhilite = color {"#544"};
		contrast(shade, true);
	}
	else
	{
		nimbus = color {"#60c8fd"};
		fmbg_raw = btn_bg_raw = colors::white;
		tbfg = lbfg = colors::black;
		Label_fg = color {"#569"};
		Text_fg = color {"#575"};
		Text_fg_error = color {"#a55"};
		cbox_fg = color {"#458"};
		btn_fg = color {"#67a"};
		path_fg = color {"#354"};
		path_link_fg = color {"#789"};
		sep_bg = color {"#c5c5c5"};
		expcol_fg = color {"#aaa"};
		gpfg = "0x81544F";
		gpfg_clr = color {"#81544F"};
		lb_headerbg_raw = color {"#f1f2f4"};
		title_fg = color {"#789"};
		tbkw = color {"#272"};
		tbkw_id = color {"#777"};
		tbkw_special = color {"#722"};
		tbkw_warning = color {"#B96C00"};
		tbkw_error = color {"#aa2222"};
		overlay_fg = color {"#bcc0c3"};
		border = color {"#9CB6C5"};
		tb_selbg = color {"#5695D3"};
		tree_selbg = color {"#eaf0f6"};
		tree_selfg = color {"#97aeb4"};
		tree_hilitebg = color {"#e7eef4"};
		tree_hilitefg = color {"#d7dee4"};
		tree_parent_node = color {"#909090"};
		tree_expander = colors::black;
		tree_expander_hovered = colors::deep_sky_blue;
		tree_key_fg = color {"#459"};
		tree_val_fg = color {"#474"};
		list_check_highlight_fg = color {"#888"};
		list_check_highlight_bg = color {"#D5D8e0"};
		msg_label_fg = color {"#457"};
		lbselfg = color {"#c7dEe4"};
		lbselbg = color {"#d0e2e8"};
		lbhilite = color {"#eee"};
		contrast(shade, false);
	}
}


void theme_t::contrast(double factor, bool dark)
{
	using namespace nana;
	shade = std::clamp(factor, .0, .3);
	if(dark)
	{
		fmbg = fmbg_raw.blend(colors::black, shade);
		lbbg = fmbg.blend(colors::black, .035);
		gpbg = tree_bg = fmbg.blend(colors::white, .035);
		btn_bg = btn_bg_raw.blend(colors::black, shade);
		lb_headerbg = lb_headerbg_raw.blend(colors::black, shade);
		tb_selbg_unfocused = tb_selbg.blend(colors::black, .3);
		tree_selhilitebg = tree_selbg.blend(colors::white, .06);
		tree_selhilitefg = tree_selfg.blend(colors::white, .2);
	}
	else
	{
		fmbg = lbbg = tree_bg = fmbg_raw.blend(colors::light_grey, .3 - shade);
		btn_bg = btn_bg_raw.blend(colors::light_grey, .3 - shade);
		gpbg = fmbg.blend(colors::black, .025);
		lb_headerbg = lb_headerbg_raw.blend(colors::white, shade);
		tb_selbg_unfocused = tb_selbg.blend(nana::colors::white, .3);
		tree_selhilitebg = tree_selbg.blend(colors::gray, .1);
		tree_selhilitefg = tree_selfg.blend(colors::gray, .1);
	}
}