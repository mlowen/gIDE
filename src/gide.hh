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

#ifndef GIDE_HH
#define GIDE_HH

#include <list>

#include <gtkmm.h>
#include <libglademm.h>

#include "../config.h"
#include "configuration_manager.hh"
#include "console/build_output.hh"
#include "console/terminal.hh"
#include "document/document.hh"
#include "project/project.hh"
#include "project/template_manager.hh"

class gIDE : public Gtk::Window {
	public:
		gIDE(BaseObjectType *, Glib::RefPtr<Gnome::Glade::Xml>);
		virtual ~gIDE();
			
		void setup(Glib::ustring);
	
	protected:
		// Glade Files
		Glib::RefPtr<Gnome::Glade::Xml> m_main_ui;
		Glib::RefPtr<Gnome::Glade::Xml> m_project_ui;
		Glib::RefPtr<Gnome::Glade::Xml> m_misc_ui;
		
		// UI Elements
		std::list<Document *> m_documents;
		std::list<Terminal *> m_terminals;
		BuildOutput m_build_output;
		Project m_project;
		Gtk::Menu m_project_popup;
		
		// Variables
		Glib::ustring m_prefix;
		bool m_open_project;
		ConfigurationManager m_config;
		TemplateManager m_template_manager;
		int m_next_terminal_id;
		
		// Visible variables
		bool m_project_visible;
		bool m_console_visible;
		
		void add_document(Document *);
		bool close_documents();
		
		// Signal Handlers - Toolbar/Menubar
		// File
		virtual void on_open();
		virtual void on_new();
		virtual void on_save();
		virtual void on_save_all();
		
		// Project
		virtual void on_add();
		virtual void on_remove();
		virtual void on_rename();
		virtual void on_build();
		virtual void on_project_properties();
		
		// Edit
		virtual void on_copy();
		virtual void on_cut();
		virtual void on_paste();
		virtual void on_undo();
		virtual void on_redo();
		virtual void on_preferences();
		
		// Search
		virtual void on_find();
		virtual void on_replace();
		
		// Help
		virtual void on_about();

		// Signal Handlers - Document
		virtual void on_doc_close(Document *);
		virtual void on_doc_open(const Gtk::TreeModel::Path &, Gtk::TreeViewColumn *);
		
		// Signal Handlers - Visibility
		virtual void on_hide_project();
		virtual void on_hide_console();
		
		// Signal Handlers - Terminals
		virtual void on_add_terminal();
		virtual void on_close_terminal();
		virtual void on_remove_terminal(Terminal *);
		
		// Signal Handlers - Other
		virtual bool on_window_close(GdkEventAny *);
		virtual void on_project_click(GdkEventButton *);
};

#endif // GIDE_HH
