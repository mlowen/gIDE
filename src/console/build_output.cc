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

#include <iostream>
#include <sstream>
#include <cstring>

#include "build_output.hh"

BuildOutput::BuildOutput() : 
 m_terminal(), m_scrollbars(), m_status_box(), m_status_label(""), m_build_progress()
{
	// Set the terminals behaviour
	m_terminal.set_scroll_on_output(true);
	m_terminal.set_scroll_on_keystroke(true);
	m_terminal.set_mouse_autohide(true);
	m_terminal.signal_child_exited().connect(sigc::mem_fun(*this, &BuildOutput::run_command));
	
	// Set the scrollbars behaviour
	m_scrollbars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scrollbars.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
	m_scrollbars.set_vadjustment(m_terminal.get_adjustment());
	m_scrollbars.add(m_terminal);
	
	// Set up the status box
	m_status_box.pack_start(m_status_label, false, false);
	m_status_box.pack_end(m_build_progress, false, false);
	
	pack_start(m_scrollbars, true, true, 3);
	pack_end(m_status_box, false, true, 3);
	
	m_total_instruction_count = 0;
	
	show_all();
}

void BuildOutput::run(std::vector<Glib::ustring> instructions, Glib::ustring directory) 
{
	m_directory = directory;
	m_instructions = instructions;
	
	run_command();
}

void BuildOutput::set_options(ConsoleOptions options)
{
	m_terminal.set_font_from_string(options.font);
	m_terminal.set_color_foreground(options.foreground);
	m_terminal.set_color_background(options.background);
	m_terminal.set_color_cursor(options.background);
	m_terminal.set_cursor_blinks(false);
}

void BuildOutput::run_command()
{
	int return_value;

	if(m_instructions.empty()) {
		if(m_total_instruction_count > 0) {
			return_value = m_terminal.get_child_exit_status();
			
			if(return_value == 0) {
				m_status_label.set_text("Build Successful");
				m_build_progress.set_fraction(1.0);
			} else {
				m_status_label.set_text("Build Failed");
				m_build_progress.set_fraction(0.0);
			}
		}
		
		m_total_instruction_count = 0;
		return;
	}
	
	int size = m_instructions.size();
	
	if(m_total_instruction_count > 0) {
		return_value = m_terminal.get_child_exit_status();
		
		if(return_value != 0) {
			m_instructions.clear();
			m_status_label.set_text("Build Failed");
			m_build_progress.set_fraction(0.0);
			m_total_instruction_count = 0;
		
			return;
		}
	} else {
		m_total_instruction_count = size;
		m_status_label.set_text("Building");
	}
	
	Glib::ustring instruction = m_instructions[0];
	m_instructions.erase(m_instructions.begin());
	
	std::vector<Glib::ustring> parameters = split(instruction);
	
	std::vector<Glib::ustring> env;
	env.push_back("");
	
	m_build_progress.set_fraction((double)(m_total_instruction_count - size)/(double) m_total_instruction_count);
	
	m_terminal.feed("Running: " + instruction + "\r\n");
	m_terminal.fork_command(parameters[0], parameters, env, m_directory, false, false, false);
}

std::vector<Glib::ustring> BuildOutput::split(Glib::ustring command) 
{
	int start = 0, pos = 0;
	std::vector<Glib::ustring> parts;
	
	while(true) {
		pos = command.find(' ', start);
		
		if(pos == -1) {
			parts.push_back(command.substr(start));
		
			return parts;
		}
		
		parts.push_back(command.substr(start, pos - start));
		
		start = pos + 1;
	}
}
