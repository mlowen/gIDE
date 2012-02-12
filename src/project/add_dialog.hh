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

#ifndef ADD_DIALOG_HH
#define ADD_DIALOG_HH

#include <gtkmm.h>
#include <libglademm.h>

class AddDialog : public Gtk::Dialog {
	public:
		AddDialog(BaseObjectType *, Glib::RefPtr<Gnome::Glade::Xml>);
		
		void reset();
		
		bool get_is_directory();
		Glib::ustring get_file_name();
		
	protected:		
		virtual void on_ok();
		virtual void on_cancel();
		
	private:
		Glib::RefPtr<Gnome::Glade::Xml> m_xml;
};

#endif // ADD_TO_PROJECT_DIALOG_HH
