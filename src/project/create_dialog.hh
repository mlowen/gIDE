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

#ifndef CREATE_DIALOG_HH
#define CREATE_DIALOG_HH

#include <gtkmm.h>
#include <libglademm.h>

#include "project.hh"
#include "template_manager.hh"

class CreateDialog : public Gtk::Dialog {
	public:
		CreateDialog(BaseObjectType *, Glib::RefPtr<Gnome::Glade::Xml>);
		
		void set_default_directory(Glib::ustring);
		void set_template_manager(TemplateManager *);
		
		void setup();
		
		Glib::ustring get_project_name();
		Glib::ustring get_project_directory();
		
		Glib::ustring get_template_language() { return m_template_language; }
		Glib::ustring get_template_name() { return m_template_name; }
		
	protected:	
		virtual void on_ok();
		virtual void on_cancel();
		virtual void on_browse();
		virtual void on_check_button();
		virtual void on_language_select(GdkEventButton *); 
		virtual void on_template_select();
		
	private:	
		Glib::RefPtr<Gnome::Glade::Xml> m_xml;
		Glib::ustring m_default_dir;
		Glib::ustring m_template_language;
		Glib::ustring m_template_name;
		
		TemplateManager *m_template_manager;
};

#endif // CREATE_DIALOG_HH
