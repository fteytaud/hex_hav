// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef VIEW_DRAWING_HPP_
#define VIEW_DRAWING_HPP_

#include <hex_hav/Game.hpp>
class Controller;
class View;

#include <gtkmm.h>

class ViewDrawing : public Gtk::DrawingArea 
{
    private:
        Controller & _refController;
        View & _refView;

    public:
        ViewDrawing(Controller & refController, View & refView);
        void update();

    protected:
        bool on_expose_event(GdkEventExpose* ptrEvent);
        bool on_button_press_event(GdkEventButton* ptrEvent);

    private:
        void ij_to_xy(double radius, int i, int j, int & x, int & y) const;
        void xy_to_ij(double radius, int x, int y, int & i, int & j) const;
        void drawDisc(double radius, int x, int y, 
                Cairo::RefPtr<Cairo::Context> ptrContext);
        void drawHexagon(double radius, int x, int y, 
                Cairo::RefPtr<Cairo::Context> ptrContext);
        void drawEmptyHexagon(double radius, int x, int y, 
                Cairo::RefPtr<Cairo::Context> ptrContext);
        void setHexagon(double radius, int x, int y, 
                Cairo::RefPtr<Cairo::Context> ptrContext);
};

#endif
