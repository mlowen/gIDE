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

#include <gtkmm.h>
#include <libglademm.h>
#include <gconfmm.h>
#include <gtksourceviewmm.h>
#include <giomm.h>
#include <libvtemm.h>

#include "../config.h"
#include "gide.hh"

int main(int argc, char *argv[])
{
	// Initialisation
	Gtk::Main kit(argc, argv);
	gtksourceview::init();
	Gio::init();
	Gnome::Conf::init();
	Gnome::Vte::init();
	
	// Set up the 
	Glib::ustring hidden_dir = "file://" + Glib::ustring(getenv("HOME")) + "/." + Glib::ustring(PACKAGE);
	
	Glib::RefPtr<Gio::File> dir = Gio::File::create_for_uri(hidden_dir);
	
	if(!dir->query_exists())
		dir->make_directory();
	
	gIDE *window;
	
	Glib::ustring prefix = INSTALL_PREFIX;
	Glib::RefPtr<Gnome::Glade::Xml> xml = Gnome::Glade::Xml::create(prefix + "/share/gIDE/ui/main.glade");
	
	xml->get_widget_derived("gide_main", window);
	window->setup(prefix);
	
	Gtk::Main::run(*window);

	delete window;

	return 0;
}
