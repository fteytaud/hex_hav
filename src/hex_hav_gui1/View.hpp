// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <hex_hav/Game.hpp>
#include <hex_hav_gui1/ViewDrawing.hpp>
#include <hex_hav_gui1/ViewMenu.hpp>

#include <thread>

class View 
{
    private:
        Controller & _refController;

        const double _hexagonRadius = 24;
        const double _discRadius = 6;
        int _menuHeight;
        int _statusHeight;
        bool _isPlaying;

        Gtk::Main _kit;
        Gtk::Window _window;
        Gtk::VBox _vbox;
        ViewMenu _menu;
        ViewDrawing _drawing;
        Gtk::Statusbar _statusbar;

        sigc::connection _updateConnection;

    public:
        View(Controller & refController, int argc, char ** argv);
        void run();
        void quit();

        void newGame();

        double getHexgonRadius() const;
        double getDiscRadius() const;

        void displayMessage(const std::string& title, const std::string& text);
        void displayStatus(const std::string & refMessage);

    private:
        bool handleUpdate();
};

#endif
