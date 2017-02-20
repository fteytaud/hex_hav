
#ifndef PLAYER_MCTS_T_HPP_
#define PLAYER_MCTS_T_HPP_

#include <hex_hav/Player.hpp>

#include <cassert>
#include <iostream>

////////////////////////////////////////////////////////////////////////
// template declaration
////////////////////////////////////////////////////////////////////////

template<typename T>
class PlayerMctsT : public PlayerAi 
{
    protected:
        std::vector<T> _nodes;
        cell_t _playerCellt;

    public:
        PlayerMctsT();
        virtual void newTurn(const BOARD_TYPE & refBoard) override;

    protected:
        virtual void computeOneSimulationInit() {}
        void computeOneSimulation();

    protected:
        virtual T * computeSelection(T * ptrNode) = 0;
        virtual int computeExpansion(T * ptrNode) = 0;
        virtual cell_t computeSimulation(T * ptrNode) = 0;
        virtual void computeBackpropagation(T * ptrNode, cell_t winCellt) = 0;

#ifdef LOG
        virtual void printLog() {}
#endif  // LOG

};

////////////////////////////////////////////////////////////////////////
// template implementation
////////////////////////////////////////////////////////////////////////

    template<typename T>
PlayerMctsT<T>::PlayerMctsT() 
{
    _parameters["nbSimulations"] = 10000;
}

    template<typename T>
void PlayerMctsT<T>::computeOneSimulation() 
{

    computeOneSimulationInit();

    // selection : select a leaf node
    T * ptrCurrentNode = std::addressof(_nodes.front());
    while (not ptrCurrentNode->_childNodes.empty() 
            and ptrCurrentNode->_possibleIndices.empty()) 
    {
        ptrCurrentNode = computeSelection(ptrCurrentNode);
    }

    // expansion : unless the current node ends the game, create a new node
    if (not ptrCurrentNode->_board.isGameFinished()) 
    {
        // select one cell index
        int index = computeExpansion(ptrCurrentNode);

        // create a node
        const BOARD_TYPE & board = ptrCurrentNode->_board;
        _nodes.emplace_back(board, index, ptrCurrentNode);

        // store this new child node in parent node
        T * ptrChildNode = std::addressof(_nodes.back());
        ptrCurrentNode->_childNodes.push_back(ptrChildNode);

        // select child node as current node
        ptrCurrentNode = ptrChildNode;
    }

    // simulation
    cell_t winnerCellt = computeSimulation(ptrCurrentNode);

    // backpropagation
    computeBackpropagation(ptrCurrentNode, winnerCellt);
}

    template<typename T>
void PlayerMctsT<T>::newTurn(const BOARD_TYPE & refBoard) 
{

    const unsigned int nbSimulations = _parameters["nbSimulations"];
    _playerCellt = refBoard.getCurrentCellt();

    // build search tree
    _nodes.clear();
    // warning: _nodes must be fully reserved !!!
    _nodes.reserve(nbSimulations+1);
    _nodes.emplace_back(refBoard);

    // compute simulations
    for (unsigned int i=0; i<nbSimulations; i++)
        computeOneSimulation();

    // get best cell to play
    T * ptrBestNode = (T *)_nodes.front().getBestNode();
    assert(ptrBestNode);
    setIndexToPlay(ptrBestNode->_index);

#ifdef LOG
    printLog();
    std::clog << BOARD_TYPE::convertCelltToString(refBoard.getCurrentCellt())
        << "\t(nbSimulations = " << nbSimulations 
        << ")\twinRatio = " << ptrBestNode->getWinRatio() << std::endl;
#endif  // LOG

}

#endif  // PLAYER_MCTS_T_HPP_

