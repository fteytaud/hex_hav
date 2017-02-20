// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <hex_hav_gui1/View.hpp>

#include <hex_hav_gui1/Controller.hpp>

View::View(Controller & refController, int argc, char ** argv) :
    _refController(refController),
    _kit(argc, argv), 
    _menu(refController, *this),
    _drawing(refController, *this) 
{
    Gtk::Widget & refMenuWidget = _menu.init(_window);
    _vbox.pack_start(refMenuWidget, Gtk::PACK_SHRINK);
    _vbox.pack_start(_drawing);

    _statusbar.push("");
    _vbox.pack_start(_statusbar, Gtk::PACK_SHRINK);

    _window.set_size_request(400, 300);
    _window.set_title("HEX");
    //_window.set_resizable(false);
    _window.add(_vbox);
    _window.show_all();

    _isPlaying = false;
    _menuHeight = refMenuWidget.get_height();
    _statusHeight = _statusbar.get_height();
    _updateConnection = Glib::signal_timeout().connect(sigc::mem_fun(
                *this, &View::handleUpdate), 100);
}

void View::run() 
{
    _kit.run(_window);
}

bool View::handleUpdate() 
{
    // update board display
    _drawing.update();

    // update status bar
    if (_refController.isComputing()) displayStatus("Computing...");
    else if (_isPlaying) displayStatus("Click to play.");
    else displayStatus("");

    // display message box if there is a winner
    if (_refController.isGameFinished() and _isPlaying) 
    {
        _isPlaying = false;
        _updateConnection.disconnect();
        displayStatus("");
        cell_t winnerCellt = _refController.getWinnerCellt();
        std::string playerName = 
            Board<BOARD_TYPE>::convertCelltToString(winnerCellt);
        displayMessage("End of game", playerName);
        _updateConnection = Glib::signal_timeout().connect(sigc::mem_fun(
                    *this, &View::handleUpdate), 100);
    }

    return true;
}

void View::newGame() 
{
    int gameSize = _refController.getSize();
    int w = _hexagonRadius*sqrt(3.0)*1.5*(gameSize + 1.0);
    int h = _menuHeight + _statusHeight + _hexagonRadius*1.5*(gameSize+3.0);
    //_window.set_resizable(true);
    _window.set_size_request(w, h);
    //_window.set_resizable(false);
    _isPlaying = true;
}

void View::displayStatus(const std::string & refMessage) 
{
    _statusbar.pop();
    _statusbar.push(refMessage);
}

void View::displayMessage(const std::string & title, const std::string & text) 
{
    Glib::ustring titre(title.c_str());
    Glib::ustring message(text.c_str());
    Gtk::MessageDialog dialog(_window, message, false, Gtk::MESSAGE_INFO, 
            Gtk::BUTTONS_OK, true);
    dialog.set_title(titre);
    dialog.run();
}

void View::quit() 
{
    exit(0);
}

double View::getHexgonRadius() const 
{
    return _hexagonRadius;
}

double View::getDiscRadius() const 
{
    return _discRadius;
}

