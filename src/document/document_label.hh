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

#include <gtkmm.h>

#ifndef DOCUMENT_LABEL_HH
#define DOCUMENT_LABEL_HH

class DocumentLabel : public Gtk::HBox {
	public:
		DocumentLabel(Glib::ustring);

		void set_text(Glib::ustring);
		Glib::ustring get_text() { return m_text; }
		
		void set_changed(bool);

		sigc::signal<void> signal_close() { return m_signal_close; }
		
	private:
		Glib::ustring m_text;
		
		Gtk::Image m_icon;
		Gtk::Image m_image;
		Gtk::Label m_label;
		Gtk::Button m_button;
		
		// Signal Handlers
		virtual void on_button_clicked();
	
	protected:
		// Signals
		sigc::signal<void> m_signal_close;
};

#endif // DOCUMENT_LABEL_HH

