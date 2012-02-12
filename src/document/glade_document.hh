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

#ifndef GLADE_DOCUMENT_HH
#define GLADE_DOCUMENT_HH

#include "document.hh"

//#include <glade/glade.h>
//#include <gladeui/glade.h>
//#include <gladeui/glade-design-view.h>
//#include <gladeui/glade-popup.h>
//#include <gladeui/glade-inspector.h>

class GladeDocument : public Document {
	public:
		GladeDocument();
	
		// Operations
		virtual void copy();
		virtual void cut();
		virtual void paste();
		virtual void undo();
		virtual void redo();
		virtual void find();
		virtual void replace();
	
		//static GladeApp *glade_app;
	
	protected:
		// I/O
		virtual bool read();
		virtual bool write();
	
	private:
		// Glade Stuff
		//GladeProject *m_project;
		//GladeEditor *m_editor;
		//GladePalette *m_pallete;
		//GladeInspector *m_inspector;
		//GladeSignalEditor *signal_editor;
};

#endif // GLADE_DOCUMENT_HH
