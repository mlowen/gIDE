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

#include <vector>
#include <cstdlib>

#include "configuration_manager.hh"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              Public Functions                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

ConfigurationManager::ConfigurationManager()
{
	void set_default_document_options();
	void set_default_terminal_options();
	void set_default_build_output_options();
	void set_default_window_options();
	void set_default_project_options();		
}

void ConfigurationManager::load() 
{
	Glib::RefPtr<Gnome::Conf::Client> client = Gnome::Conf::Client::get_default_client();
	
	load_document_options(client);
	load_terminal_options(client);
	load_build_output_options(client);
	load_window_options(client);
	load_project_options(client);
	
	save();
}

void ConfigurationManager::save()
{
	Glib::RefPtr<Gnome::Conf::Client> client = Gnome::Conf::Client::get_default_client();
	
	save_document_options(client);
	save_terminal_options(client);
	save_build_output_options(client);
	save_window_options(client);
	save_project_options(client);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  Save                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void ConfigurationManager::save_document_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	client->set("/apps/gide/documents/DisplayLineNumbers", m_document.show_line_numbers);
	client->set("/apps/gide/documents/HighlightCurrentLine", m_document.highlight_current_line);
	client->set("/apps/gide/documents/DisplayRightMargin", m_document.show_margin);
	client->set("/apps/gide/documents/RightMarginPosition", m_document.margin_position);
	client->set("/apps/gide/documents/Font", m_document.font);
	client->set("/apps/gide/documents/TabWidth", m_document.tab_width);
	client->set("/apps/gide/documents/UseSpacesInsteadOfTabs", m_document.spaces_for_tabs);
	client->set("/apps/gide/documents/AutomaticIndentation", m_document.auto_indent);
	client->set("/apps/gide/documents/MaxUndo", m_document.max_undo);
}

void ConfigurationManager::save_terminal_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	client->set("/apps/gide/terminal/Font", m_terminal.font);

	std::vector<int> colour;

	// Forground Colour
	colour.push_back((int) m_terminal.foreground.get_red());
	colour.push_back((int) m_terminal.foreground.get_green());
	colour.push_back((int) m_terminal.foreground.get_blue());
	
	client->set_int_list("/apps/gide/terminal/Foreground", colour);	

	colour.clear();

	// Background Colour
	colour.push_back((int) m_terminal.background.get_red());
	colour.push_back((int) m_terminal.background.get_green());
	colour.push_back((int) m_terminal.background.get_blue());
	
	client->set_int_list("/apps/gide/terminal/Background", colour);
}

void ConfigurationManager::save_build_output_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	client->set("/apps/gide/build_output/Font", m_build_output.font);

	std::vector<int> colour;

	// Forground Colour
	colour.push_back((int) m_build_output.foreground.get_red());
	colour.push_back((int) m_build_output.foreground.get_green());
	colour.push_back((int) m_build_output.foreground.get_blue());
	
	client->set_int_list("/apps/gide/build_output/Foreground", colour);	

	colour.clear();

	// Background Colour
	colour.push_back((int) m_build_output.background.get_red());
	colour.push_back((int) m_build_output.background.get_green());
	colour.push_back((int) m_build_output.background.get_blue());
	
	client->set_int_list("/apps/gide/build_output/Background", colour);
}

void ConfigurationManager::save_window_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	client->set("/apps/gide/window/height", m_window.height);
	client->set("/apps/gide/window/width", m_window.width);
	client->set("/apps/gide/window/v_separator", m_window.v_separator);
	client->set("/apps/gide/window/h_separator", m_window.h_separator);
	client->set("/apps/gide/window/hide_console", m_window.hide_console);
	client->set("/apps/gide/window/hide_project", m_window.hide_project);
}

void ConfigurationManager::save_project_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	client->set("/apps/gide/project/default_directory", m_project.default_directory);
	client->set("/apps/gide/project/delete_files", m_project.delete_files);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  load                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void ConfigurationManager::load_document_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	Gnome::Conf::Value key;

	key = client->get("/apps/gide/documents/DisplayLineNumbers");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.show_line_numbers = key.get_bool();
	
	key = client->get("/apps/gide/documents/HighlightCurrentLine");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.highlight_current_line = key.get_bool();
	
	key = client->get("/apps/gide/documents/DisplayRightMargin");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.show_margin = key.get_bool();
	
	key = client->get("/apps/gide/documents/RightMarginPosition");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.margin_position = key.get_int();
	
	key = client->get("/apps/gide/documents/Font");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.font = key.get_string();
	
	key = client->get("/apps/gide/documents/TabWidth");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.tab_width = key.get_int();
	
	key = client->get("/apps/gide/documents/UseSpacesInsteadOfTabs");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.spaces_for_tabs = key.get_bool();
	
	key = client->get("/apps/gide/documents/AutomaticIndentation");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.auto_indent = key.get_bool();
	
	key = client->get("/apps/gide/documents/MaxUndo");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_document.max_undo = key.get_int();
}

void ConfigurationManager::load_terminal_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	Gnome::Conf::Value key;
	std::vector<int> colour;

	key = client->get("/apps/gide/terminal/Font");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_terminal.font = key.get_string();
	
	// Forground Colour
	key = client->get("/apps/gide/terminal/Foreground");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID) {
		colour = key.get_int_list();
		m_terminal.foreground.set_rgb((short) colour[0], (short) colour[1], (short) colour[2]);	
		Gdk::Colormap::get_system()->alloc_color(m_terminal.foreground);
	}
	
	colour.clear();
		
	// Background Colour
	key = client->get("/apps/gide/terminal/Background");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID) {
		colour = key.get_int_list();
		m_terminal.background.set_rgb((short) colour[0], (short) colour[1], (short) colour[2]);	
		Gdk::Colormap::get_system()->alloc_color(m_terminal.foreground);
	}
}

void ConfigurationManager::load_build_output_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	Gnome::Conf::Value key;
	std::vector<int> colour;

	key = client->get("/apps/gide/build_output/Font");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_build_output.font = key.get_string();
	
	// Forground Colour
	key = client->get("/apps/gide/build_output/Foreground");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID) {
		colour = key.get_int_list();
		m_build_output.foreground.set_rgb((short) colour[0], (short) colour[1], (short) colour[2]);	
		Gdk::Colormap::get_system()->alloc_color(m_terminal.foreground);
	}
	
	colour.clear();
		
	// Background Colour
	key = client->get("/apps/gide/build_output/Background");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID) {
		colour = key.get_int_list();
		m_build_output.background.set_rgb((short) colour[0], (short) colour[1], (short) colour[2]);	
		Gdk::Colormap::get_system()->alloc_color(m_terminal.foreground);
	}
}

void ConfigurationManager::load_window_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	Gnome::Conf::Value key;
	
	key = client->get("/apps/gide/window/height");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_window.height = key.get_int();

	key = client->get("/apps/gide/window/width");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_window.width = key.get_int();
	
	key = client->get("/apps/gide/window/v_separator");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_window.v_separator = key.get_int();
	
	key = client->get("/apps/gide/window/h_separator");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_window.h_separator = key.get_int();
		
	key = client->get("/apps/gide/window/hide_console");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_window.hide_console = key.get_bool();
		
	key = client->get("/apps/gide/window/hide_project");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_window.hide_project = key.get_bool();
}

void ConfigurationManager::load_project_options(Glib::RefPtr<Gnome::Conf::Client> client)
{
	Gnome::Conf::Value key;
	
	key = client->get("/apps/gide/project/default_directory");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_project.default_directory = key.get_string();
	
	key = client->get("/apps/gide/project/delete_files");
	if(key.get_type() != Gnome::Conf::VALUE_INVALID)
		m_project.delete_files = key.get_bool();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                Defaults                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void ConfigurationManager::set_default_document_options()
{
	m_document.show_line_numbers = false;
	m_document.highlight_current_line = true;
	m_document.show_margin = true;
	m_document.margin_position = 80;
	m_document.font = "Monospace 10";
	m_document.tab_width = 4;
	m_document.spaces_for_tabs = false; 
	m_document.auto_indent = true;
	m_document.max_undo = 50;
}

void ConfigurationManager::set_default_terminal_options()
{
	m_terminal.font = "Monospace 10";
	m_terminal.foreground.set_rgb(255, 255, 255);
	m_terminal.background.set_rgb(0, 0, 0);
	
	Gdk::Colormap::get_system()->alloc_color(m_terminal.foreground);
	Gdk::Colormap::get_system()->alloc_color(m_terminal.background);
}

void ConfigurationManager::set_default_build_output_options()
{
	m_build_output.font = "Monospace 10";
	m_build_output.foreground.set_rgb(0, 0, 0);
	m_build_output.background.set_rgb(255, 255, 255);
	
	Gdk::Colormap::get_system()->alloc_color(m_build_output.foreground);
	Gdk::Colormap::get_system()->alloc_color(m_build_output.background);
}

void ConfigurationManager::set_default_window_options()
{
	m_window.width = 700;
	m_window.height = 500;
	m_window.v_separator = 350;
	m_window.h_separator = 150;
	m_window.hide_console = false;
	m_window.hide_project = false;
}

void ConfigurationManager::set_default_project_options()
{
	m_project.default_directory = Glib::ustring(getenv("HOME")) + "/Projects";
	m_project.delete_files = false;
}

