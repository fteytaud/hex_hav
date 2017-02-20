
#ifndef PLAYER_RAVE_HPP_
#define PLAYER_RAVE_HPP_

#include <hex_hav/PlayerMcts.hpp>

////////////////////////////////////////////////////////////////////////
// structures for evaluating cells (for debug purpose)
////////////////////////////////////////////////////////////////////////

struct CellEval {
    int _i;
    int _j;
    unsigned _nbSims;
    double _score;
};

struct BoardEval {
    static const int SIMS_THRESHOLD = 10;
    std::vector<CellEval> _cellEvals;
    unsigned _nbSims;
    unsigned _nbSimsBest;
    double _scoreBest;
    int _iBest;
    int _jBest;
};

////////////////////////////////////////////////////////////////////////
// Node struct for MCTS+RAVE tree
////////////////////////////////////////////////////////////////////////

struct NodeRave : NodeMcts 
{
    // a RAVE vector gives the number of time each cell has been played in the
    // sub tree 
    std::vector<double> _raveChildWins;  
    std::vector<double> _raveChildSims; 

    NodeRave(const BOARD_TYPE & b);
    NodeRave(const BOARD_TYPE & b, int index, NodeRave * ptrParentNode);

    BoardEval computeBoardEval(unsigned nbSims) const;
};

////////////////////////////////////////////////////////////////////////
// RAVE algorithm
////////////////////////////////////////////////////////////////////////

class PlayerRave : public PlayerMctsT<NodeRave> 
{
    protected:
        Prng _prngRaveSim;

    protected: 
        // board indices of the full game played by the last call to
        // computeSimulation, first move: root node, ...
        std::vector<int> _allMovesInSim;   

    public:
        PlayerRave();

        // TODO refactor board eval (PlayerMcts)
        BoardEval computeBoardEval() const;

    protected:
        double computeScoreUctRave(NodeRave * ptrNode) const;
        virtual void computeOneSimulationInit() override;
        virtual NodeRave * computeSelection(NodeRave * ptrNode) override;
        virtual int computeExpansion(NodeRave * ptrNode) override;
        virtual cell_t computeSimulation(NodeRave * ptrNode) override;
        virtual void computeBackpropagation(NodeRave * ptrNode, 
                cell_t winnerCellt) override;

#ifdef LOG
        virtual void printLog() override;
#endif  // LOG

};

#endif  // PLAYER_RAVE_HPP_

