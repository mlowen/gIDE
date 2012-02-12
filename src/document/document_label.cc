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

#include "document_label.hh"

DocumentLabel::DocumentLabel(Glib::ustring text) :
 m_label(),
 m_icon(Gtk::Stock::FILE, Gtk::ICON_SIZE_MENU),
 m_image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU),
 m_button()
{
	set_text(text);
	
	m_button.set_relief(Gtk::RELIEF_NONE);
	m_button.set_size_request(20, 20);
	m_button.set_border_width(0);
	m_button.add(m_image);

	m_button.signal_clicked().connect(sigc::mem_fun(*this, &DocumentLabel::on_button_clicked));

	m_label.set_single_line_mode(true);

	m_label.show();
	m_icon.show();
	m_image.show();
	m_button.show();
	
	pack_start(m_icon, false, false);
	pack_start(m_label, false, false);
	pack_end(m_button, false, false);
}

void DocumentLabel::set_text(Glib::ustring text) 
{
	m_text = text;
	m_label.set_label(m_text);
}

void DocumentLabel::set_changed(bool changed)
{
	if(changed)
		m_label.set_label("*" + m_text);
	else 
		m_label.set_label(m_text);
}

void DocumentLabel::on_button_clicked() 
{
	m_signal_close.emit();
}
