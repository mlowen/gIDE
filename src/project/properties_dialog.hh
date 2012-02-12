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

#ifndef PROPERTIES_DIALOG_HH
#define PROPERTIES_DIALOG_HH

#include <gtkmm.h>
#include <libglademm.h>

#include "project.hh"

class InstructionColumn : public Gtk::TreeModel::ColumnRecord {
	public:
		InstructionColumn() { add(Instruction); }
		
		Gtk::TreeModelColumn<Glib::ustring> Instruction;
};

class PropertiesDialog : public Gtk::Dialog {
	public:
		PropertiesDialog(BaseObjectType *, Glib::RefPtr<Gnome::Glade::Xml>);
		
		void setup(Project *);
		
	protected:
		// Signal Handlers
		virtual void on_ok();
		virtual void on_cancel();
	
		virtual void on_add();
		virtual void on_remove();
		virtual void on_move_up();
		virtual void on_move_down();
	
	private:
		Project *m_project;

		Glib::RefPtr<Gnome::Glade::Xml> m_xml;

		InstructionColumn m_columns;
		Glib::RefPtr<Gtk::ListStore> m_instructions;
};

#endif // PROPERTIES_DIALOG_HH
