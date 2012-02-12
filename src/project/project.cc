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
 
#include "project.hh"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              Project Column                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

ProjectColumn::ProjectColumn()
{
	add(Name);
	add(Path);
	add(IsDirectory);
	add(Image);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  Project                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Project::Project() 
{
	m_data = Gtk::TreeStore::create(m_columns);
	
	m_data->set_sort_func(0, sigc::mem_fun(*this, &Project::sort));
	m_data->set_sort_column(0, Gtk::SORT_ASCENDING);
	
	m_delete_files = false;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                    IO                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool Project::load(Glib::ustring uri) 
{	
	m_data->clear();
	m_build_instructions.clear();
	m_file = Gio::File::create_for_uri(uri);
	
	if(!m_file->query_exists())
		return false;
	
	char *raw;
	gsize bytes_read;
	
	m_base_path = m_file->get_parent()->get_uri() + "/";
	
	try {
		m_file->load_contents(raw, bytes_read, m_e_tag);
		Glib::ustring buffer = raw;
	
		xmlpp::DomParser parser;
	
		parser.parse_memory(buffer);
		
		xmlpp::Node *root = parser.get_document()->get_root_node();
		
		if(root->get_name() != "project")
			return false;
			
		xmlpp::Node::NodeList list = root->get_children();
		xmlpp::Element *node;
		for(xmlpp::Node::NodeList::iterator i = list.begin(); i != list.end(); ++i) {
			if(node = dynamic_cast<xmlpp::Element *>(*i)) {
				if(node->get_name() == "name")
					m_name = trim(node->get_child_text()->get_content());
				else if(node->get_name() == "files")
					parse_files(node);
				else if(node->get_name() == "build")
					parse_build_instructions(node);
				
			}
		}
	} catch(const std::exception &ex) {
		m_error = ex.what();
	
		return false;
	}
	
	return true;
}

bool Project::save()
{
	try {
		xmlpp::Document doc;
		
		xmlpp::Element *root = doc.create_root_node("project");
		
		xmlpp::Element *node = root->add_child("name");
		node->set_child_text(m_name);
				
		write_files(root->add_child("files"));
		
		write_build_instructions(root->add_child("build"));
		
		Glib::ustring buffer = doc.write_to_string_formatted();
		
		Glib::RefPtr<Gio::FileInfo> info = m_file->query_info();
		std::string e_tag;
		
		m_file->replace_contents(buffer.c_str(), info->get_etag(), e_tag);
	} catch(const std::exception &ex) {
		m_error = ex.what();
		
		return false;
	}
	
	return true;
}

void Project::close()
{
	m_data->clear();
	m_build_instructions.clear();
	m_name = m_base_path = "";
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           Attaching to view                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Project::attach_all_to_view(Gtk::TreeView *view)
{
	view->set_model(m_data);
	Gtk::CellRendererPixbuf *renderer = Gtk::manage(new Gtk::CellRendererPixbuf);
	Gtk::TreeView::Column *column = Gtk::manage(new Gtk::TreeView::Column(""));
	
	column->pack_start(*renderer, false);
	column->pack_start(m_columns.Name);
	
	if(column)	
		column->add_attribute(renderer->property_stock_id(), m_columns.Image);
	
	view->append_column(*column);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                  Misc                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Glib::ustring Project::get_uri(Gtk::TreeModel::Row &row)
{
	return m_base_path + row[m_columns.Path] + row[m_columns.Name];
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Project Operations                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Project::new_project(Glib::ustring name, Glib::ustring path)
{
	if(path[path.size()-1] != '/')
		path += "/";
	
	path += name + "/";
	
	Glib::RefPtr<Gio::File> project_dir = Gio::File::create_for_uri(path);
	
	if(!project_dir->query_exists())
		project_dir->make_directory();
	
	m_file = Gio::File::create_for_uri(path + name + ".xml");
	
	if(!m_file->query_exists())
		m_file->create_file();

	m_data->clear();
	
	m_name = name;
	m_base_path = path;
	
	save();
}

Glib::ustring Project::add(Glib::ustring name, bool is_directory, Gtk::Window *win)
{
	for(Gtk::TreeIter i = m_data->children().begin(); i != m_data->children().end(); i++) {
		if(i->get_value(m_columns.Name) == name) {
			Gtk::MessageDialog dialog(*win, "Cannot add " + name + " as a file already exists in the project.", true, Gtk::MESSAGE_ERROR);
			dialog.run();
			
			return "";
		}
	}

	Gtk::TreeModel::Row row = *m_data->append();
	
	row[m_columns.Name] = name;
	row[m_columns.IsDirectory] = is_directory;
	
	if(is_directory) 
		row[m_columns.Image] = Gtk::StockID(Gtk::Stock::DIRECTORY).get_string();
	else 
		row[m_columns.Image] = Gtk::StockID(Gtk::Stock::FILE).get_string();
	
	Glib::ustring uri = m_base_path + name;
	
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(uri);
	
	if(file->query_exists())
		return uri;
	
	if(is_directory)
		file->make_directory();
	else
		file->create_file();
	
	// save();
	
	return uri;
}

Glib::ustring Project::add(Glib::ustring name, bool is_directory, Gtk::TreeIter parent, Gtk::Window *win)
{
	if(!parent)
		return add(name, is_directory, win);

	for(Gtk::TreeIter i = parent->children().begin(); i != parent->children().end(); i++) {
		if(i->get_value(m_columns.Name) == name) {
			Gtk::MessageDialog dialog(*win, "Cannot add " + name + " as a file already exists in the project.", true, Gtk::MESSAGE_ERROR);
			dialog.run();
			
			return "";
		}
	}

	Gtk::TreeIter row = m_data->append(parent->children());
	
	Glib::ustring path = parent->get_value(m_columns.Path) + parent->get_value(m_columns.Name) + "/";
	Glib::ustring fullPath = m_base_path + path;
	Glib::ustring uri = fullPath + name;
	
	row->set_value(m_columns.Name, name);
	row->set_value(m_columns.IsDirectory, is_directory);
	row->set_value(m_columns.Path, path);
	
	if(is_directory)
		row->set_value(m_columns.Image, Gtk::StockID(Gtk::Stock::DIRECTORY).get_string());
	else
		row->set_value(m_columns.Image, Gtk::StockID(Gtk::Stock::FILE).get_string());
	
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(uri);
	
	if(file->query_exists())
		return uri;
	
	if(is_directory)
		file->make_directory();
	else
		file->create_file();
	
	// save();
	
	return uri;
}

void Project::remove(Gtk::TreeIter iter)
{
	if(!(*iter))
		return;

	if(m_delete_files) {
		Gtk::TreeModel::Row row = *iter;
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(get_uri(row));
		file->remove();
	}
	
	m_data->erase(iter);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                            Parsing Files                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Project::parse_files(xmlpp::Element *root) 
{
	Gtk::TreeModel::Row row;
	xmlpp::Element *node;
	
	xmlpp::Node::NodeList list = root->get_children();
	for(xmlpp::Node::NodeList::iterator i = list.begin(); i != list.end(); ++i) {
		if(node = dynamic_cast<xmlpp::Element *>(*i)) {
			if(node->get_name() == "file") {
				row = *m_data->append();
				
				row[m_columns.Image] = Gtk::StockID(Gtk::Stock::FILE).get_string();
				row[m_columns.Name] = trim(node->get_child_text()->get_content());
				row[m_columns.IsDirectory] = false;
			} else if(node->get_name() == "folder") {
				row = *m_data->append();
				
				row[m_columns.Image] = Gtk::StockID(Gtk::Stock::DIRECTORY).get_string();
				row[m_columns.IsDirectory] = true;
				
				parse_folder(node, row);
			}			
		}
	}
}

void Project::parse_files(xmlpp::Element *root, Gtk::TreeModel::Row &parent) 
{
	Gtk::TreeModel::Row row;
	xmlpp::Element *node;
	
	xmlpp::Node::NodeList list = root->get_children();
	for(xmlpp::Node::NodeList::iterator i = list.begin(); i != list.end(); ++i) {
		if(node = dynamic_cast<xmlpp::Element *>(*i)) {
			if(node->get_name() == "file") {
				row = *m_data->append(parent.children());
				
				row[m_columns.IsDirectory] = false;
				row[m_columns.Image] = Gtk::StockID(Gtk::Stock::FILE).get_string();
				row[m_columns.Name] = trim(node->get_child_text()->get_content());
				row[m_columns.Path] = parent[m_columns.Path] + parent[m_columns.Name] + "/";
			} else if(node->get_name() == "folder") {
				row = *m_data->append(parent.children());
			
				row[m_columns.Image] = Gtk::StockID(Gtk::Stock::DIRECTORY).get_string();
				row[m_columns.Path] = parent[m_columns.Path] + parent[m_columns.Name] + "/";
				row[m_columns.IsDirectory] = true;
				
				parse_folder(node, row);
			}			
		}
	}
}

void Project::parse_folder(xmlpp::Element *root, Gtk::TreeModel::Row &row)
{
	xmlpp::Element *node;
	xmlpp::Node::NodeList list = root->get_children();
	
	for(xmlpp::Node::NodeList::iterator i = list.begin(); i != list.end(); ++i) {
		if(node = dynamic_cast<xmlpp::Element *>(*i)) {
			if(node->get_name() == "name") {
				row[m_columns.Name] = trim(node->get_child_text()->get_content());
			} else if(node->get_name() == "files") {
				parse_files(node, row);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                              Writing Files                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Project::write_files(xmlpp::Element *root)
{
	Gtk::TreeModel::Row row;
	Gtk::TreeModel::Children list = m_data->children();
	xmlpp::Element *node;
	
	for(Gtk::TreeModel::iterator i = list.begin(); i != list.end(); i++) {
		row = *i;
		
		if(row[m_columns.IsDirectory]) {
			node = root->add_child("folder");
			write_folder(node, row);
		} else {
			node = root->add_child("file");
			node->set_child_text(row[m_columns.Name]);
		}
	}
}

void Project::write_files(xmlpp::Element *root, Gtk::TreeModel::Row &parent)
{
	Gtk::TreeModel::Row row;
	Gtk::TreeModel::Children list = parent.children();
	xmlpp::Element *node;
	
	for(Gtk::TreeModel::iterator i = list.begin(); i != list.end(); i++) {
		row = *i;
		
		if(row[m_columns.IsDirectory]) {
			node = root->add_child("folder");
			write_folder(node, row);
		} else {
			node = root->add_child("file");
			node->set_child_text(row[m_columns.Name]);
		}
	}
}

void Project::write_file(xmlpp::Element *root, Gtk::TreeModel::Row &row)
{
	xmlpp::Element *node = root->add_child("name");
	
	node->set_child_text(row[m_columns.Name]);
	
	if(row[m_columns.Path] != "") {
		node = root->add_child("path");
		node->set_child_text(row[m_columns.Path]);
	}
}

void Project::write_folder(xmlpp::Element *root, Gtk::TreeModel::Row &row)
{
	xmlpp::Element *node = root->add_child("name");
	
	node->set_child_text(row[m_columns.Name]);
	
	if(row[m_columns.Path] != "") {
		node = root->add_child("path");
		node->set_child_text(row[m_columns.Path]);
	}
	
	if(!row.children().empty()) {
		node = root->add_child("files");
		
		write_files(node, row);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         Parsing Build Instructions                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Project::parse_build_instructions(xmlpp::Element *root) 
{
	xmlpp::Element *node;
	xmlpp::Node::NodeList children = root->get_children();
	
	for(xmlpp::Node::NodeList::iterator i = children.begin(); i != children.end(); i++) {
		if(node = dynamic_cast<xmlpp::Element *>(*i)) {
			if(node->get_name() == "instruction")
				m_build_instructions.push_back(trim(node->get_child_text()->get_content()));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         Writing Build Instructions                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Project::write_build_instructions(xmlpp::Element *root)
{
	xmlpp::Element *instruction = NULL;
	
	for(std::vector<Glib::ustring>::iterator i = m_build_instructions.begin(); i != m_build_instructions.end(); i++) {
		instruction = root->add_child("instruction");
		instruction->set_child_text(*i);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                            Utility Functions                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int Project::sort(const Gtk::TreeModel::iterator &a, const Gtk::TreeModel::iterator &b)
{
	if(a->get_value(m_columns.IsDirectory) && !b->get_value(m_columns.IsDirectory))
		return -1;
	
	if(!a->get_value(m_columns.IsDirectory) && b->get_value(m_columns.IsDirectory))
		return 1;
			
	if(a->get_value(m_columns.Name) > b->get_value(m_columns.Name))
		return 1;
		
	if(a->get_value(m_columns.Name) < b->get_value(m_columns.Name))
		return -1;
		
	return 0;		
}

Glib::ustring Project::trim(Glib::ustring str) 
{
	Glib::ustring whitespace = " \a\b\f\n\r\t\v";
	
	if(str.length() == 0)
		return str;
		
	Glib::ustring::size_type beg = str.find_first_not_of(whitespace);
	Glib::ustring::size_type end = str.find_last_not_of(whitespace);
	
	if(beg == Glib::ustring::npos)
		return "";
		
	return Glib::ustring(str, beg, end - beg + 1);
}
