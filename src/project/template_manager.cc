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

#include <giomm.h>

#include "template_manager.hh"

TemplateManager::TemplateManager()
{	
	m_extension = ".template";
}

void TemplateManager::setup(Glib::ustring prefix)
{
	m_prefix = prefix + "/share/pixmaps/gIDE/";
	
	m_default_image = Gdk::Pixbuf::create_from_file(m_prefix + "project.png");
}

bool TemplateManager::load(Glib::ustring uri)
{	
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(uri);
	
	if(uri.find_last_of('/') != uri.length() - 1)
		uri += "/";
	
	if(!file->query_exists()) {
		m_error = "Directory " + uri + " does not exist.";
		return false;
	}
	
	Glib::RefPtr<Gio::FileInfo> info = file->query_info();
	
	if(info->get_file_type() != Gio::FILE_TYPE_DIRECTORY) {
		m_error = "Uri " + uri + " is not a directory.";
		return false;
	}
	
	Glib::RefPtr<Gio::FileEnumerator> children = file->enumerate_children();
	
	for(info = children->next_file(); info; info = children->next_file()) {
		Glib::ustring name = info->get_name();
		
		// A couple of quick sanity checks
		if(info->get_file_type() != Gio::FILE_TYPE_REGULAR || info->is_hidden() || info->is_backup())
			continue;
		
		if(name.substr(name.length() - m_extension.length()) == m_extension) {
			Template t;
			
			if(t.parse(uri + name)) {
				// Check if a template by this name for this langauge alreay exists
				if(get_template(t.get_name(), t.get_language()) != NULL)
					continue;
			
				if(language_exists(t.get_language())) {
					for(std::vector<TemplateList>::iterator i = m_languages.begin(); i != m_languages.end(); i++) {
						if(i->Language == t.get_language()) {
							i->Templates.push_back(t);
							break;
						}
					}
				} else {
					TemplateList list;
					
					list.Language = t.get_language();
					
					list.Templates.push_back(t); 
					
					m_languages.push_back(list);
				}
			}
		}
	}
	
	return true;
}

Template *TemplateManager::get_template(Glib::ustring language, Glib::ustring name)
{
	for(std::vector<TemplateList>::iterator i = m_languages.begin(); i != m_languages.end(); i++) {
		if(i->Language == language) {
			for(std::vector<Template>::iterator j = i->Templates.begin(); j != i->Templates.end(); j++) {
				if(j->get_name() == name)
					return &(*j);
			}
			
			m_error = "Template does not exist.";
			return NULL;
		}
	}
	
	m_error = "Template does not exist.";
	return NULL;
}

Glib::RefPtr<Gtk::ListStore> TemplateManager::get_langauges()
{
	if(!m_langauges_list) { // This is the first time it is being called better create it.
		m_langauges_list = Gtk::ListStore::create(m_langauge_column);
		Gtk::TreeModel::Row row;
		
		for(std::vector<TemplateList>::iterator i = m_languages.begin(); i != m_languages.end(); i++) {
			row = *m_langauges_list->append();
			
			row[m_langauge_column.Langauge] = i->Language;
		}
	}

	return m_langauges_list; 
}

Glib::RefPtr<Gtk::ListStore> TemplateManager::get_templates(Glib::ustring language)
{
	m_template_list = Gtk::ListStore::create(m_template_column);
	
	for(std::vector<TemplateList>::iterator i = m_languages.begin(); i != m_languages.end(); i++) {
		if(i->Language == language) {
			for(std::vector<Template>::iterator j = i->Templates.begin(); j != i->Templates.end(); j++) {
				Gtk::TreeModel::Row row = *m_template_list->append();
								
				row[m_template_column.Name] = j->get_name();
				row[m_template_column.Image] = m_default_image;
			}
		
			break;
		}
	}
	
	return m_template_list;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                            Private Functions                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool TemplateManager::language_exists(Glib::ustring langauge) 
{
	for(std::vector<TemplateList>::iterator i = m_languages.begin(); i != m_languages.end(); i++) {
		if(i->Language == langauge)
			return true;
	}
	
	return false;
}
