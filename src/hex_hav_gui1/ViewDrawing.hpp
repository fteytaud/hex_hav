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

#endif  // VIEW_DRAWING_HPP_

