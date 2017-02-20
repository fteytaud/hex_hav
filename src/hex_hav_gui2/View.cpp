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

#include <hex_hav_gui2/View.hpp>
#include <hex_hav_gui2/Controller.hpp>

#include <iomanip>

View::View(Controller & refController, int argc, char ** argv) :
    _refController(refController),
    _kit(argc, argv), 
    _labelEval("nb sims:                       "),
    _buttonEval("compute"),
    _labelEval2(""),
    _menu(refController, *this),
    _drawing(refController, *this) 
{
    Gtk::Widget & refMenuWidget = _menu.init(_window);
    _vbox.pack_start(refMenuWidget, Gtk::PACK_SHRINK);

    _vbox.pack_start(_hboxDrawing);
    _hboxDrawing.pack_start(_vboxEval, Gtk::PACK_SHRINK);
    _vboxEval.pack_start(_labelEval, Gtk::PACK_SHRINK);
    _vboxEval.pack_start(_entryEval, Gtk::PACK_SHRINK);
    _entryEval.set_text("10000");
    _vboxEval.pack_start(_buttonEval, Gtk::PACK_SHRINK);
    _vboxEval.pack_start(_labelEval2, Gtk::PACK_SHRINK);
    _hboxDrawing.pack_start(_drawing);

    _window.set_size_request(900, 400);
    _window.set_title("HEX");
    //_window.set_resizable(false);
    _window.add(_vbox);
    _window.show_all();

    _isPlaying = false;
    _menuHeight = refMenuWidget.get_height();

    _buttonEval.signal_clicked().connect(sigc::mem_fun(
                *this, &View::handleComputeEval));

}

void View::run() 
{
    _kit.run(_window);
}

void View::handleComputeEval()
{
    if (not _refController.isGameFinished())
    {
        int nbSims = std::stoi(_entryEval.get_text());
        _refController.updateBoardEval(nbSims);
    }
}

void View::update() 
{
    // update board display
    _drawing.update();

    if (_refController.isBoardEvalOk())
    {
        auto boardEval = _refController.getBoardEval();
        std::stringstream ss;
        ss << "nbSims: " << boardEval._nbSims << std::endl;
        ss << "nbSimsBest: " << boardEval._nbSimsBest << std::endl;
        ss << "scoreBest: " << std::setprecision(3) << boardEval._scoreBest << std::endl;
        _labelEval2.set_text(ss.str());
    }
    else
    {
        _labelEval2.set_text("");
    }

    // display message box if there is a winner
    if (_refController.isGameFinished() and _isPlaying) 
    {
        _isPlaying = false;
        cell_t winnerCellt = _refController.getWinnerCellt();
        std::string playerName = 
            Board<BOARD_TYPE>::convertCelltToString(winnerCellt);
        displayMessage("End of game", playerName);
    }
}

void View::newGame() 
{
    int gameSize = _refController.getSize();
    int w = _hexagonRadius*sqrt(3.0)*1.5*(gameSize + 1.0);
    int h = _menuHeight + _hexagonRadius*1.5*(gameSize+3.0);
    //_window.set_resizable(true);
    _window.set_size_request(w, h);
    //_window.set_resizable(false);
    _isPlaying = true;
    update();
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

