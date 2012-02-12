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

#ifndef PREFERENCES_DIALOG_HH
#define PREFERENCES_DIALOG_HH

#include <gtkmm.h>
#include <libglademm.h>

#include "options.hh"

class PreferencesDialog : public Gtk::Dialog {
	public:
		PreferencesDialog(BaseObjectType *, Glib::RefPtr<Gnome::Glade::Xml>);	
		
		void setup(DocumentOptions, ConsoleOptions, ConsoleOptions, ProjectOptions);
		
		DocumentOptions get_document_options() { return m_document; }
		ConsoleOptions get_terminal_options() { return m_terminal; }
		ConsoleOptions get_build_output_options() { return m_build_output; }
		ProjectOptions get_project_options() { return m_project; }
		
	protected:
		DocumentOptions m_document;
		ConsoleOptions m_terminal;
		ConsoleOptions m_build_output;
		ProjectOptions m_project;
	
		Glib::RefPtr<Gnome::Glade::Xml> m_xml;
	
		virtual void on_ok();
		virtual void on_browse();
};

#endif // PREFERENCES_DIALOG_HH
