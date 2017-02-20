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

#ifndef PLAYER_RAVE_NAST2_HPP_
#define PLAYER_RAVE_NAST2_HPP_

#include <hex_hav/PlayerRave.hpp>

// The n-Gram average sampling technique record.  A record is a n-gram seq of
// action, associated with the total reward and number of visits.  Here n = 2.
class PlayerRaveNast2 : public PlayerRave 
{
    private:
        // record structure for 2-grams
        struct Record 
        {
            int _action1; 
            int _action2; 
            int _wins;
            int _sims;
            double _cdf;
            Record() = delete;
            void updateCdf(double & cdf);
        };

    protected:
        // board indices of the full game played by the last call to
        // computeSimulation, first move: root node, ...
        std::map<int,Record> _nastRecords;
        Prng _prngNast2Cdf;

    public:
        PlayerRaveNast2() = default;
        void newTurn(const BOARD_TYPE & refBoard) override;

    protected:
        cell_t computeSimulation(NodeRave *ptrNode) override;

        void computeBackpropagation(NodeRave *ptrNode,
                cell_t winnerCellt) override;

#ifdef LOG
        virtual void printLog() override;
#endif  // LOG
};

#endif  // PLAYER_RAVE_NAST2_HPP_

