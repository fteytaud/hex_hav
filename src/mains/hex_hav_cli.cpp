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

#include <hex_hav/Game.hpp>
#include <hex_hav/PlayerMcts.hpp>
#include <hex_hav/PlayerRaveLgrf1.hpp>
#include <hex_hav/PlayerRaveMast.hpp>
#include <hex_hav/PlayerRaveNast2.hpp>
#include <hex_hav/PlayerRavePr.hpp>
#include <hex_hav/PlayerRavePpr.hpp>
#include <hex_hav/PlayerRave.hpp>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using CrefString = const std::string &;
using UptrPlayer = std::unique_ptr<Player>;
using PlayerMap = std::map<std::string,std::function<UptrPlayer()>>;

template <typename U> 
UptrPlayer make_functor() { return std::make_unique<U>(); };

void printUsageAndExit(CrefString progName, const PlayerMap & playerMap)
{
    // display usage
    std::cerr << "usage: " << progName << " <game size> <nb games> <white AI> "
        << "white params> <black AI> <black params>" << std::endl;
    std::cerr << "example: " << progName 
        << " 7 10 Mcts 0.25_10000 Rave 70_0.25_10000" << std::endl;

    // display players and options
    for (const auto & p : playerMap)
    {
        // player name
        std::cerr << p.first << ':' << std::endl;
        // player options
        UptrPlayer uptr = p.second();
        for (const auto & pp : uptr->_parameters)
            std::cerr << "  - " << pp.first 
                << " (" << pp.second << ")" << std::endl;
    }
    exit(-1);
}

UptrPlayer createPlayer(CrefString progName, const PlayerMap & playerMap,
        CrefString playerName, CrefString playerOptions)
{
    // create player
    auto iterPlayer = playerMap.find(playerName);
    if (iterPlayer == playerMap.end())
    {
        std::cerr << "error: unknown player" << std::endl;
        printUsageAndExit(progName, playerMap);
    }

    // set player options
    UptrPlayer uptrPlayer = iterPlayer->second();
    std::istringstream ss(playerOptions);
    std::string str;
    for (auto & p : uptrPlayer->_parameters)
    {
        if (std::getline(ss, str, '_') and not str.empty())
            p.second = std::stod(str);
        else
        {
            std::cerr << "error: missing options" << std::endl;
            printUsageAndExit(progName, playerMap);
        }
    }
    if (std::getline(ss, str, '_'))
    {
        std::cerr << "error: too many options" << std::endl;
        printUsageAndExit(progName, playerMap);
    }
    return uptrPlayer;
}


void printPlayerHeader(const Player & player, CrefString playerName) 
{
    std::cout << playerName << "; ";
    for (const auto & p : player._parameters)
        std::cout << p.first << "; ";
}

void printPlayerData(const Player & player, CrefString playerType) 
{
    std::cout << playerType << "; ";
    for (const auto & p : player._parameters)
        std::cout << p.second << "; ";
}

int main(int argc, char ** argv) 
{
    // register functors for creating players
    PlayerMap playerMap;
    playerMap["Random"] = make_functor<PlayerRandom>;
    playerMap["Mc"] = make_functor<PlayerMc>;
    playerMap["Mcts"] = make_functor<PlayerMcts>;
    playerMap["Rave"] = make_functor<PlayerRave>;
    playerMap["RaveLgrf1"] = make_functor<PlayerRaveLgrf1>;
    playerMap["RaveMast"] = make_functor<PlayerRaveMast>;
    playerMap["RaveNast2"] = make_functor<PlayerRaveNast2>;
    playerMap["RavePr"] = make_functor<PlayerRavePr>;
    playerMap["RavePpr"] = make_functor<PlayerRavePpr>;

    // check command line arguments
    const std::string progName = argv[0];
    if (argc != 7) 
        printUsageAndExit(progName, playerMap);
    const unsigned int boardsize = std::stoi(argv[1]);
    const unsigned int nbGames = std::stoi(argv[2]);
    const double invNbGames = 1.0/double(nbGames);
    const std::string whiteName = argv[3];
    const std::string whiteOptions = argv[4];
    const std::string blackName = argv[5];
    const std::string blackOptions = argv[6];

    // get players
    UptrPlayer uptrWhitePlayer 
        = createPlayer(progName, playerMap, whiteName, whiteOptions);
    UptrPlayer uptrBlackPlayer 
        = createPlayer(progName, playerMap, blackName, blackOptions);

    // compute simulations
    Game game;
    double whiteWinRatio = 0;
    double blackWinRatio = 0;
    double meanNbTurns = 0;
    Chrono chrono;
    chrono.start();

    for (unsigned int i=0; i<nbGames; i++) 
    {
        // create game
        game.reset(boardsize, uptrWhitePlayer.get(), uptrBlackPlayer.get());

        // play game simulation
        int nbTurns = 0;
        while (game.hasTurnToPlay()) 
        {
            game.playTurn();
            nbTurns++; 
        }

        // get simulation results
        cell_t winner = game.getWinnerCellt();
        if (winner == CELL_WHITE) whiteWinRatio += invNbGames;
        else blackWinRatio += invNbGames;
        meanNbTurns += nbTurns*invNbGames;
    }
    chrono.stop();
    double averageTime = chrono.elapsedStopped()/double(nbGames); 

    // output
    // print header
    std::cout << "nb games; board type; board size; ";
    printPlayerHeader(*uptrWhitePlayer, "white");
    printPlayerHeader(*uptrBlackPlayer, "black");
    std::cout << "white ratio; black ratio; mean nb turns; average time;";
    std::cout << "draw; white ring; white bridge; white fork; " 
        << "black ring; black bridge; black fork;";
    std::cout << std::endl;

    // print data
    std::cout << nbGames << "; ";
    std::cout << game.getBoardType() << "; ";
    std::cout << boardsize << "; ";
    printPlayerData(*uptrWhitePlayer, whiteName);
    printPlayerData(*uptrBlackPlayer, blackName);
    std::cout << whiteWinRatio << "; ";
    std::cout << blackWinRatio << "; ";
    std::cout << meanNbTurns << "; ";
    std::cout << averageTime << "; ";

    // TODO refactor havannah stats
    for (int s : game._stats)
        std::cout << s*invNbGames << "; ";
    std::cout << std::endl;

    return 0;
}

