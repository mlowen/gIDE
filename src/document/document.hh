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

#ifndef DOCUMENT_HH
#define DOCUMENT_HH

#include <string>

#include <gtkmm.h>
#include <giomm.h>
#include <libglademm.h>

#include "../options.hh"
#include "document_label.hh"

class Document : public Gtk::VBox {
	public:
		Document();
		
		// File Operations
		bool open(Glib::ustring);
		bool close(Gtk::Window *);
		bool save();
		
		// Document Operations
		virtual void copy() = 0;
		virtual void cut() = 0;
		virtual void paste() = 0;
		virtual void undo() = 0;
		virtual void redo() = 0;
		virtual void find() { };
		virtual void replace() { };
		
		Glib::ustring get_name();
		Glib::ustring get_uri();
		
		DocumentLabel *get_label() { return &m_label; } 
		void set_glade_file(Glib::RefPtr<Gnome::Glade::Xml> xml) { m_xml = xml; }
		
		virtual void setup() { };
		virtual void set_options(DocumentOptions options) { };
		
		// Signal Accessors
		sigc::signal<void, Document *> signal_close_document() { return m_signal_close_document; }
		
	protected:		
		virtual void set_changed();

		Glib::RefPtr<Gio::File> get_file() { return m_file; }
		Glib::RefPtr<Gnome::Glade::Xml> get_glade_file() { return m_xml; }
		
		// I/O Functions
		virtual bool read() = 0;
		virtual bool write() = 0;
		
		// Signal Handlers
		virtual void on_label_close();
	
	private:
		Glib::RefPtr<Gio::File> m_file;
		Glib::RefPtr<Gnome::Glade::Xml> m_xml;
		bool m_changed;
		DocumentLabel m_label;
		
		// Signals
		sigc::signal<void, Document *> m_signal_close_document;
};

Document *get_document(Glib::ustring, DocumentOptions, Glib::RefPtr<Gnome::Glade::Xml>);

#endif // DOCUMENT_HH

