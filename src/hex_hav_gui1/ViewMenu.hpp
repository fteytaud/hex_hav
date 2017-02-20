// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEW_MENU_HPP_
#define VIEW_MENU_HPP_

#include <gtkmm.h>

#include <hex_hav/Player.hpp>

class Controller;
class View;

class ViewMenu 
{

    struct PlayerParam : public Gtk::HBox 
    {
        Gtk::Label _label;
        Gtk::Entry _entry;
        PlayerParam(const std::string & name, double value);
        PlayerParam(const PlayerParam &);
        void paramToPlayer(Player * ptrPlayer) const;
    };

    Controller & _refController;
    View & _refView;

    Glib::RefPtr<Gtk::UIManager> _ptrUIManager;
    Glib::RefPtr<Gtk::ActionGroup> _ptrActionGroup;

    public:
    ViewMenu(Controller & refController, View & refView);
    Gtk::Widget & init(Gtk::Window & refWindow);

    private:
    void handleFileNewgame();
    void handleMenuAbout();

    void dialogToPlayerData(
            const Gtk::ComboBoxText & combo,
            const std::vector<ViewMenu::PlayerParam> & params,
            const std::map<std::string, std::unique_ptr<Player>> & players,
            std::string & refActivePlayerName) const;

    void playerDataToDialog(
            Gtk::VBox * ptrVBox,
            Gtk::ComboBoxText & combo,
            std::vector<ViewMenu::PlayerParam> & params,
            const std::map<std::string, std::unique_ptr<Player>> & players,
            const std::string & playerName) const;
};

#endif

