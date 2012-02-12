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

#include<cstdlib>

#include "terminal.hh"

Terminal::Terminal() : m_terminal()
{
	// Set the terminals behaviour
	m_terminal.set_scroll_on_output(true);
	m_terminal.set_scroll_on_keystroke(true);
	m_terminal.set_mouse_autohide(true);
	
	// Set the scrollbars behaviour
	set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	set_shadow_type(Gtk::SHADOW_ETCHED_IN);
	
	// Setup the scrollbar
	set_vadjustment(m_terminal.get_adjustment());
	
	add(m_terminal);
	
	// Setup the signal to destroy the terminal
	m_terminal.signal_child_exited().connect(sigc::mem_fun(*this, &Terminal::on_child_exited));
	
	m_terminal.show();

	std::vector<Glib::ustring> holder;
	holder.push_back("");

	m_terminal.fork_command(getenv("SHELL"), holder, holder, "", false, false, false);
}

void Terminal::set_options(ConsoleOptions options) 
{
	m_terminal.set_font_from_string(options.font);
	m_terminal.set_color_foreground(options.foreground);
	m_terminal.set_color_background(options.background);
}

void Terminal::on_child_exited()
{
	m_signal_terminal_finished.emit(this);
}

