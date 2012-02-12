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

#ifndef TEMPLATE_MANAGER_HH
#define TEMPLATE_MANAGER_HH

#include <gtkmm.h>

#include "template.hh"

struct TemplateList {
	Glib::ustring Language;
	std::vector<Template> Templates;
};

class LangaugeColumn : public Gtk::TreeModel::ColumnRecord {
	public:
		LangaugeColumn() { add(Langauge); }
		
		Gtk::TreeModelColumn<Glib::ustring> Langauge;
};

class TemplateColumn : public Gtk::TreeModel::ColumnRecord {
	public:
		TemplateColumn() { add(Name); add(Image); }
		
		Gtk::TreeModelColumn<Glib::ustring> Name;
		Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > Image;
};

class TemplateManager {
	public:
		TemplateManager();
	
		void setup(Glib::ustring);
	
		bool load(Glib::ustring);
	
		// Getters
		Template *get_template(Glib::ustring, Glib::ustring);
		Glib::ustring get_error() { return m_error; }
		
		Glib::RefPtr<Gtk::ListStore> get_langauges();
		Glib::RefPtr<Gtk::ListStore> get_templates() { return m_template_list; }
		Glib::RefPtr<Gtk::ListStore> get_templates(Glib::ustring);
		
		LangaugeColumn *get_langauge_column() { return &m_langauge_column; }
		TemplateColumn *get_template_column() { return &m_template_column; }
		
	private:
		Glib::RefPtr<Gtk::ListStore> m_langauges_list;
		Glib::RefPtr<Gtk::ListStore> m_template_list;
		Glib::RefPtr<Gdk::Pixbuf> m_default_image;
		
		LangaugeColumn m_langauge_column;
		TemplateColumn m_template_column;
	
		Glib::ustring m_error;
		Glib::ustring m_extension;
		Glib::ustring m_prefix;
		
		std::vector<TemplateList> m_languages;
		
		bool language_exists(Glib::ustring);
};

#endif // TEMPLATE_MANAGER_HH

