// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <hex_hav/Game.hpp>

#include <hex_hav_gui2/ViewDrawing.hpp>
#include <hex_hav_gui2/ViewMenu.hpp>

class View 
{
    private:
        Controller & _refController;

        const double _hexagonRadius = 24;
        const double _discRadius = 6;

        int _menuHeight;
        bool _isPlaying;

        Gtk::Main _kit;
        Gtk::Window _window;

        Gtk::VBox _vbox;
        Gtk::HBox _hboxDrawing;
        Gtk::VBox _vboxEval;
        Gtk::Label _labelEval;
        Gtk::Entry _entryEval;
        Gtk::Button _buttonEval;
        Gtk::Label _labelEval2;

        ViewMenu _menu;
        ViewDrawing _drawing;

    public:
        View(Controller & refController, int argc, char ** argv);
        void run();
        void quit();

        void newGame();
        void update();

        double getHexgonRadius() const;
        double getDiscRadius() const;

        void displayMessage(const std::string& title, const std::string& text);
        void displayStatus(const std::string & refMessage);

    private:
        void handleComputeEval();
};

#endif
