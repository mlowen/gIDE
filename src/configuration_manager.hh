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

#ifndef CONFIGURATION_MANAGER_HH
#define CONFIGURATION_MANAGER_HH

#include <gconfmm.h>

#include "options.hh"

class ConfigurationManager {
	public:
		ConfigurationManager();
	
		void load();
		void save();

		// Setters
		void set_document_options(DocumentOptions options) { m_document = options; }
		void set_terminal_options(ConsoleOptions options) { m_terminal = options; }
		void set_build_output_options(ConsoleOptions options) { m_build_output = options; }
		void set_window_options(WindowOptions options) { m_window = options; }
		void set_project_options(ProjectOptions options) { m_project = options; }

		// Getters 
		DocumentOptions get_document_options() { return m_document; }
		ConsoleOptions get_terminal_options() { return m_terminal; }
		ConsoleOptions get_build_output_options() { return m_build_output; }
		WindowOptions get_window_options() { return m_window; }
		ProjectOptions get_project_options() { return m_project; }

	private:
		DocumentOptions m_document;
		ConsoleOptions m_terminal;
		ConsoleOptions m_build_output;
		WindowOptions m_window;
		ProjectOptions m_project;

		void save_document_options(Glib::RefPtr<Gnome::Conf::Client>);
		void save_terminal_options(Glib::RefPtr<Gnome::Conf::Client>);
		void save_build_output_options(Glib::RefPtr<Gnome::Conf::Client>);
		void save_window_options(Glib::RefPtr<Gnome::Conf::Client>);
		void save_project_options(Glib::RefPtr<Gnome::Conf::Client>);

		void load_document_options(Glib::RefPtr<Gnome::Conf::Client>);
		void load_terminal_options(Glib::RefPtr<Gnome::Conf::Client>);
		void load_build_output_options(Glib::RefPtr<Gnome::Conf::Client>);
		void load_window_options(Glib::RefPtr<Gnome::Conf::Client>);
		void load_project_options(Glib::RefPtr<Gnome::Conf::Client>);

		void set_default_document_options();
		void set_default_terminal_options();
		void set_default_build_output_options();
		void set_default_window_options();
		void set_default_project_options();
};
 
#endif // CONFIGURATION_MANAGER_HH

