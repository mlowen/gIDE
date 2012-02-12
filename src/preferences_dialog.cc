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

#include "preferences_dialog.hh"

PreferencesDialog::PreferencesDialog(BaseObjectType *parent, Glib::RefPtr<Gnome::Glade::Xml> xml) :
 Gtk::Dialog(parent)
{
	m_xml = xml;
	
	xml->connect_clicked("preferences_ok", sigc::mem_fun(*this, &PreferencesDialog::on_ok));
	xml->connect_clicked("project_dir_browse", sigc::mem_fun(*this, &PreferencesDialog::on_browse));
}

void PreferencesDialog::setup(DocumentOptions doc, ConsoleOptions term, ConsoleOptions build, ProjectOptions project)
{
	// Set the Document Options
	((Gtk::CheckButton *) m_xml->get_widget("lineNumbers"))->set_active(doc.show_line_numbers);	
	((Gtk::CheckButton *) m_xml->get_widget("highlightLine"))->set_active(doc.highlight_current_line);	
	((Gtk::CheckButton *) m_xml->get_widget("displayMargin"))->set_active(doc.show_margin);	
	((Gtk::SpinButton *) m_xml->get_widget("marginWidth"))->set_value(doc.margin_position);	
	((Gtk::FontButton *) m_xml->get_widget("editorFont"))->set_font_name(doc.font);	
	((Gtk::SpinButton *) m_xml->get_widget("tabWidth"))->set_value(doc.tab_width);	
	((Gtk::CheckButton *) m_xml->get_widget("spacesForTabs"))->set_active(doc.spaces_for_tabs);	
	((Gtk::CheckButton *) m_xml->get_widget("automaticIndentation"))->set_active(doc.auto_indent);
	((Gtk::SpinButton *) m_xml->get_widget("undoLevel"))->set_value(doc.max_undo);
	
	// Set General Console Options
	((Gtk::FontButton *) m_xml->get_widget("console_font"))->set_font_name(term.font);	
	
	// Set Terminal Options
	((Gtk::ColorButton *) m_xml->get_widget("terminal_colour_foreground"))->set_color(term.foreground);
	((Gtk::ColorButton *) m_xml->get_widget("terminal_colour_background"))->set_color(term.background);
	
	// Set Build Output Options
	((Gtk::ColorButton *) m_xml->get_widget("build_colour_foreground"))->set_color(build.foreground);
	((Gtk::ColorButton *) m_xml->get_widget("build_colour_background"))->set_color(build.background);
	
	// Set the project Options
	((Gtk::Entry *) m_xml->get_widget("default_project_directory"))->set_text(project.default_directory);
	((Gtk::CheckButton *) m_xml->get_widget("project_delete_files"))->set_active(project.delete_files);
}

void PreferencesDialog::on_ok() 
{
	// Get Document Options
	m_document.show_line_numbers = ((Gtk::CheckButton *) m_xml->get_widget("lineNumbers"))->get_active();	
	m_document.highlight_current_line = ((Gtk::CheckButton *) m_xml->get_widget("highlightLine"))->get_active();	
	m_document.show_margin = ((Gtk::CheckButton *) m_xml->get_widget("displayMargin"))->get_active();	
	m_document.margin_position = ((Gtk::SpinButton *) m_xml->get_widget("marginWidth"))->get_value();	
	m_document.font = ((Gtk::FontButton *) m_xml->get_widget("editorFont"))->get_font_name();	
	m_document.tab_width = ((Gtk::SpinButton *) m_xml->get_widget("tabWidth"))->get_value();	
	m_document.spaces_for_tabs = ((Gtk::CheckButton *) m_xml->get_widget("spacesForTabs"))->get_active();	
	m_document.auto_indent = ((Gtk::CheckButton *) m_xml->get_widget("automaticIndentation"))->get_active();
	m_document.max_undo = ((Gtk::SpinButton *) m_xml->get_widget("undoLevel"))->get_value();
	
	// Get General Console Options
	m_terminal.font = m_build_output.font = ((Gtk::FontButton *) m_xml->get_widget("console_font"))->get_font_name();
	
	// Get Terminal Options
	m_terminal.foreground = ((Gtk::ColorButton *) m_xml->get_widget("terminal_colour_foreground"))->get_color();
	m_terminal.background = ((Gtk::ColorButton *) m_xml->get_widget("terminal_colour_background"))->get_color();
	
	// Get Build Output Options
	m_build_output.foreground = ((Gtk::ColorButton *) m_xml->get_widget("build_colour_foreground"))->get_color();
	m_build_output.background = ((Gtk::ColorButton *) m_xml->get_widget("build_colour_background"))->get_color();
	
	// Get the project Options
	m_project.default_directory = ((Gtk::Entry *) m_xml->get_widget("default_project_directory"))->get_text();
	m_project.delete_files = ((Gtk::CheckButton *) m_xml->get_widget("project_delete_files"))->get_active();
	
	hide();
	response(Gtk::RESPONSE_OK);
}

void PreferencesDialog::on_browse()
{
	Gtk::FileChooserDialog dialog("Choose Directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_transient_for(*this);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	
	if(dialog.run() == Gtk::RESPONSE_OK) {
		Glib::ustring path = Glib::filename_to_utf8(dialog.get_current_folder());
		((Gtk::Entry *) m_xml->get_widget("default_project_directory"))->set_text(path);
	}
}
