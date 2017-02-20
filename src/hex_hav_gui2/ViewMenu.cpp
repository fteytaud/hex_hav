// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include "ViewMenu.hpp"

#include "Controller.hpp"
#include <set>
#include <iomanip>

ViewMenu::ViewMenu(Controller & refController, View & refView) :
    _refController(refController), 
    _refView(refView) 
{}

Gtk::Widget & ViewMenu::init(Gtk::Window & refWindow) 
{
    _ptrActionGroup = Gtk::ActionGroup::create();

    // file menu
    _ptrActionGroup->add(Gtk::Action::create("MenuFile", "_File"));
    _ptrActionGroup->add(
            Gtk::Action::create("MenuFileNewgame", "_New game"), 
            Gtk::AccelKey("<control>n"), 
            sigc::mem_fun(*this, &ViewMenu::handleFileNewgame));
    _ptrActionGroup->add(Gtk::Action::create("MenuFileQuit", "_Quit"), 
            Gtk::AccelKey("<control>q"), sigc::mem_fun(_refView, &View::quit));

    // help menu
    _ptrActionGroup->add(Gtk::Action::create("MenuHelp", "_Help"));
    _ptrActionGroup->add(Gtk::Action::create("MenuHelpAbout", "_About..."), 
            sigc::mem_fun(*this, &ViewMenu::handleMenuAbout));

    _ptrUIManager = Gtk::UIManager::create();
    _ptrUIManager->insert_action_group(_ptrActionGroup);
    refWindow.add_accel_group(_ptrUIManager->get_accel_group());

    Glib::ustring uiInfo;
    uiInfo += "<ui>";
    uiInfo += "  <menubar name='MenuBar'>";
    uiInfo += "    <menu action='MenuFile'>";
    uiInfo += "      <menuitem action='MenuFileNewgame'/>";
    uiInfo += "      <menuitem action='MenuFileQuit'/>";
    uiInfo += "    </menu>";
    uiInfo += "    <menu action='MenuHelp'>";
    uiInfo += "      <menuitem action='MenuHelpAbout'/>";
    uiInfo += "    </menu>";
    uiInfo += "  </menubar>";
    uiInfo += "</ui>";

    _ptrUIManager->add_ui_from_string(uiInfo);

    return *(_ptrUIManager->get_widget("/MenuBar"));
}

void ViewMenu::handleFileNewgame() 
{
    _refController.newGame();
}

void ViewMenu::handleMenuAbout() 
{
    Gtk::AboutDialog dialog;
    dialog.set_program_name("hstar");
    dialog.set_version("1.0");
    dialog.run();
}

