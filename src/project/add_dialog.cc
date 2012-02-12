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

#include "add_dialog.hh"

AddDialog::AddDialog(BaseObjectType *parent, Glib::RefPtr<Gnome::Glade::Xml> xml) :
 Gtk::Dialog(parent)
{
	m_xml = xml;

	xml->connect_clicked("add_button_add", sigc::mem_fun(*this, &AddDialog::on_ok));
	xml->connect_clicked("add_button_cancel", sigc::mem_fun(*this, &AddDialog::on_cancel));
	
	Gtk::RadioButtonGroup rbg;
	
	((Gtk::RadioButton *) xml->get_widget("add_file"))->set_group(rbg);
	((Gtk::RadioButton *) xml->get_widget("add_directory"))->set_group(rbg);
}

void AddDialog::reset()
{
	((Gtk::Entry *) m_xml->get_widget("add_name"))->set_text("");
	((Gtk::RadioButton *) m_xml->get_widget("add_file"))->set_active();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                    Getters                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool AddDialog::get_is_directory()
{
	return ((Gtk::RadioButton *) m_xml->get_widget("add_directory"))->get_active();
}

Glib::ustring AddDialog::get_file_name()
{
	return ((Gtk::Entry *) m_xml->get_widget("add_name"))->get_text();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Signal Handlers                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void AddDialog::on_ok() 
{
	hide();
	response(Gtk::RESPONSE_OK);
}

void AddDialog::on_cancel()
{
	hide();
	response(Gtk::RESPONSE_CANCEL);
}
