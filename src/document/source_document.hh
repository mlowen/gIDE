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

#ifndef SOURCE_DOCUMENT_HH
#define SOURCE_DOCUMENT_HH

#include <gtksourceviewmm.h>
#include "document.hh"

class SourceDocument : public Document {
	public:
		SourceDocument();
		
		// Operations
		virtual void copy();
		virtual void cut();
		virtual void paste();
		virtual void undo();
		virtual void redo();
		virtual void find();
		virtual void replace();
		
		// Options
		virtual void set_options(DocumentOptions);
		virtual void setup();
		
	protected:		
		// I/O
		virtual bool read();
		virtual bool write();
	
		// Signal Handlers
		virtual void marker_set(const Gtk::TextIter &, const Glib::RefPtr<Gtk::TextMark> &);
		virtual void on_find();
		virtual void on_find_close();
		
	private:
		gtksourceview::SourceView m_text;
		Gtk::ScrolledWindow m_scroll_bars;
		std::string m_e_tag;
		Gtk::Label m_cursor_position;
}; 

#endif // SOURCE_DOCUMENT_HH
