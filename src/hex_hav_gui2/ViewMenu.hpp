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

};

#endif

