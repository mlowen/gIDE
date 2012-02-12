# gIDE

*This project is not currently maintained and has not had any active development since late 2009.*

## Introduction

gIDE aims to be an unobtrusive IDE which allows the user to do what they want: code without the IDE getting in their way.  To achieve this gIDE is not tied to any particular language or build tools, but does allow the user to configure the 
build system on a per-project basis.

gIDE was designed to integrate nicely with the GNOME desktop environment and uses many GTK and GNOME related technologies.

## Dependencies 

* gtkmm 2.4 (2.12.5)
* libgtksourceviewmm 2.0 (1.9.2)
* giomm 2.4 (2.16.0)
* libxml++ 2.6 (2.20.0)
* liglademm 2.4 (2.6.6)
* vtemm (0.17.2)
* gconfmm 2.6 (2.22.0)

## Build & Installation

In brief to build this project run the following from the projects base folder:

	./configure; make; make install

For more in-depth instructions see INSTALL.

## TODO

At the time the time that active development was stopped the following was the TODO list to reach version 1.

* Import from directory.
* Glade integration.
* Plugin system.