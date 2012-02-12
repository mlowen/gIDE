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

#ifndef BUILD_OUTPUT_HH
#define BUILD_OUTPUT_HH

#include <vector>

#include <gtkmm.h>
#include <libvtemm.h>

#include "../options.hh"

class BuildOutput : public Gtk::VBox {
	public:
		BuildOutput();
	
		void run(std::vector<Glib::ustring>, Glib::ustring);
		
		virtual void set_options(ConsoleOptions);
	
	private:	
		void run_command();	
		std::vector<Glib::ustring> split(Glib::ustring);
		
		Glib::ustring m_directory;
		std::vector<Glib::ustring> m_instructions;
		int m_total_instruction_count;
		
		Gnome::Vte::Terminal m_terminal;
		Gtk::ScrolledWindow m_scrollbars;
		Gtk::HBox m_status_box;
		Gtk::Label m_status_label;
		Gtk::ProgressBar m_build_progress;
};

#endif // BUILD_OUTPUT_HH

