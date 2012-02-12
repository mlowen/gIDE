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

#include "source_document.hh"

SourceDocument::SourceDocument() : Document(), m_cursor_position("Line 1, Col 1", Gtk::ALIGN_RIGHT)
{
	// Add the GtkSourceView to the scrolled window
	m_scroll_bars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll_bars.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
	m_scroll_bars.add(m_text);
	
	// Connect the signal handlers
	m_text.get_source_buffer()->signal_mark_set().connect(sigc::mem_fun(*this, &SourceDocument::marker_set));
	m_text.get_source_buffer()->signal_changed().connect(sigc::mem_fun(*this, &SourceDocument::set_changed));
	
	// Start showing stuff
	m_text.show();
	m_scroll_bars.show();
	m_cursor_position.show();

	pack_start(m_scroll_bars);
	pack_start(m_cursor_position, false, true);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Operations                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void SourceDocument::copy() { m_text.get_source_buffer()->copy_clipboard(Gtk::Clipboard::get()); }

void SourceDocument::cut() { m_text.get_source_buffer()->cut_clipboard(Gtk::Clipboard::get()); }

void SourceDocument::paste() { m_text.get_source_buffer()->paste_clipboard(Gtk::Clipboard::get()); }

void SourceDocument::undo() 
{ 
	if(m_text.get_source_buffer()->can_undo())
		m_text.get_source_buffer()->undo();
}

void SourceDocument::redo() 
{ 
	if(m_text.get_source_buffer()->can_redo())
		m_text.get_source_buffer()->redo();
}

void SourceDocument::find() 
{ 
	((Gtk::Dialog *) get_glade_file()->get_widget("find_dialog"))->show();
}

void SourceDocument::replace() 
{ 

}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 Options                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void SourceDocument::set_options(DocumentOptions options) 
{
	m_text.set_show_line_numbers(options.show_line_numbers);
	m_text.set_tab_width(options.tab_width);
	m_text.set_auto_indent(options.auto_indent);
	m_text.set_insert_spaces_instead_of_tabs(options.spaces_for_tabs);
	m_text.set_right_margin_position(options.margin_position);
	m_text.set_show_right_margin(options.show_margin);
	m_text.set_highlight_current_line(options.highlight_current_line);
	m_text.get_source_buffer()->set_max_undo_levels(options.max_undo);
	
	Pango::FontDescription font(options.font);
	
	m_text.modify_font(font);
}

void SourceDocument::setup()
{
	Glib::RefPtr<Gnome::Glade::Xml> xml = get_glade_file();
	
	xml->connect_clicked("find_button_find", sigc::mem_fun(*this, &SourceDocument::on_find));
	xml->connect_clicked("find_button_close", sigc::mem_fun(*this, &SourceDocument::on_find_close));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   I/O                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool SourceDocument::read()
{
	char *raw;
	gsize read_bytes;
	
	if(!get_file()->load_contents(raw, read_bytes, m_e_tag))
		return false;
	
	Glib::ustring buf = raw;
	
	// Time to add the data to the sourceview
	Glib::RefPtr<gtksourceview::SourceBuffer> buffer = m_text.get_source_buffer();

	buffer->begin_not_undoable_action();
	buffer->set_text(buf);
	buffer->end_not_undoable_action();
	
	// Retrieve the files mime type.
	Glib::ustring mime_type = Gio::content_type_get_mime_type(get_file()->query_info()->get_content_type());
	
	// Set the syntax highlighting
	Glib::RefPtr<gtksourceview::SourceLanguageManager> manager = gtksourceview::SourceLanguageManager::create();
		
	std::vector<Glib::ustring> lang_ids = manager->get_language_ids();
	std::vector<Glib::ustring> mime_types;
	
	// Okay this seems really clunky, I sure as hell hope that this isn't the best way to do it.
	Glib::RefPtr<gtksourceview::SourceLanguage> language;
	for(std::vector<Glib::ustring>::iterator i = lang_ids.begin(); i != lang_ids.end(); i++) {
		language = manager->get_language(*i);
		
		if(language) {
			mime_types = language->get_mime_types();
			
			for(std::vector<Glib::ustring>::iterator j = mime_types.begin(); j != mime_types.end(); j++) {
				if(*j == mime_type) {
					buffer->set_language(language);
					buffer->set_highlight_syntax(true);
					return true;
				}
			}
		}
	}

	return true;
}

bool SourceDocument::write()
{
	get_file()->replace_contents(m_text.get_source_buffer()->get_text(), m_e_tag, m_e_tag);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              Signal Handlers                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void SourceDocument::marker_set(const Gtk::TextIter &i, const Glib::RefPtr<Gtk::TextMark> &m)
{
	Gtk::TextIter iter = m_text.get_source_buffer()->get_insert()->get_iter();
	
	std::stringstream cursor_pos;
	
	cursor_pos.imbue(std::locale(""));
	
	cursor_pos << "Line " << (iter.get_line() + 1) << ", Col " <<+ (iter.get_line_offset()+1);
			
	m_cursor_position.set_label(Glib::locale_to_utf8(cursor_pos.str()));
}

void SourceDocument::on_find() 
{

}

void SourceDocument::on_find_close()
{
	((Gtk::Dialog *) get_glade_file()->get_widget("find_dialog"))->hide();
}
