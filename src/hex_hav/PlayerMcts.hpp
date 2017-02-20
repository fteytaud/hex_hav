
#ifndef PLAYER_MCTS_HPP_
#define PLAYER_MCTS_HPP_

#include <hex_hav/PlayerMctsT.hpp>

////////////////////////////////////////////////////////////////////////
// Node struct for MTCS tree
// A NodeMcts implements a game state during MCTS search
////////////////////////////////////////////////////////////////////////

struct NodeMcts 
{
    // current board of the node
    BOARD_TYPE _board;   

    // index of the node, played in the board (so _board.getCurrentCellt() is
    // the opponent of _index cell_t)
    int _index;     

    // number of winning simulations (for root cell_t) which have used the node
    unsigned _nbWins;       

    // number of simulations which have used the node
    unsigned _nbSims;	

    // child nodes in MCTS tree, ie empty cells of _board which have been
    // considered as the following cell to play (by the opponent)
    std::vector<NodeMcts*> _childNodes;

    // empty indices of _board which have not been considered yet (no
    // corresponding child node) so "_board.getEmptyIndices() == _childNodes +
    // _possibleIndices"
    std::vector<int> _possibleIndices;

    // parent Mcts node (so "this" should be in _ptrParentNode->_childNodes)
    NodeMcts * _ptrParentNode;

    NodeMcts(const BOARD_TYPE & b);
    NodeMcts(const BOARD_TYPE & b, int index, NodeMcts * ptrParentNode);
    NodeMcts * getBestNode() const;
    double getWinRatio() const;
};

////////////////////////////////////////////////////////////////////////
// class for implementing the MCTS algorithm
////////////////////////////////////////////////////////////////////////

class PlayerMcts : public PlayerMctsT<NodeMcts> 
{
    protected:
        Prng _prngMctsSim;

    public:
        PlayerMcts();

    protected:
        NodeMcts * computeSelection(NodeMcts * ptrNode) override ;
        int computeExpansion(NodeMcts * ptrNode) override ;
        cell_t computeSimulation(NodeMcts * ptrNode) override ;
        void computeBackpropagation(NodeMcts * ptrNode, cell_t winnerCellt) 
            override ;

    private:
        double computeScoreUct(NodeMcts * ptrNode) const;
};

#endif  // PLAYER_MCTS_HPP_

