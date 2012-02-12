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

#ifndef PROJECT_HH
#define PROJECT_HH

#include <vector>

#include <gtkmm.h>
#include <giomm.h>
#include <libxml++/libxml++.h>

class ProjectColumn : public Gtk::TreeModel::ColumnRecord {
	public:
		ProjectColumn();
		
		Gtk::TreeModelColumn<Glib::ustring> Name;
		Gtk::TreeModelColumn<Glib::ustring> Path;
		Gtk::TreeModelColumn<Glib::ustring> Image;
		Gtk::TreeModelColumn<bool> IsDirectory;
};

class Project {
	public:
		Project();
	
		bool load(Glib::ustring);
		bool save();
		void close();
		
		void attach_all_to_view(Gtk::TreeView *);

		Glib::ustring get_name() { return m_name; }
		void set_name(Glib::ustring name) { m_name = name; }
		Glib::ustring get_error() { return m_error; }
		Glib::ustring get_base_path() { return m_base_path; }
		Glib::ustring get_project_directory() { return m_file->get_parent()->get_path(); }
		ProjectColumn *get_columns() { return &m_columns; }
		bool is_local() { return m_file->is_native(); }
		
		Glib::ustring get_uri(Gtk::TreeModel::Row &);
		
		// Project Operations
		void new_project(Glib::ustring, Glib::ustring);
		
		Glib::ustring add(Glib::ustring, bool, Gtk::Window *);
		Glib::ustring add(Glib::ustring, bool, Gtk::TreeIter, Gtk::Window *);
		
		void remove(Gtk::TreeIter);
		
		// Delete files
		void set_delete_files(bool do_delete) { m_delete_files = do_delete; }
		
		// Build Instructions
		std::vector<Glib::ustring> get_build_instructions() { return m_build_instructions; }
		void set_build_instructions(std::vector<Glib::ustring> instructions) { m_build_instructions = instructions; }
		
		Glib::ustring trim(Glib::ustring);
	protected:	
		// Parsing files
		void parse_files(xmlpp::Element *);
		void parse_files(xmlpp::Element *, Gtk::TreeModel::Row &);
		void parse_folder(xmlpp::Element *, Gtk::TreeModel::Row &);
		
		// Writing files
		void write_files(xmlpp::Element *);
		void write_files(xmlpp::Element *, Gtk::TreeModel::Row &);
		void write_file(xmlpp::Element *, Gtk::TreeModel::Row &);
		void write_folder(xmlpp::Element *, Gtk::TreeModel::Row &);
		
		// Parsing build instructions
		void parse_build_instructions(xmlpp::Element *);
		
		// Writing build instructions
		void write_build_instructions(xmlpp::Element *);
		
		// Utility Functions
		virtual int sort(const Gtk::TreeModel::iterator &, const Gtk::TreeModel::iterator &);
	
	private:
		// Project properties
		Glib::ustring m_name;
		Glib::ustring m_base_path;
		bool m_delete_files;
	
		// Gtk data representation
		ProjectColumn m_columns;
		Glib::RefPtr<Gtk::TreeStore> m_data;

		// Build Instructions
		std::vector<Glib::ustring> m_build_instructions;

		// Project Files
		Glib::RefPtr<Gio::File> m_file;
		std::string m_e_tag;
		Glib::ustring m_error;
};

#endif // PROJECT_HH

