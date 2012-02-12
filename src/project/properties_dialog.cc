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

#include "properties_dialog.hh"

PropertiesDialog::PropertiesDialog(BaseObjectType *parent, Glib::RefPtr<Gnome::Glade::Xml> xml) :
 Gtk::Dialog(parent)
{
	m_xml = xml;
	
	m_xml->connect_clicked("project_options_ok", sigc::mem_fun(*this, &PropertiesDialog::on_ok));
	m_xml->connect_clicked("project_options_cancel", sigc::mem_fun(*this, &PropertiesDialog::on_cancel));
	
	m_xml->connect_clicked("po_instructions_add", sigc::mem_fun(*this, &PropertiesDialog::on_add));
	m_xml->connect_clicked("po_instructions_remove", sigc::mem_fun(*this, &PropertiesDialog::on_remove));
	m_xml->connect_clicked("po_instructions_up", sigc::mem_fun(*this, &PropertiesDialog::on_move_up));
	m_xml->connect_clicked("po_instructions_down", sigc::mem_fun(*this, &PropertiesDialog::on_move_down));
}

void PropertiesDialog::setup(Project *project)
{
	m_project = project;
	
	((Gtk::Entry *) m_xml->get_widget("po_name"))->set_text(m_project->get_name());
	
	// Set up the build instruction list
	std::vector<Glib::ustring> instructions = m_project->get_build_instructions();
	
	m_instructions = Gtk::ListStore::create(m_columns);
	
	for(int i = 0; i < instructions.size(); i++) {
		Gtk::TreeModel::Row row = *m_instructions->append();
		
		row[m_columns.Instruction] = instructions[i];
	}
	
	Gtk::TreeView *view = (Gtk::TreeView *) m_xml->get_widget("po_build_instructions");
	
	view->remove_all_columns();
	
	view->set_model(m_instructions);
	view->append_column("Build Instructions", m_columns.Instruction);
	
	((Gtk::Entry *) m_xml->get_widget("new_instruction"))->set_text("");
}

void PropertiesDialog::on_ok() 
{
	Glib::ustring name = ((Gtk::Entry *) m_xml->get_widget("po_name"))->get_text();
	
	name = m_project->trim(name);
	
	if(name == "") {
		Gtk::MessageDialog dialog(*this, "You need to specify a name.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_NONE);
		dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
		
		dialog.run();
		return;
	}

	m_project->set_name(name);

	// Write the build instructions
	
	std::vector<Glib::ustring> instructions;
	Gtk::TreeModel::Children children = m_instructions->children();
	
	for(Gtk::TreeIter i = children.begin(); i != children.end(); i++) {
		Gtk::TreeModel::Row row = *i;
		
		instructions.push_back(m_project->trim(row[m_columns.Instruction]));
	}

	m_project->set_build_instructions(instructions);

	hide();
	response(Gtk::RESPONSE_OK);
}

void PropertiesDialog::on_cancel()
{
	hide();
	response(Gtk::RESPONSE_CANCEL);
}

void PropertiesDialog::on_add() 
{
	Gtk::InputDialog dialog;
	
	Gtk::Entry *new_instruction = (Gtk::Entry *) m_xml->get_widget("new_instruction");
	
	Glib::ustring instruction = m_project->trim(new_instruction->get_text());
	
	if(instruction == "") {
		Gtk::MessageDialog dialog(*this, "No instruction specified.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_NONE);
		dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
		
		dialog.run();
		return;
	}
	
	Gtk::TreeModel::Row row = *m_instructions->append();
		
		row[m_columns.Instruction] = instruction;
	
	new_instruction->set_text("");
}

void PropertiesDialog::on_remove()
{
	Gtk::TreeView *view = (Gtk::TreeView *) m_xml->get_widget("po_build_instructions");
	
	Gtk::TreeIter iter = view->get_selection()->get_selected();
	
	if(!iter)
		return;
		
	m_instructions->erase(iter);
}

void PropertiesDialog::on_move_up()
{
	Gtk::TreeView *view = (Gtk::TreeView *) m_xml->get_widget("po_build_instructions");
	
	Gtk::TreeIter iter_a = view->get_selection()->get_selected();
	
	if(!iter_a)
		return;
		
	Gtk::TreeIter iter_b = iter_a--;
	
	if(!iter_b)
		return;
	
	m_instructions->iter_swap(iter_a, iter_b);	
}

void PropertiesDialog::on_move_down()
{
	Gtk::TreeView *view = (Gtk::TreeView *) m_xml->get_widget("po_build_instructions");
	
	Gtk::TreeIter iter_a = view->get_selection()->get_selected();
	
	if(!iter_a)
		return;
		
	Gtk::TreeIter iter_b = iter_a++;
	
	if(!iter_b)
		return;
	
	m_instructions->iter_swap(iter_a, iter_b);
}
