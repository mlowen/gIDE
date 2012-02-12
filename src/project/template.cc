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

#include <string>
#include <giomm.h>

#include "template.hh"

bool Template::parse(Glib::ustring uri) 
{
	Glib::RefPtr<Gio::File> fp = Gio::File::create_for_uri(uri);
	
	if(!fp->query_exists()) {
		m_error = "File " + uri + " does not exist.";
		return false;
	}
		
	bool info = false, build = false, create = false, files = false;
		
	char *buffer;
	std::string e_tag;
	gsize length; 
		
	if(!fp->load_contents(buffer, length, e_tag)) {
		m_error = "Unable to load file: " + uri;
		return false;
	}
		
	std::vector<Glib::ustring> lines = split(buffer, '\n');
	int pos = 0;
	int size = lines.size();
	Glib::ustring line;
		
	while(pos < size && pos != -1) {			
		line = trim(lines[pos]);
			
		if(line.size() == 0 || line[0] == '#') {
			pos++;
		} else if(line == "{INFO}") {
			if(info) {
				m_error = "Multiple declearations of INFO section in template: " + uri;
				return false;
			}
			
			info = true;
			pos = parse_info(lines, pos + 1);
		} else if(line == "{CREATE}") {
			if(build) {
				m_error = "Multiple declearations of CREATE section in template: " + uri;
				return false;
			}
			
			create = true;
			pos = parse_create(lines, pos + 1);
		} else if(line == "{FILES}") {
			if(files) {
				m_error = "Multiple declearations of FILES section in template: " + uri;
				return false;
			}
			
			files = true;
			pos = parse_files(lines, pos + 1);
		} else if(line == "{BUILD}" && !build) {
			if(build) {
				m_error = "Multiple declearations of BUILD section in template: " + uri;
				return false;
			}
			
			build = true;
			pos = parse_build(lines, pos + 1);
		} else {
			m_error = "Unknown Identifier " + line + " in template: " + uri;
			return false;
		}
	}
	
	if(pos == -1)
		return false;
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Create Project                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool Template::create(Glib::ustring base_uri, Glib::ustring name)
{
	if(base_uri[base_uri.size() - 1] != '/')
		base_uri += "/";
	
	// Create Files.
	Glib::RefPtr<Gio::File> project_folder = Gio::File::create_for_uri(base_uri + name);
		
	if(!project_folder->query_exists())
		project_folder->make_directory();
	
	// Create Project Directories
	std::sort(m_create_directories.begin(), m_create_directories.end());
	
	for(std::vector<Glib::ustring>::iterator i = m_create_directories.begin(); i != m_create_directories.end(); i++) {
		Glib::RefPtr<Gio::File> dir = Gio::File::create_for_uri(base_uri + name + "/" + *i);
		
		if(!dir->query_exists())
			dir->make_directory();
	}
	
	// Create files
	for(std::vector<Glib::ustring>::iterator j = m_create_files.begin(); j != m_create_files.end(); j++)
		create_file(base_uri + name + "/", *j, name);
	
	// Create project file
	return create_project_file(name, base_uri + name + "/" + name + ".project");
}

Glib::ustring Template::get_project_uri(Glib::ustring base_uri, Glib::ustring name)
{
	if(base_uri[base_uri.size() - 1] != '/')
		base_uri += "/";
	
	return base_uri + name + "/" + name + ".project";
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 Parsing                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int Template::parse_info(std::vector<Glib::ustring> lines, int pos)
{		
	bool name = false, language = false;
	int size = lines.size();
	Glib::ustring buffer;

	while(pos < size) {
		buffer = trim(lines[pos]);
		
		// Check if it is an empty line or a comment
		if(buffer.size() == 0 || buffer[0] == '#') {
			pos++;
			continue;
		}
		
		// Check if it is the end of the section
		if(buffer == "{END}")
			return pos + 1;
			
		Glib::ustring::size_type l = buffer.find_first_of(':');
		
		if(l == Glib::ustring::npos) {
			m_error = "Incorrect syntax: " + buffer;
			return -1;
		}
		
		Glib::ustring tag = buffer.substr(0, l);
		
		if(tag == "{NAME}") {
			if(name) {
				m_error = "Multiple declearations of NAME.";
				return -1;
			}
			
			name = true;
			m_name = buffer.substr(l + 1);
		} else if(tag == "{LANGUAGE}") {
			
		
			language = true;
			m_language = buffer.substr(l + 1);
		} else {
			m_error = "Unknown/Unexpected syntax: " + buffer;
			return -1;
		}
		
		pos++;
	}
	
	m_error = "Unexpected end of file.";
	return -1;
}

int Template::parse_create(std::vector<Glib::ustring> lines, int pos)
{
	int size = lines.size();
	Glib::ustring buffer;
	
	while(pos < size) {
		buffer = trim(lines[pos]);
		
		if(buffer.size() == 0 || buffer[0] == '#') {
			pos++;
			continue;
		}
		
		if(buffer == "{END}")
			return pos + 1;
		
		Glib::ustring::size_type p = buffer.find_first_of(':');
		
		if(p == Glib::ustring::npos) {
			m_error = "Incorrect syntax: " + buffer;
			return -1;
		}
		
		Glib::ustring tag = buffer.substr(0, p);
		
		buffer.substr(p + 1);
		
		if(tag == "{FILE}") {
			m_create_files.push_back(buffer.substr(p + 1));
		} else if(tag == "{DIR}") {
			m_create_directories.push_back(buffer.substr(p + 1));
		} else {
			m_error = "Unknown/Unexpected syntax: " + buffer;
			return -1;
		}
		
		pos++;
	}	

	m_error = "Unexpected end of file.";
	return -1;
}

int Template::parse_files(std::vector<Glib::ustring> lines, int pos)
{
	int size = lines.size();
	Glib::ustring buffer;
	
	while(pos < size) {
		buffer = trim(lines[pos]);
		
		if(buffer.size() == 0 || buffer[0] == '#') {
			pos++;
			continue;
		}
		
		if(buffer == "{END}")
			return pos + 1;
			
		Glib::ustring::size_type p = buffer.find_first_of(':');
		
		if(p == Glib::ustring::npos) {
			m_error = "Incorrect syntax: " + buffer;
			return -1;
		}
		
		Glib::ustring tag = buffer.substr(0, p);
		
		if(tag == "{FILE}") {
			TemplateFile f;
			
			f.Name = buffer.substr(p + 1);
			f.Content = "";
			pos++;
			
			while(pos < size && trim(lines[pos]) != "{END}") {
				f.Content += lines[pos] + "\n";					
				pos++;
			}
			
			if(pos >= size) {
				m_error = "Unexpected end of file.";
				return -1;
			}
			
			m_files.push_back(f);
		} else {
			m_error = "Unknown/Unexpected syntax: " + buffer;
		}
		
		pos++;
	}	

	m_error = "Unexpected end of file.";
	return -1;
}

int Template::parse_build(std::vector<Glib::ustring> lines, int pos)
{
	int size = lines.size();
	Glib::ustring buffer;
	
	while(pos < size) {
		buffer = trim(lines[pos]);
		
		if(buffer.size() == 0 || buffer[0] == '#') {
			pos++;
			continue;
		}
		
		if(buffer == "{END}")
			return pos + 1;
			
		Glib::ustring::size_type p = buffer.find_first_of(':');
		
		if(p == Glib::ustring::npos) {
			m_error = "Incorrect syntax: " + buffer;
			return -1;
		} else {
			Glib::ustring tag = buffer.substr(0, p);
			
			if(tag == "{INSTRUCTION}") {
				Glib::ustring instruction = buffer.substr(p + 1);
				m_build_instructions.push_back(instruction);
			} else {
				m_error = "Unknown/Unexpected syntax: " + buffer;
				return -1;
			}
			
			pos++;
		}
	}	
	
	m_error = "Unexpected end of file.";
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                 Creating                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Template::create_file(Glib::ustring base_uri, Glib::ustring file_name, Glib::ustring project_name) 
{
	Glib::ustring uri = base_uri + file_name;
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(uri);
	
	if(file->query_exists())
		return;
		
	Glib::RefPtr<Gio::FileOutputStream> fos = file->create_file();
	
	if(file_has_contents(file_name)) {
		fos->write(file_get_contents(file_name, project_name, uri));
		fos->flush();
	}
	
	fos->close();
}

bool Template::file_has_contents(Glib::ustring name)
{
	for(std::vector<TemplateFile>::iterator i = m_files.begin(); i != m_files.end(); i++) {
		if(i->Name == name) return true;
	}

	return false;
}

Glib::ustring Template::file_get_contents(Glib::ustring file_name, Glib::ustring project_name, Glib::ustring file_uri)
{
	Glib::ustring buffer;
	
	for(std::vector<TemplateFile>::iterator i = m_files.begin(); i != m_files.end(); i++) {
		if(i->Name == file_name) {
			buffer = i->Content;
			break;
		}
	}
	
	int pos = 0, size = buffer.size();
	Glib::ustring tag = "";
	
	while(pos < buffer.size()) {
		tag = "";
		
		if(buffer[pos] == '{' && buffer[pos + 1] == '%') {
			int temp = pos;
			
			temp += 2;
			
			while(buffer[temp] != '%' && buffer[temp + 1] != '}' && temp < size) {
				tag += buffer[temp];
				temp++; 
			}
			
			if(temp >= size)
				return buffer;
			
			if(tag == "NAME")
				buffer = buffer.replace(pos, tag.size() + 4, project_name); // The + 4 is for the {% and %} around the tag.
			else if(tag == "URI")
				buffer = buffer.replace(pos, tag.size() + 4, file_uri);
		} else {
			pos++;
		}
	}
	
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             XML File Building                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool Template::create_project_file(Glib::ustring project_name, Glib::ustring uri)
{
	try {
		xmlpp::Document doc;
		
		xmlpp::Element *root = doc.create_root_node("project");

		xmlpp::Element *node = root->add_child("name");
		node->set_child_text(project_name);
		
		node = root->add_child("files");
		
		for(std::vector<Glib::ustring>::iterator i = m_create_directories.begin(); i != m_create_directories.end(); i++)
			create_project_file_directories(node, split(*i, '/'));
			
		for(std::vector<Glib::ustring>::iterator j = m_create_files.begin(); j != m_create_files.end(); j++) {
			std::vector<Glib::ustring> path = split(*j, '/');
			Glib::ustring file_name = path.back();
			path.pop_back();
			
			create_project_file_files(node, file_name, path);
		}
		
		node = root->add_child("build");
		
		for(std::vector<Glib::ustring>::iterator k = m_build_instructions.begin(); k != m_build_instructions.end(); k++)
			node->add_child("instruction")->set_child_text(*k);
			
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(uri);
		
		if(file->query_exists()) {
			std::string e_tag = file->query_info()->get_etag();
			
			file->replace_contents(doc.write_to_string_formatted().c_str(), e_tag, e_tag);
		} else {
			Glib::RefPtr<Gio::FileOutputStream> fos = file->create_file();
	
			fos->write(doc.write_to_string_formatted());
			fos->flush();
			fos->close();
		}
	} catch(const std::exception &ex) {
		m_error = ex.what();
		
		return false;
	}
	
	return true;
}

std::vector<Glib::ustring> Template::create_project_file_directories(xmlpp::Element *parent, std::vector<Glib::ustring> dirs)
{
	if(dirs.empty())
		return dirs;
	
	Glib::ustring dir = dirs.front();
	
	xmlpp::Node::NodeList list = parent->get_children("folder");
	xmlpp::Element *node;
	xmlpp::Element *name;
	xmlpp::Element *files;
	
	for(xmlpp::Node::NodeList::iterator i = list.begin(); i != list.end(); i++) {
		if(node = dynamic_cast<xmlpp::Element *>(*i)) {
			// Can do this because we know the layout of the nodes
			name = dynamic_cast<xmlpp::Element *>(*node->get_children("name").begin());
			files = dynamic_cast<xmlpp::Element *>(*node->get_children("files").begin());
			
			if(name->get_child_text()->get_content() == dir) {
				dirs.erase(dirs.begin());
				dirs = create_project_file_directories(files, dirs);
			}
		}
	}
		
	if(!dirs.empty()) {
		node = parent->add_child("folder");
		node->add_child("name")->set_child_text(dir);
		files = node->add_child("files");
		
		dirs.erase(dirs.begin());
		dirs = create_project_file_directories(files, dirs);
	}
	
	return dirs;
}

void Template::create_project_file_files(xmlpp::Element *parent, Glib::ustring file_name, std::vector<Glib::ustring> path)
{
	xmlpp::Element *node;
	xmlpp::Element *name;
	xmlpp::Element *files;

	if(path.empty()) {
		node = parent->add_child("file");
		node->set_child_text(file_name);
		return;
	}
	
	Glib::ustring dir = path.front();
	path.erase(path.begin());
	
	xmlpp::Node::NodeList list = parent->get_children("folder");
	
	for(xmlpp::Node::NodeList::iterator i = list.begin(); i != list.end(); i++) {
		if(node = dynamic_cast<xmlpp::Element *>(*i)) {
			// Can do this because we know the layout of the nodes
			name = dynamic_cast<xmlpp::Element *>(*node->get_children("name").begin());
			files = dynamic_cast<xmlpp::Element *>(*node->get_children("files").begin());
			
			if(name->get_child_text()->get_content() == dir)
				create_project_file_files(files, file_name, path);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                   Utils                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::vector<Glib::ustring> Template::split(Glib::ustring str, char ch)
{
	std::vector<Glib::ustring> lines;
	Glib::ustring buf = "";
	int l = str.size();
	
	for(int i = 0; i < l; i++) {
		if(str[i] == ch) {
			lines.push_back(buf);
			buf = "";
		} else {
			buf += str[i];
		}
	}
	
	if(buf != "")
		lines.push_back(buf);
		
	return lines;
}

Glib::ustring Template::trim(Glib::ustring str) 
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

