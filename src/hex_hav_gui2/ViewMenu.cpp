/*
BSD 3-Clause License

Copyright (c) 2017, 
Fabien Teytaud, Julien Dehos, Joris Duguépéroux and Ahmad Mazyad
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
    dialog.set_program_name("hex_hav_gui2");
    dialog.set_authors( std::vector<Glib::ustring> {"Fabien Teytaud", 
            "Julien Dehos", "Joris Duguépéroux", "Ahmad Mazyad"} );
    dialog.set_website("https://github.com/fteytaud/hex_hav");
    dialog.run();
}

