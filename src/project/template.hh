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

#ifndef TEMPLATE_HH
#define TEMPLATE_HH

#include <vector>
#include <glibmm.h>
#include <libxml++/libxml++.h>

#include "project.hh"

struct TemplateFile {
	Glib::ustring Name;
	Glib::ustring Content;
};

class Template {
	public:		
		bool parse(Glib::ustring);
		bool create(Glib::ustring, Glib::ustring);
			
		Glib::ustring get_error() { return m_error; }
		Glib::ustring get_name() { return m_name; }
		Glib::ustring get_language() { return m_language; }
		
		Glib::ustring get_project_uri(Glib::ustring, Glib::ustring);

	private:
		Glib::ustring m_name;
		Glib::ustring m_language;
			
		std::vector<Glib::ustring> m_create_files;
		std::vector<Glib::ustring> m_create_directories;
		std::vector<Glib::ustring> m_build_instructions;
		std::vector<TemplateFile> m_files;
			
		Glib::ustring m_error;
			
		// Parsing
		int parse_info(std::vector<Glib::ustring>, int);
		int parse_create(std::vector<Glib::ustring>, int);
		int parse_files(std::vector<Glib::ustring>, int);
		int parse_build(std::vector<Glib::ustring>, int);
			
		// Creating
		void create_file(Glib::ustring, Glib::ustring, Glib::ustring);
		bool file_has_contents(Glib::ustring);
		Glib::ustring file_get_contents(Glib::ustring, Glib::ustring, Glib::ustring);
			
		// Building XML file
		bool create_project_file(Glib::ustring, Glib::ustring);
		std::vector<Glib::ustring> create_project_file_directories(xmlpp::Element *, std::vector<Glib::ustring>);
		void create_project_file_files(xmlpp::Element *, Glib::ustring, std::vector<Glib::ustring>);
			
		// Util
		std::vector<Glib::ustring> split(Glib::ustring, char);
		Glib::ustring trim(Glib::ustring);
};

#endif // TEMPLATE_HH
