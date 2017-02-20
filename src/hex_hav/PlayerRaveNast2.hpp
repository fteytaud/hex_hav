
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

