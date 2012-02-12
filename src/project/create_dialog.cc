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
 
#include "create_dialog.hh"

CreateDialog::CreateDialog(BaseObjectType *parent, Glib::RefPtr<Gnome::Glade::Xml> xml) :
 Gtk::Dialog(parent)
{
	m_xml = xml;
	
	xml->connect_clicked("create_button_ok", sigc::mem_fun(*this, &CreateDialog::on_ok));
	xml->connect_clicked("create_button_cancel", sigc::mem_fun(*this, &CreateDialog::on_cancel));
	xml->connect_clicked("create_button_browse", sigc::mem_fun(*this, &CreateDialog::on_browse));
	
	Gtk::CheckButton *use_default = (Gtk::CheckButton *) xml->get_widget("create_use_default_dir");
	
	use_default->set_active(true);
	use_default->signal_toggled().connect(sigc::mem_fun(*this, &CreateDialog::on_check_button));
	
	m_template_manager = NULL;
	m_template_language = m_template_name = "";
	
	((Gtk::TreeView *) m_xml->get_widget("create_langauges_list"))->signal_button_press_event().connect_notify(sigc::mem_fun(*this, &CreateDialog::on_language_select));
	((Gtk::IconView *) m_xml->get_widget("create_project_templates"))->signal_selection_changed().connect(sigc::mem_fun(*this, &CreateDialog::on_template_select));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 Setters                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void CreateDialog::set_default_directory(Glib::ustring dir) 
{
	m_default_dir = dir;
	
	((Gtk::Entry *) m_xml->get_widget("create_project_dir"))->set_text(dir);
}

void CreateDialog::set_template_manager(TemplateManager *manager)
{
	m_template_manager = manager;
	
	Gtk::TreeView *view = (Gtk::TreeView *) m_xml->get_widget("create_langauges_list");
	
	if(!view->get_model()) {
		view->set_model(m_template_manager->get_langauges());
	
		view->append_column("Langauges", m_template_manager->get_langauge_column()->Langauge);
	}
}

void CreateDialog::setup()
{
	((Gtk::Entry *) m_xml->get_widget("create_project_name"))->set_text("");
	((Gtk::CheckButton *) m_xml->get_widget("create_use_default_dir"))->set_active(true);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  Getters                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Glib::ustring CreateDialog::get_project_name() 
{
	return ((Gtk::Entry *) m_xml->get_widget("create_project_name"))->get_text();
}

Glib::ustring CreateDialog::get_project_directory()
{
	Glib::ustring uri = "file://";

	if(((Gtk::CheckButton *) m_xml->get_widget("create_use_default_dir"))->get_active())
		uri += m_default_dir;
	else 
		uri += ((Gtk::Entry *) m_xml->get_widget("create_project_dir"))->get_text();
	
	return uri;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              Signal Handlers                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void CreateDialog::on_ok() 
{
	// Check if all the details for creating a project are present
	Gtk::MessageDialog dialog(*this, "", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_NONE);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
	
	if(m_template_language == "" || m_template_language == "") {
		dialog.set_message("No project template selected.");
		dialog.run();
		
		return;
	}
	
	Glib::ustring name = get_project_name();
	Glib::ustring directory = get_project_directory();
	
	if(name == "") {
		dialog.set_message("No project name.");
		dialog.run();
	
		return;
	}
	
	if(directory == "") {
		dialog.set_message("No project directory set.");
		dialog.run();
		
		return;
	}
	
	hide();
	response(Gtk::RESPONSE_OK);
}

void CreateDialog::on_cancel()
{
	hide();
	response(Gtk::RESPONSE_CANCEL);
}

void CreateDialog::on_browse()
{
	Gtk::FileChooserDialog dialog("Choose Directory", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	dialog.set_transient_for(*this);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	dialog.set_local_only(false);
	
	if(dialog.run() == Gtk::RESPONSE_OK) {
		Glib::ustring path = Glib::filename_to_utf8(dialog.get_current_folder());
		((Gtk::Entry *) m_xml->get_widget("create_project_dir"))->set_text(path);
	}
}

void CreateDialog::on_check_button() 
{
	bool sensitive = !((Gtk::CheckButton *) m_xml->get_widget("create_use_default_dir"))->get_active();
		
	m_xml->get_widget("create_project_dir")->set_sensitive(sensitive);
	m_xml->get_widget("create_button_browse")->set_sensitive(sensitive);
}

void CreateDialog::on_language_select(GdkEventButton *event)
{
	// Get the langauge
	Gtk::TreeView *view = (Gtk::TreeView *) m_xml->get_widget("create_langauges_list");
	
	Gtk::TreeIter iter = view->get_selection()->get_selected();
	
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		Glib::ustring language = row[m_template_manager->get_langauge_column()->Langauge];
	
		// Add the template list
		Gtk::IconView *templates = (Gtk::IconView *) m_xml->get_widget("create_project_templates");
	
		templates->set_model(m_template_manager->get_templates(language));
		templates->set_pixbuf_column(m_template_manager->get_template_column()->Image);
		templates->set_text_column(m_template_manager->get_template_column()->Name);
		
		m_template_language = language;
		m_template_name = "";
	} else {
		m_template_language = "";
	}
}

void CreateDialog::on_template_select()
{
	Gtk::IconView *view = (Gtk::IconView *) m_xml->get_widget("create_project_templates");
	Glib::RefPtr<Gtk::ListStore> model = m_template_manager->get_templates();
	
	std::list<Gtk::TreePath> selected = view->get_selected_items();
	
	if(!selected.empty()) {
		const Gtk::TreePath &path = *selected.begin();
		
		Gtk::TreeModel::Row row = *model->get_iter(path);
		
		m_template_name = row[m_template_manager->get_template_column()->Name];
	} else {
		m_template_name = "";
	}
}

