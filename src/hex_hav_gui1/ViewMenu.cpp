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
            Gtk::Action::create("MenuFileNewgame", "_New game..."), 
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
    Gtk::Dialog dialog("New game...", true);
    Gtk::VBox * ptrVBox = dialog.get_vbox();

    Gtk::HBox playerHBox;
    ptrVBox->pack_start(playerHBox);

    // game size
    Gtk::VBox gameVBox;
    gameVBox.set_border_width(8);
    playerHBox.pack_start(gameVBox);
    Gtk::Label sizeLabel("Game size: ", Gtk::ALIGN_START);
    gameVBox.pack_start(sizeLabel, Gtk::PACK_SHRINK);
    Gtk::Adjustment sizeAdjustment(_refController._sizeParameter, 5, 
            MAX_BOARDSIZE);
    Gtk::SpinButton sizeSpinButton(sizeAdjustment);
    gameVBox.pack_start(sizeSpinButton, Gtk::PACK_SHRINK);

    // white player
    Gtk::VBox whiteVBox;
    whiteVBox.set_border_width(8);
    playerHBox.pack_start(whiteVBox);
    Gtk::Label whiteLabel("white player: ", Gtk::ALIGN_START);
    whiteVBox.pack_start(whiteLabel);
    Gtk::ComboBoxText whiteCombo;
    std::vector<PlayerParam> whiteParams;
    playerDataToDialog(&whiteVBox, whiteCombo, whiteParams, 
            _refController._whitePlayers, _refController._whiteName);

    // black player
    Gtk::VBox blackVBox;
    blackVBox.set_border_width(8);
    playerHBox.pack_start(blackVBox);
    Gtk::Label blackLabel("black player: ", Gtk::ALIGN_START);
    blackVBox.pack_start(blackLabel);
    Gtk::ComboBoxText blackCombo;
    std::vector<ViewMenu::PlayerParam> blackParams;
    playerDataToDialog(&blackVBox, blackCombo, blackParams, 
            _refController._blackPlayers, _refController._blackName);

    // ok/cancel buttons
    dialog.add_button("Ok", Gtk::RESPONSE_OK); 
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL); 

    // run dialog
    dialog.show_all();
    int response = dialog.run();

    // handle response
    if (response == Gtk::RESPONSE_OK) 
    {
        _refController._sizeParameter = sizeSpinButton.get_value_as_int();
        dialogToPlayerData(whiteCombo, whiteParams, _refController._whitePlayers, 
                _refController._whiteName);
        dialogToPlayerData(blackCombo, blackParams, 
                _refController._blackPlayers, _refController._blackName);
        _refController.newGame();
    }
}

void ViewMenu::dialogToPlayerData(
        const Gtk::ComboBoxText & combo,
        const std::vector<ViewMenu::PlayerParam> & params,
        const std::map<std::string, std::unique_ptr<Player>> & players,
        std::string & refActivePlayerName) const 
{
    // get name of active player
    refActivePlayerName = combo.get_active_text();

    // get parameter values
    Player * ptrPlayer = players.at(refActivePlayerName).get();
    for (auto & param: params) 
        param.paramToPlayer(ptrPlayer);
}

void ViewMenu::playerDataToDialog(
        Gtk::VBox * ptrVBox,
        Gtk::ComboBoxText & combo,
        std::vector<ViewMenu::PlayerParam> & params,
        const std::map<std::string, std::unique_ptr<Player>> & players,
        const std::string & activePlayerName) const 
{
    params.reserve(50);
    for (auto & uptrPlayerPair : players) 
    {
        const std::string & playerName = uptrPlayerPair.first;
        Player * ptrPlayer = uptrPlayerPair.second.get();

        // add player
        combo.append_text(playerName);

        // get parameters
        for (auto & parameterPair : ptrPlayer->_parameters) 
        {
            const std::string & paramName = parameterPair.first;
            Glib::ustring pName(paramName);
            double paramValue = parameterPair.second;
            auto f = [pName] (const PlayerParam & param) -> bool 
            { return param._label.get_text() == pName; };
            auto iter = std::find_if(params.begin(), params.end(), f);
            if (iter == params.end())
                params.emplace_back(paramName, paramValue);
        }
    }
    combo.set_active_text(activePlayerName);
    ptrVBox->pack_start(combo);

    Gtk::Label * label = 
        Gtk::manage(new Gtk::Label("\nParameters: ", Gtk::ALIGN_START));
    ptrVBox->pack_start(*label);

    // show params
    for (auto & p : params) 	
        ptrVBox->pack_start(p);
}

void ViewMenu::handleMenuAbout() 
{
    Gtk::AboutDialog dialog;
    dialog.set_program_name("hex_hav_gui1");
    dialog.set_authors( std::vector<Glib::ustring> {"Fabien Teytaud", 
            "Julien Dehos", "Joris Duguépéroux", "Ahmad Mazyad"} );
    dialog.set_website("https://github.com/fteytaud/hex_hav");
    dialog.run();
}

ViewMenu::PlayerParam::PlayerParam(const PlayerParam &) : 
    sigc::trackable(), Glib::ObjectBase(), Gtk::HBox()
{
    assert(false);
}

ViewMenu::PlayerParam::PlayerParam(const std::string & name, double value) :
    _label(name, Gtk::ALIGN_START) 
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    _entry.set_text(ss.str());
    _label.set_padding(2, 0);
    pack_start(_label);
    pack_start(_entry, Gtk::PACK_SHRINK);
}

void ViewMenu::PlayerParam::paramToPlayer(Player * ptrPlayer) const 
{
    assert(ptrPlayer);
    // get parameter name
    std::string paramName = _label.get_text();
    // get parameter value
    std::stringstream ss;
    ss << _entry.get_text() << std::endl;
    double paramValue;
    ss >> paramValue;
    // set player parameter (if player has this parameter)
    auto iter = ptrPlayer->_parameters.find(paramName);
    if (iter != ptrPlayer->_parameters.end()) 
        iter->second = paramValue;
}

