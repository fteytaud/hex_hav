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

#endif  // VIEW_HPP_

