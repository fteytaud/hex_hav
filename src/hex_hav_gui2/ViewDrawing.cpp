// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <hex_hav_gui2/Controller.hpp>
#include <hex_hav_gui2/View.hpp>
#include <hex_hav_gui2/ViewDrawing.hpp>

// from http://paulbourke.net/texture_colour/colourspace/
void grayToRgb(double v, double vmin, double vmax, 
        double &r, double &g, double &b)
{
    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;

    double dv = vmax - vmin;
    r = 1.0;
    g = 1.0;
    b = 1.0;

    if (v < (vmin + 0.25 * dv)) 
    {
        r = 0;
        g = 4 * (v - vmin) / dv;
    } 
    else if (v < (vmin + 0.5 * dv)) 
    {
        r = 0;
        b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } 
    else if (v < (vmin + 0.75 * dv)) 
    {
        r = 4 * (v - vmin - 0.5 * dv) / dv;
        b = 0;
    }
    else 
    {
        g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
        b = 0;
    }
}

ViewDrawing::ViewDrawing(Controller & refController, View & refView) :
    _refController(refController), 
    _refView(refView) 
{
    add_events(Gdk::BUTTON_PRESS_MASK);
}

bool ViewDrawing::on_expose_event(GdkEventExpose * ) 
{
    Glib::RefPtr<Gdk::Window> refWindow = get_window();
    if(refWindow) 
    {
        Cairo::RefPtr<Cairo::Context> ptrContext = 
            refWindow->create_cairo_context();
        double hexagonRadius = _refView.getHexgonRadius();
        double discRadius = _refView.getDiscRadius();
        int size = _refController.getSize();

        // draw background
        ptrContext->set_source_rgb(1, 1, 1);
        ptrContext->rectangle(0, 0, refWindow->get_width(), 
                refWindow->get_height());
        ptrContext->fill();
        ptrContext->stroke();

        // draw cells
        std::vector<Cell> whiteCells, blackCells, emptyCells;
        _refController.guiCells(whiteCells, blackCells, emptyCells);

        for (const Cell & c : emptyCells) 
        {
            int x, y;
            ij_to_xy(hexagonRadius, c._i, c._j, x, y);
            ptrContext->set_source_rgb(0.8, 0.8, 0.8);
            drawHexagon(hexagonRadius, x, y, ptrContext);
            ptrContext->set_source_rgb(0, 0, 0);
            drawEmptyHexagon(hexagonRadius, x, y, ptrContext);
        }

        // draw cell evaluation
        if (_refController.isBoardEvalOk())
        {
            auto boardEval = _refController.getBoardEval();
            for (const CellEval & c : boardEval._cellEvals) 
            {
                if (c._nbSims > BoardEval::SIMS_THRESHOLD)
                {
                    double r, g, b;
                    grayToRgb(c._score, 0.0, 1.0, r, g, b);
                    int x, y;
                    ij_to_xy(hexagonRadius, c._i, c._j, x, y);
                    ptrContext->set_source_rgb(r, g, b);
                    drawHexagon(hexagonRadius, x, y, ptrContext);
                    ptrContext->set_source_rgb(0, 0, 0);
                    drawEmptyHexagon(hexagonRadius, x, y, ptrContext);
                }
            }

            // draw colormap
            {
                int x0 = 620;
                int x1 = 660;
                int y0 = 20;
                int dy = 2;
                ptrContext->save();
                ptrContext->set_line_width(dy);
                int y = 0;
                for (double k=1.0; k>=0.0; k-=0.005)
                {
                    double r, g, b;
                    grayToRgb(k, 0.0, 1.0, r, g, b);
                    ptrContext->set_source_rgb(r, g, b);
                    ptrContext->move_to(x0, y0+y);
                    ptrContext->line_to(x1, y0+y);
                    ptrContext->stroke();
                    y += dy;
                }
                ptrContext->restore();
            }

            // draw best stone
            {
                int x, y;
                ptrContext->set_source_rgb(0.0, 0.0, 0.0);
                ij_to_xy(hexagonRadius, boardEval._iBest, boardEval._jBest, x, y);
                drawDisc(discRadius, x, y, ptrContext);
            }

        }

        for (const Cell & c : whiteCells) 
        {
            int x, y;
            ij_to_xy(hexagonRadius, c._i, c._j, x, y);
            ptrContext->set_source_rgb(1, 1, 1);
            drawHexagon(hexagonRadius, x, y, ptrContext);
            ptrContext->set_source_rgb(0, 0, 0);
            drawEmptyHexagon(hexagonRadius, x, y, ptrContext);
        }

        for (const Cell & c : blackCells) 
        {
            int x, y;
            ij_to_xy(hexagonRadius, c._i, c._j, x, y);
            ptrContext->set_source_rgb(0.4, 0.4, 0.4);
            drawHexagon(hexagonRadius, x, y, ptrContext);
            ptrContext->set_source_rgb(0, 0, 0);
            drawEmptyHexagon(hexagonRadius, x, y, ptrContext);
        }

        // show current cellt
        cell_t currentCellt = _refController.getCurrentCellt();
        cell_t winnerCellt = _refController.getWinnerCellt();
        if (currentCellt != CELL_EMPTY and winnerCellt == CELL_EMPTY) 
        {
            if (currentCellt == CELL_WHITE)
                ptrContext->set_source_rgb(1, 1, 1);
            else
                ptrContext->set_source_rgb(0.4, 0.4, 0.4);
            int x, y;
            ij_to_xy(hexagonRadius, size, -(size+1)/2, x, y);
            drawHexagon(hexagonRadius, x, y, ptrContext);
            ptrContext->set_source_rgb(0, 0, 0);
            drawEmptyHexagon(hexagonRadius, x, y, ptrContext);
        }

    }

    return true;
}

bool ViewDrawing::on_button_press_event(GdkEventButton* ptrEvent) 
{
    int i, j;
    xy_to_ij(_refView.getHexgonRadius(), ptrEvent->x, ptrEvent->y, i, j);
    _refController.clickCell(Cell(i, j));
    return true;
}

void ViewDrawing::ij_to_xy(double radius, int i, int j, int & x, int & y) const
{
    x = radius * sqrt(3.0)*0.5 * (2.0 * j + i + 3);
    y = 1.5 * radius * (i + 2.0);
}

void ViewDrawing::xy_to_ij(double radius, int x, int y, int & i, int & j) const 
{
    double xr = x/radius;
    double yr = y/radius;
    i = (yr*2.0/3.0) - 1.5;
    j = xr/sqrt(3.0) - yr/3.0;
}

void ViewDrawing::update() 
{
    Glib::RefPtr<Gdk::Window> refWindow = get_window();
    if(refWindow) refWindow->invalidate(false);
}

void ViewDrawing::drawHexagon(double radius, int x, int y, 
        Cairo::RefPtr<Cairo::Context> ptrContext) 
{
    setHexagon(radius, x, y, ptrContext);
    ptrContext->fill();
    ptrContext->stroke();
}

void ViewDrawing::drawEmptyHexagon(double radius, int x, int y, 
        Cairo::RefPtr<Cairo::Context> ptrContext) 
{
    setHexagon(radius, x, y, ptrContext);
    ptrContext->stroke();
}

void ViewDrawing::drawDisc(double radius, int x, int y, 
        Cairo::RefPtr<Cairo::Context> ptrContext) {
    ptrContext->save();
    ptrContext->translate(x, y);
    ptrContext->scale(radius, radius);
    ptrContext->arc(0, 0, 1, 0, 2*M_PI);
    ptrContext->fill();
    ptrContext->restore();
    ptrContext->stroke();
}

void ViewDrawing::setHexagon(double radius, int x, int y, 
        Cairo::RefPtr<Cairo::Context> ptrContext) 
{
    const int rx = radius * sqrt(3.0)*0.5 + 0.5;
    const int ry = radius * 0.5;
    ptrContext->move_to(x, y-radius);
    ptrContext->line_to(x-rx, y-ry);
    ptrContext->line_to(x-rx, y+ry);
    ptrContext->line_to(x, y+radius);
    ptrContext->line_to(x+rx, y+ry);
    ptrContext->line_to(x+rx, y-ry);
    ptrContext->line_to(x, y-radius);
}
