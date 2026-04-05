#include "widgets.hpp"
#include <codecvt>
#include <Windows.h>

using namespace widgets;

bool theme::dark {false};
double theme::shade {0};
std::string theme::gpfg;

nana::color theme::nimbus, theme::fmbg, theme::Label_fg, theme::Text_fg, theme::Text_fg_error, theme::cbox_fg, theme::btn_bg,  
	theme::btn_fg, theme::path_fg, theme::path_link_fg, theme::sep_bg, theme::tbfg, theme::tbkw, theme::tbkw_id, 
	theme::tbkw_special, theme::tbkw_warning, theme::tbkw_error, theme::gpbg, theme::lb_headerbg, theme::title_fg, theme::overlay_fg, 
	theme::border, theme::tb_selbg, theme::tb_selbg_unfocused, theme::expcol_fg, theme::tree_selbg, theme::tree_selfg, theme::tree_hilitebg, 
	theme::tree_hilitefg, theme::tree_selhilitebg, theme::tree_selhilitefg, theme::tree_parent_node, theme::tree_expander, 
	theme::tree_expander_hovered, theme::tree_bg, theme::tree_key_fg, theme::tree_val_fg, theme::list_check_highlight_fg, 
	theme::list_check_highlight_bg, theme::msg_label_fg, theme::lbbg, theme::lbselfg, theme::lbselbg, theme::lbhilite, theme::lbfg;


void widgets::theme::import(const theme_t &src)
{
	nimbus = src.nimbus;
	fmbg = src.fmbg;
	lbbg = src.lbbg;
	btn_bg = src.btn_bg;
	gpbg = src.gpbg;
	tbfg = src.tbfg;
	Label_fg = src.Label_fg;
	Text_fg = src.Text_fg;
	Text_fg_error = src.Text_fg_error;
	cbox_fg = src.cbox_fg;
	btn_fg = src.btn_fg;
	path_fg = src.path_fg;
	path_link_fg = src.path_link_fg;
	sep_bg = src.sep_bg;
	expcol_fg = src.expcol_fg;
	gpfg = src.gpfg;
	title_fg = src.title_fg;
	tbkw = src.tbkw;
	tbkw_id = src.tbkw_id;
	tbkw_special = src.tbkw_special;
	tbkw_warning = src.tbkw_warning;
	tbkw_error = src.tbkw_error;
	overlay_fg = src.overlay_fg;
	border = src.border;
	tb_selbg = src.tb_selbg;
	tb_selbg_unfocused = src.tb_selbg_unfocused;
	tree_selbg = src.tree_selbg;
	tree_selfg = src.tree_selfg;
	tree_hilitebg = src.tree_hilitebg;
	tree_hilitefg = src.tree_hilitefg;
	tree_selhilitebg = src.tree_selhilitebg;
	tree_selhilitefg = src.tree_selfg;
	tree_parent_node = src.tree_parent_node;
	tree_expander = src.tree_expander;
	tree_expander_hovered = src.tree_expander_hovered;
	tree_bg = src.tree_bg;
	tree_key_fg = src.tree_key_fg;
	tree_val_fg = src.tree_val_fg;
	list_check_highlight_fg = src.list_check_highlight_fg;
	list_check_highlight_bg = src.list_check_highlight_bg;
	msg_label_fg = src.msg_label_fg;
	lb_headerbg = src.lb_headerbg;
	lbselbg = src.lbselbg;
	lbselfg = src.lbselfg;
	lbhilite = src.lbhilite;
	lbfg = src.lbfg;
}

void theme::copy(theme_t &dest)
{
	dest.nimbus = nimbus;
	dest.fmbg = fmbg;
	dest.lbbg = lbbg;
	dest.btn_bg = btn_bg;
	dest.gpbg = gpbg;
	dest.tbfg = tbfg;
	dest.Label_fg = Label_fg;
	dest.Text_fg = Text_fg;
	dest.Text_fg_error = Text_fg_error;
	dest.cbox_fg = cbox_fg;
	dest.btn_fg = btn_fg;
	dest.path_fg = path_fg;
	dest.path_link_fg = path_link_fg;
	dest.sep_bg = sep_bg;
	dest.expcol_fg = expcol_fg;
	dest.gpfg = gpfg;
	dest.title_fg = title_fg;
	dest.tbkw = tbkw;
	dest.tbkw_id = tbkw_id;
	dest.tbkw_special = tbkw_special;
	dest.tbkw_warning = tbkw_warning;
	dest.tbkw_error = tbkw_error;
	dest.overlay_fg = overlay_fg;
	dest.border = border;
	dest.tb_selbg = tb_selbg;
	dest.tb_selbg_unfocused = tb_selbg_unfocused;
	dest.tree_selbg = tree_selbg;
	dest.tree_selfg = tree_selfg;
	dest.tree_hilitebg = tree_hilitebg;
	dest.tree_hilitefg = tree_hilitefg;
	dest.tree_selhilitebg = tree_selhilitebg;
	dest.tree_selfg = tree_selhilitefg;
	dest.tree_parent_node = tree_parent_node;
	dest.tree_expander = tree_expander;
	dest.tree_expander_hovered = tree_expander_hovered;
	dest.fmbg = tree_bg;
	dest.tree_key_fg = tree_key_fg;
	dest.tree_val_fg = tree_val_fg;
	dest.list_check_highlight_fg = list_check_highlight_fg;
	dest.list_check_highlight_bg = list_check_highlight_bg;
	dest.msg_label_fg = msg_label_fg;
	dest.lb_headerbg = lb_headerbg;
	dest.lbselbg = lbselbg;
	dest.lbselfg = lbselfg;
	dest.lbhilite = lbhilite;
	dest.lbfg = lbfg;
}


void theme::make_light(const theme_t &src)
{
	dark = false;
	import(src);
}


void theme::make_dark(const theme_t &src)
{
	dark = true;
	import(src);
}


void theme::contrast(double factor, theme_t &src)
{
	shade = std::clamp(factor, .0, .3);
	src.contrast(shade, dark);
	import(src);
}


void Label::create(nana::window parent, std::string_view text, bool dpi_adjust)
{
	label::create(parent);
	caption(text);
	fgcolor(theme::Label_fg);
	typeface(nana::paint::font_info {"Segoe UI", 12 - (double)(nana::API::screen_dpi(true) > 96) * 2 * dpi_adjust});
	text_align(nana::align::right, nana::align_v::center);
	nana::API::effects_bground(*this, nana::effects::bground_transparent(0), 0);
	events().expose([this] { fgcolor(theme::Label_fg); });
}


void path_label::create(nana::window parent, const variant var)
{
	label::create(parent);
	v = var;
	is_path = std::holds_alternative<std::filesystem::path*>(v);
	refresh_theme();
	if(is_path) typeface(nana::paint::font_info {"Tahoma", 10});
	else typeface(nana::paint::font_info {"Segoe UI", 11, {700}});
	text_align(nana::align::center, nana::align_v::center);
	nana::API::effects_bground(*this, nana::effects::bground_transparent(0), 0);
	nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::over);
	events().expose([this] { refresh_theme(); });
	events().resized([this] { update_caption(); });

	nana::drawing {*this}.draw([this](nana::paint::graphics &g)
	{
		if(theme::is_dark())
			g.rectangle(false, theme::fmbg.blend(nana::colors::white, .3));
		else g.rectangle(false, nana::color {"#9aa"});
	});
}


void path_label::update_caption()
{
	const std::wstring wstr {is_path ? std::get<std::filesystem::path*>(v)->wstring() : *std::get<std::wstring*>(v)};
	if(!size().empty())
	{
		caption(wstr);
		int offset {0};
		while(measure(1234).width > size().width)
		{
			offset += 1;
			if(wstr.size() - offset < 4)
			{
				caption("");
				return;
			}
			caption(L"..." + wstr.substr(offset));
		}
	}
}


void path_label::refresh_theme()
{
	scheme().activated = theme::nimbus;
	if(is_path)
		fgcolor(caption().find("!!!") == -1 ? theme::path_fg : theme::tbkw_special);
	else fgcolor(theme::path_link_fg);
}


bool Button::btn_bg::draw(nana::paint::graphics &g, const nana::color &bg,
	const nana::color &fg, const nana::rectangle &r, nana::element_state state)
{
	auto from {bg}, to {bg.blend(nana::color {"#e6e6e8"}, .15)};
	if(state != nana::element_state::pressed)
		g.gradual_rectangle(r, from, to, true);
	else g.gradual_rectangle(r, to, from, true);
	g.rectangle(r, false, static_cast<nana::color_rgb>(0x7f7f7f));
	return true;
}


void Button::create(nana::window parent, std::string_view text, bool small)
{
	button::create(parent);
	caption(text);
	refresh_theme();
	typeface(nana::paint::font_info {"Segoe UI", small ? 11e0 : 14, {800}});
	enable_focus_color(false);
	events().expose([this] { refresh_theme(); });
}


void Button::refresh_theme()
{
	fgcolor(theme::btn_fg);
	bgcolor(theme::btn_bg);
	scheme().activated = theme::nimbus;
	if(theme::is_dark())
		set_bground(btn_bg {});
	else set_bground("");
}


void Button::image(const void *data, unsigned size)
{
	img.open(data, size);
	if(enabled()) icon(img);
}


void Button::image_disabled(const void *data, unsigned size)
{
	img_disabled.open(data, size);
	if(!enabled()) icon(img_disabled);
}


void Button::enable(bool state)
{
	enabled(state);
	if(state)
	{
		if(img) icon(img);
	}
	else
	{
		if(img_disabled) icon(img_disabled);
	}
}


void Progress::create(nana::window parent)
{
	nana::progress_ex::create(parent, true);
	refresh_theme();
	typeface(nana::paint::font_info {"Segoe UI", 11, {800}});
	nana::paint::image img;
	img.open(arr_progbar_jpg, sizeof arr_progbar_jpg);
	image(img);
	events().expose([this] { refresh_theme(); });
}


void Progress::refresh_theme()
{
	if(theme::is_dark())
	{
		dark_bg(true);
		outline_color(nana::color {"#2a3b5c"});
		text_contrast_colors(theme::Label_fg, theme::Label_fg);
		scheme().background = theme::fmbg.blend(nana::colors::black, .1);
		scheme().lower_background = theme::fmbg.blend(nana::colors::white, .1);
	}
	else
	{
		dark_bg(false);
		outline_color(nana::color {"#678"});
		text_contrast_colors(nana::colors::white, nana::color {"#678"});
		scheme().background = nana::color {nana::colors::white}.blend(nana::colors::light_grey, .3 - theme::contrast());
		scheme().lower_background = nana::color {"#f5f5f5"}.blend(nana::colors::light_grey, .3 - theme::contrast());
	}
}