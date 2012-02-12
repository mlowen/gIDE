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

#include <cstdlib>
#include <string>
#include <vector>
#include <giomm.h>

#include "gide.hh"
#include "preferences_dialog.hh"
#include "project/add_dialog.hh"
#include "project/create_dialog.hh"
#include "project/properties_dialog.hh"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                       Construtor and Destructor                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

gIDE::gIDE(BaseObjectType *parent, Glib::RefPtr<Gnome::Glade::Xml> xml) : Gtk::Window(parent)
{
	m_open_project = false;

	m_project_visible = m_console_visible = true;

	m_main_ui = xml;
	
	// Load options
	m_config.load();
	
	m_build_output.set_options(m_config.get_build_output_options());
	
	// Set the window size/position options
	set_default_size(m_config.get_window_options().width, m_config.get_window_options().height);
	
	((Gtk::Paned *) xml->get_widget("h_separator"))->set_position(m_config.get_window_options().h_separator);
	((Gtk::Paned *) xml->get_widget("v_separator"))->set_position(m_config.get_window_options().v_separator);
	
	// Lets add the terminal
	m_build_output.show();
	
	Gtk::Notebook *notebook = (Gtk::Notebook *) xml->get_widget("console_notebook");
	notebook->append_page(m_build_output, "Build Output");
	
	// Lets make it visible/invisible
	if(m_config.get_window_options().hide_console)
		on_hide_console();
	
	if(m_config.get_window_options().hide_project)
		on_hide_project();
	
	// Attach the on close handler
	((Gtk::Window *) xml->get_widget("gide_main"))->signal_delete_event().connect(sigc::mem_fun(*this, &gIDE::on_window_close));
	
	// Project View stuff
	Gtk::TreeView *project_view = (Gtk::TreeView *) xml->get_widget("project_view");
	
	m_project.set_delete_files(m_config.get_project_options().delete_files);
	m_project.attach_all_to_view(project_view);
	project_view->signal_row_activated().connect(sigc::mem_fun(*this, &gIDE::on_doc_open));
	project_view->signal_button_press_event().connect_notify(sigc::mem_fun(*this, &gIDE::on_project_click));
	
	// Attach the toolbar and menu item signal handlers
	xml->connect_clicked("toolbar_new", sigc::mem_fun(*this, &gIDE::on_new));
	xml->connect_clicked("menu_file_new", sigc::mem_fun(*this, &gIDE::on_new));	
	
	xml->connect_clicked("toolbar_open", sigc::mem_fun(*this, &gIDE::on_open));
	xml->connect_clicked("menu_file_open", sigc::mem_fun(*this, &gIDE::on_open));	
	
	xml->connect_clicked("toolbar_save", sigc::mem_fun(*this, &gIDE::on_save));
	xml->connect_clicked("menu_file_save", sigc::mem_fun(*this, &gIDE::on_save));
	
	xml->connect_clicked("toolbar_add", sigc::mem_fun(*this, &gIDE::on_add));
	xml->connect_clicked("menu_project_add", sigc::mem_fun(*this, &gIDE::on_add));
	xml->connect_clicked("toolbar_remove", sigc::mem_fun(*this, &gIDE::on_remove));
	xml->connect_clicked("menu_project_remove", sigc::mem_fun(*this, &gIDE::on_remove));
	xml->connect_clicked("toolbar_build", sigc::mem_fun(*this, &gIDE::on_build));
	xml->connect_clicked("menu_project_build", sigc::mem_fun(*this, &gIDE::on_build));
	
	xml->connect_clicked("menu_project_properties", sigc::mem_fun(*this, &gIDE::on_project_properties));
	xml->connect_clicked("toolbar_properties", sigc::mem_fun(*this, &gIDE::on_project_properties));
	
	xml->connect_clicked("menu_edit_copy", sigc::mem_fun(*this, &gIDE::on_copy));
	xml->connect_clicked("menu_edit_cut", sigc::mem_fun(*this, &gIDE::on_cut));
	xml->connect_clicked("menu_edit_paste", sigc::mem_fun(*this, &gIDE::on_paste));
	xml->connect_clicked("menu_edit_undo", sigc::mem_fun(*this, &gIDE::on_undo));
	xml->connect_clicked("menu_edit_redo", sigc::mem_fun(*this, &gIDE::on_redo));
	xml->connect_clicked("menu_edit_preferences", sigc::mem_fun(*this, &gIDE::on_preferences));
	
	xml->connect_clicked("menu_search_find", sigc::mem_fun(*this, &gIDE::on_find));
	xml->connect_clicked("menu_search_replace", sigc::mem_fun(*this, &gIDE::on_replace));
	
	xml->connect_clicked("menu_help_about", sigc::mem_fun(*this, &gIDE::on_about));
	
	// Attach the editor visible signal handlers
	xml->connect_clicked("hide_project", sigc::mem_fun(*this, &gIDE::on_hide_project));	
	xml->connect_clicked("hide_console", sigc::mem_fun(*this, &gIDE::on_hide_console));
	
	// Attach the terminal signal handlers
	xml->connect_clicked("add_terminal", sigc::mem_fun(*this, &gIDE::on_add_terminal));
	xml->connect_clicked("remove_terminal", sigc::mem_fun(*this, &gIDE::on_close_terminal));
	
	// Fill the Project PopUp Menu
	m_project_popup.items().push_back(Gtk::Menu_Helpers::MenuElem("Add", sigc::mem_fun(*this, &gIDE::on_add)));
	m_project_popup.items().push_back(Gtk::Menu_Helpers::MenuElem("Remove", sigc::mem_fun(*this, &gIDE::on_remove)));
	m_project_popup.items().push_back(Gtk::Menu_Helpers::MenuElem("Rename", sigc::mem_fun(*this, &gIDE::on_rename)));
}

gIDE::~gIDE() 
{
	// Clean up
	delete m_misc_ui->get_widget("preferences_dialog");
	delete m_project_ui->get_widget("create_dialog");
	delete m_project_ui->get_widget("add_dialog");
}

/******************************************************************************\
 *                                                                            *
 *                            Public Functions                                *
 *                                                                            *
\******************************************************************************/

void gIDE::setup(Glib::ustring prefix) 
{
	m_prefix = prefix;
	
	// Load the UI files
	m_project_ui = Gnome::Glade::Xml::create(m_prefix + "/share/gIDE/ui/project.glade");
	m_misc_ui = Gnome::Glade::Xml::create(m_prefix + "/share/gIDE/ui/misc.glade");
	
	// Load the templates
	m_template_manager.setup(m_prefix);
	m_template_manager.load("file://" + m_prefix + "/share/gIDE/templates");
}

/******************************************************************************\
 *                                                                            *
 *                            Private Functions                               *
 *                                                                            *
\******************************************************************************/

void gIDE::add_document(Document *doc)
{
	doc->signal_close_document().connect(sigc::mem_fun(*this, &gIDE::on_doc_close));
	
	m_documents.push_back(doc);
	
	doc->show();
	Gtk::Notebook *notebook = ((Gtk::Notebook *) m_main_ui->get_widget("file_notebook"));
	
	int pos = notebook->append_page(*doc, *doc->get_label());
	notebook->set_tab_reorderable(*doc);
	
	notebook->set_current_page(pos);
}

bool gIDE::close_documents() 
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	while(m_documents.size() > 0) {
		Document *doc = *m_documents.begin();
		
		if(!doc->close(this)) {
			return false;
		}
		
		notebook->remove_page(notebook->page_num(*doc));
		m_documents.remove(doc);
		
		delete doc;
	}
	
	return true;
}

/******************************************************************************\
 *                                                                            *
 *                               Signal Handlers                              *
 *                                                                            *
\******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             File Menu/Toolbar                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_open()
{
	Gtk::FileChooserDialog fcd("Open File.", Gtk::FILE_CHOOSER_ACTION_OPEN);
	fcd.set_transient_for(*this);

	fcd.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	fcd.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
	fcd.set_local_only(false);
	
	int result = fcd.run();
	
	Gtk::TreeView *view;
	m_main_ui->get_widget("project_view", view);
	
	if(result != Gtk::RESPONSE_OK) 
		return;

	if(m_open_project) {
		if(!close_documents())
			return;
		
		set_title("gIDE");
		m_project.close();
		m_open_project = false;
	}
	
	if(m_project.load(fcd.get_uri())) {
		set_title("gIDE - " + m_project.get_name());
		m_open_project = true;
	} else {
		Gtk::MessageDialog dialog(*this, m_project.get_error());
		dialog.run();
		
		m_open_project = false;
	}
}

void gIDE::on_new()
{
	CreateDialog *dialog;
	
	m_project_ui->get_widget_derived("create_dialog", dialog);

	dialog->set_default_directory(m_config.get_project_options().default_directory);
	dialog->set_template_manager(&m_template_manager);

	dialog->setup();

	int response = dialog->run();

	if(response == Gtk::RESPONSE_OK) {
		if(m_open_project) {
			if(!close_documents())
				return;
		
			set_title("gIDE");
			m_project.close();
			m_open_project = false;
		}
	
		Glib::ustring name = dialog->get_project_name();
		Glib::ustring directory = dialog->get_project_directory();
		Glib::ustring template_language = dialog->get_template_language();
		Glib::ustring template_name = dialog->get_template_name();
		
		// Get the Template
		Template *t = m_template_manager.get_template(template_language, template_name);
		
		Gtk::MessageDialog err_dialog(*this, "", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_NONE);
		err_dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
				
		if(t == NULL) {
			err_dialog.set_message("Unable to find project template.");
			err_dialog.set_secondary_text("Cannot find template for " + template_language + " - " + template_name + ".");
			err_dialog.run();
		
			return;
		}
				
		if(!t->create(directory, name)) {
			err_dialog.set_message("Unable to create project from template.");
			err_dialog.set_secondary_text(t->get_error());
			err_dialog.run();
			
			return;
		}
				
		if(!m_project.load(t->get_project_uri(directory, name))) {
			m_project.close();
			
			err_dialog.set_message("Unable to open project.");
			err_dialog.set_secondary_text(m_project.get_error());
			err_dialog.run();
			
			return;
		}
		
		set_title("gIDE - " + m_project.get_name());
		m_open_project = true;
	}
}

void gIDE::on_save()
{
	Gtk::Notebook *notebook = ((Gtk::Notebook *) m_main_ui->get_widget("file_notebook"));

	if(notebook->get_n_pages() > 0) {
		Document *current = (Document *)notebook->get_nth_page(notebook->get_current_page());
		
		if(current != NULL)
			current->save();
	}
}

void gIDE::on_save_all()
{
	Document *doc;
	for(std::list<Document *>::iterator i = m_documents.begin(); i != m_documents.end(); i++) {
		doc = *i;
		doc->save();
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           Project Menu/Toolbar                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_add()
{
	if(!m_open_project)
		return;

	AddDialog *dialog;

	m_project_ui->get_widget_derived("add_dialog", dialog);
	
	dialog->reset();
	int response = dialog->run();
	
	if(response == Gtk::RESPONSE_OK) {
		Gtk::TreeView *view = (Gtk::TreeView *) m_main_ui->get_widget("project_view");
		Gtk::TreeIter iter = view->get_selection()->get_selected();
		
		if(!iter) {
			m_project.add(dialog->get_file_name(), dialog->get_is_directory(), this);
		} else if(iter->get_value(m_project.get_columns()->IsDirectory)) {
			m_project.add(dialog->get_file_name(), dialog->get_is_directory(), iter, this);
		} else {
			iter = iter->parent();
			
			if(!iter)
				m_project.add(dialog->get_file_name(), dialog->get_is_directory(), this);
			else
				m_project.add(dialog->get_file_name(), dialog->get_is_directory(), iter, this);
		}
				
		m_project.save();
	}
}

void gIDE::on_remove()
{
	if(!m_open_project)
		return;
	
	Gtk::TreeView *view = (Gtk::TreeView *) m_main_ui->get_widget("project_view");
	Gtk::TreeIter iter = view->get_selection()->get_selected();

	if(!iter)
		return;
	
	Glib::ustring name = iter->get_value(m_project.get_columns()->Name);
	
	Gtk::MessageDialog dialog(*this, "Are you sure you want to remove " + name + " from the project.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_NONE);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

	int response = dialog.run();
	
	if(response == Gtk::RESPONSE_OK) {
		m_project.remove(iter);
		m_project.save();
	}
}

void gIDE::on_rename()
{
	
}

void gIDE::on_build()
{
	if(!m_open_project || !m_project.is_local())
		return;

	if(!m_console_visible)
		on_hide_console();

	Gtk::Notebook *console_notebook = (Gtk::Notebook *) m_main_ui->get_widget("console_notebook");
	console_notebook->set_current_page(0);

	m_build_output.run(m_project.get_build_instructions(), m_project.get_project_directory());
}

void gIDE::on_project_properties()
{
	if(!m_open_project)
		return;
	
	PropertiesDialog *dialog;
	
	m_project_ui->get_widget_derived("project_properties", dialog);
	
	dialog->setup(&m_project);
	
	dialog->run();
	
	m_project.save();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                Edit Menu                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_copy()
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	if(notebook->get_n_pages() > 0) {
		Document *current = (Document *)notebook->get_nth_page(notebook->get_current_page());
		
		if(current != NULL)
			current->copy();
	}
}

void gIDE::on_cut()
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	if(notebook->get_n_pages() > 0) {
		Document *current = (Document *)notebook->get_nth_page(notebook->get_current_page());
		
		if(current != NULL)
			current->cut();
	}
}

void gIDE::on_paste()
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	if(notebook->get_n_pages() > 0) {
		Document *current = (Document *) notebook->get_nth_page(notebook->get_current_page());
		
		if(current != NULL)
			current->paste();
	}
}

void gIDE::on_undo()
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	if(notebook->get_n_pages() > 0) {
		Document *current = (Document *) notebook->get_nth_page(notebook->get_current_page());
		
		if(current != NULL)
			current->undo();
	}
}

void gIDE::on_redo()
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	if(notebook->get_n_pages() > 0) {
		Document *current = (Document *) notebook->get_nth_page(notebook->get_current_page());
		
		if(current != NULL)
			current->redo();
	}
}

void gIDE::on_preferences()
{
	PreferencesDialog *dialog;
	
	m_misc_ui->get_widget_derived("preferences_dialog", dialog);
	
	dialog->setup(m_config.get_document_options(), m_config.get_terminal_options(), m_config.get_build_output_options(), m_config.get_project_options());
	
	dialog->run();
	
	m_config.set_document_options(dialog->get_document_options());
	m_config.set_terminal_options(dialog->get_terminal_options());
	m_config.set_build_output_options(dialog->get_build_output_options());
	m_config.set_project_options(dialog->get_project_options());
	
	m_project.set_delete_files(m_config.get_project_options().delete_files);
	
	// Set the document options
	Document *doc;
	for(std::list<Document *>::iterator i = m_documents.begin(); i != m_documents.end(); i++) {
		doc = *i;
		doc->set_options(m_config.get_document_options());
	}
	
	m_build_output.set_options(m_config.get_build_output_options());
	
	// Set the terminal options
	Terminal *terminal;
	for(std::list<Terminal *>::iterator j = m_terminals.begin(); j != m_terminals.end(); j++) {
		terminal = *j;
		terminal->set_options(m_config.get_terminal_options());
	}
	
	m_config.save();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Search Menu                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_find()
{

}

void gIDE::on_replace()
{

}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                Help Menu                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_about()
{
	Gtk::AboutDialog dialog;
	
	dialog.set_name(PACKAGE_NAME);
	dialog.set_version(PACKAGE_VERSION);
	dialog.set_website("http://www.mlowen.com/gide");
	dialog.set_copyright("Copyright © 2008, 2009 Michael Lowen");
	
	char *raw;
	gsize bytes;
	std::string e_tag;
	
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(m_prefix + "/share/doc/gIDE/AUTHORS");	
	file->load_contents(raw, bytes, e_tag);
	
	std::vector<Glib::ustring> authors;
	
	authors.push_back(raw);
	
	dialog.set_authors(authors);
	
	file = Gio::File::create_for_path(m_prefix + "/share/doc/gIDE/COPYING");
	file->load_contents(raw, bytes, e_tag);
	
	dialog.set_license(raw);	
	dialog.run();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 Documents                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_doc_close(Document *doc)
{
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	if(doc->close(this)) {
		notebook->remove_page(notebook->page_num(*doc));
		m_documents.remove(doc);
	
		delete doc;
	}
}

void gIDE::on_doc_open(const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column)
{
	Gtk::TreeView *view = (Gtk::TreeView *) m_main_ui->get_widget("project_view");

	Gtk::TreeModel::Row row = *view->get_selection()->get_selected();

	if(row[m_project.get_columns()->IsDirectory])
		return;

	Glib::ustring uri = m_project.get_uri(row);
	Gtk::Notebook *notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");

	Document *doc;
	for(std::list<Document *>::iterator i = m_documents.begin(); i != m_documents.end(); i++) {
		doc = *i;
		
		if(doc->get_uri() == uri) {
			notebook->set_current_page(notebook->page_num(*doc));
			return;
		}
	}

	doc = get_document(uri, m_config.get_document_options(), m_misc_ui);
	
	if(doc != NULL)
		add_document(doc);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                       Signal Handlers - Visibility                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_hide_project()
{
	Gtk::HBox *top_hbox = (Gtk::HBox *) m_main_ui->get_widget("top_hbox");
	Gtk::VBox *project_box = (Gtk::VBox *) m_main_ui->get_widget("project_box");
	Gtk::HPaned *h_separator = (Gtk::HPaned *) m_main_ui->get_widget("h_separator");
	Gtk::Label *project_label = (Gtk::Label *) m_main_ui->get_widget("project_label");
	Gtk::Notebook *file_notebook = (Gtk::Notebook *) m_main_ui->get_widget("file_notebook");
	Gtk::ScrolledWindow *project_window = (Gtk::ScrolledWindow * ) m_main_ui->get_widget("project_window");
	Gtk::Image *hide_project_image = (Gtk::Image *) m_main_ui->get_widget("hide_project_image");

	if(m_project_visible) {
		// Removal step
		h_separator->remove(*file_notebook);
		h_separator->remove(*project_box);	
		top_hbox->remove(*h_separator);
		
		project_label->hide();
		project_window->hide();
		
		hide_project_image->set(Gtk::Stock::GO_FORWARD, Gtk::ICON_SIZE_SMALL_TOOLBAR);
		
		top_hbox->pack_start(*project_box, false, false);
		top_hbox->pack_end(*file_notebook);
	} else {
		top_hbox->remove(*project_box);
		top_hbox->remove(*file_notebook);
		
		hide_project_image->set(Gtk::Stock::GO_BACK, Gtk::ICON_SIZE_SMALL_TOOLBAR);
		
		project_label->show();
		project_window->show();
		
		top_hbox->pack_start(*h_separator);
		h_separator->add1(*project_box);
		h_separator->add2(*file_notebook);
	}
	
	m_project_visible = !m_project_visible;
}

void gIDE::on_hide_console()
{
	Gtk::VBox *content_box = (Gtk::VBox *) m_main_ui->get_widget("content_box");
	Gtk::VBox *top_vbox = (Gtk::VBox *) m_main_ui->get_widget("top_vbox");
	Gtk::HBox *bottom_hbox = (Gtk::HBox *) m_main_ui->get_widget("bottom_hbox");
	Gtk::Notebook *console_notebook = (Gtk::Notebook *) m_main_ui->get_widget("console_notebook");
	Gtk::VPaned *v_separator = (Gtk::VPaned *) m_main_ui->get_widget("v_separator");
	Gtk::Image *hide_console_image = (Gtk::Image *) m_main_ui->get_widget("hide_console_image");
	Gtk::Button *add_terminal = (Gtk::Button *) m_main_ui->get_widget("add_terminal");
	Gtk::Button *remove_terminal = (Gtk::Button *) m_main_ui->get_widget("remove_terminal");

	if(m_console_visible) {
		v_separator->remove(*top_vbox);
		v_separator->remove(*bottom_hbox);
		content_box->remove(*v_separator);
		
		console_notebook->hide();
		add_terminal->hide();
		remove_terminal->hide();
		hide_console_image->set(Gtk::Stock::GO_UP, Gtk::ICON_SIZE_SMALL_TOOLBAR);
		
		content_box->pack_end(*bottom_hbox, false, false);
		content_box->pack_end(*top_vbox);
	} else {
		content_box->remove(*top_vbox);
		content_box->remove(*bottom_hbox);
		
		hide_console_image->set(Gtk::Stock::GO_DOWN, Gtk::ICON_SIZE_SMALL_TOOLBAR);
		add_terminal->show();
		remove_terminal->show();
		console_notebook->show();
		
		content_box->pack_end(*v_separator);
		v_separator->add1(*top_vbox);
		v_separator->add2(*bottom_hbox);
	}
	
	m_console_visible = !m_console_visible;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        Signal Handlers - Terminals                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void gIDE::on_add_terminal() 
{
	Gtk::Notebook *console_notebook = (Gtk::Notebook *) m_main_ui->get_widget("console_notebook");
	
	Terminal *t = new Terminal();
	
	t->set_options(m_config.get_terminal_options());
	t->signal_terminal_finished().connect(sigc::mem_fun(*this, &gIDE::on_remove_terminal));
	t->show();
	
	m_terminals.push_back(t);
	
	int pos = console_notebook->append_page(*t, "Terminal");
	console_notebook->set_current_page(pos);
}

void gIDE::on_close_terminal()
{
	Gtk::Notebook *console_notebook = (Gtk::Notebook *) m_main_ui->get_widget("console_notebook");
	
	int pos = console_notebook->get_current_page();
	
	if(pos == 0)
		return;
	
	on_remove_terminal((Terminal *) console_notebook->get_nth_page(pos));
}

void gIDE::on_remove_terminal(Terminal *t)
{
	Gtk::Notebook *console_notebook = (Gtk::Notebook *) m_main_ui->get_widget("console_notebook");

	console_notebook->remove_page(*t);
	m_terminals.remove(t);
	
	delete t;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   Other                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool gIDE::on_window_close(GdkEventAny *event)
{
	// Save the options
	WindowOptions options;
	
	get_size(options.width, options.height);
	options.v_separator = ((Gtk::Paned *) m_main_ui->get_widget("v_separator"))->get_position();
	options.h_separator = ((Gtk::Paned *) m_main_ui->get_widget("h_separator"))->get_position();
	options.hide_console = !m_console_visible;
	options.hide_project = !m_project_visible;

	m_config.set_window_options(options);

	m_config.save();
	
	// Close the terminals
	Terminal *t;
	Gtk::Notebook *console_notebook = (Gtk::Notebook *) m_main_ui->get_widget("console_notebook");
	
	while(!m_terminals.empty()) {
		t = *m_terminals.begin();
		
		console_notebook->remove_page(*t);
		m_terminals.remove(t);
	}
	
	return !close_documents();
}

void gIDE::on_project_click(GdkEventButton *event)
{
	if(!m_open_project)
		return;

	if(event->type == GDK_BUTTON_PRESS && event->button == 3)
		m_project_popup.popup(event->button, event->time);
}

