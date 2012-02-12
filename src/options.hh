/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.              *
 ***************************************************************************/

#ifndef OPTIONS_HH
#define OPTIONS_HH

#include <gtkmm.h>

struct ConsoleOptions {
	Glib::ustring font;
	Gdk::Color foreground;
	Gdk::Color background;
};

struct DocumentOptions {
	bool spaces_for_tabs, show_line_numbers, auto_indent;
	bool show_margin, highlight_current_line;
	int tab_width, margin_position, max_undo;
	Glib::ustring font;
};

struct WindowOptions {
	int width, height;
	int v_separator, h_separator;
	bool hide_console, hide_project;
};

struct ProjectOptions {
	Glib::ustring default_directory;
	bool delete_files;
};

#endif // OPTIONS_HH
