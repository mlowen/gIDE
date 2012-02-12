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

#include <cstring>
#include <sstream>

#include "document.hh"
#include "source_document.hh"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Document::Document() : m_label("Untitled")
{	
	m_changed = false;	
	
	// Connect the signal handlers
	m_label.signal_close().connect(sigc::mem_fun(*this, &Document::on_label_close));
	
	m_label.show();
	
	set_border_width(5);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  I/O                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool Document::open(Glib::ustring file_name)
{
	m_file = Gio::File::create_for_uri(file_name);
	
	if(!m_file->query_exists())
		return false;
	
	bool res = read();
	
	m_changed = false;
	m_label.set_text(m_file->get_basename());
	m_label.set_changed(false);
	
	return res;
}

bool Document::save()
{
	if(m_changed) {
		m_changed = false;
		m_label.set_changed(false);
		return write();
	}

	return true;
}

bool Document::close(Gtk::Window *parent)
{
	if(m_changed) {
		Gtk::MessageDialog dialog(*parent, "Do you want to save changes?", false, 
		                          Gtk::MESSAGE_WARNING, Gtk::BUTTONS_NONE);
		
		
		dialog.set_secondary_text(m_file->get_basename() + " has changed since it was last saved, do you want to save the changes.");
		dialog.add_button("Close without Saving", Gtk::RESPONSE_NO);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_YES);


		switch(dialog.run()) {
			case Gtk::RESPONSE_YES:
				return save();
			case Gtk::RESPONSE_NO:
				return true;
			case Gtk::RESPONSE_CANCEL:
				return false;
			default: return false;
		}
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Utilities                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Glib::ustring Document::get_name() 
{ 
	if(!m_file)
		return "";
	
	return m_file->get_basename();
}

Glib::ustring Document::get_uri()
{
	if(!m_file)
		return "";
	
	return m_file->get_uri();
}

void Document::set_changed() 
{
	if(m_changed)
		return;
		
	m_changed = true;
	m_label.set_changed(true);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Signal Handlers                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Document::on_label_close()
{
	m_signal_close_document.emit(this);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Functions                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Document *get_document(Glib::ustring uri, DocumentOptions options, Glib::RefPtr<Gnome::Glade::Xml> xml) 
{
	Glib::RefPtr<Gio::File> fp = Gio::File::create_for_uri(uri);
			
	if(!fp->query_exists())
		return NULL;
			
	Document *doc;
		
	/*if(uri.find_last_of('.') != Glib::ustring::npos) {
		if(uri.substr(uri.find_last_of('.')) == ".glade" && fp->is_native()) {
				
		}
	} else { doc = new SourceDocument(); }*/
	
	doc = new SourceDocument(); // Temporary until the glade document is ready.
	
	doc->set_glade_file(xml);
	doc->set_options(options);
	doc->setup();
	doc->open(uri);
	
	return doc;
}
