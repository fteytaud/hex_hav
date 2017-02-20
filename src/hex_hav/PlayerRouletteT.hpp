
#ifndef PLAYER_ROULETTE_T_HPP_
#define PLAYER_ROULETTE_T_HPP_

template <typename T1, typename T2>
class PlayerRouletteT : public T1, public T2 
{
    private:
        enum playout_t { PLAYOUT1, PLAYOUT2 };
        playout_t _currentPlayout;
        Prng _prngRoulette;

    protected:
        void computeOneSimulationInit() override
        {
            PlayerRave::computeOneSimulationInit();
            int rnd = 2 * _prngRoulette.generate();
            _currentPlayout = (playout_t) rnd;
        }

        cell_t computeSimulation(NodeRave *ptrNode) override
        {
            if (_currentPlayout == PLAYOUT1)
                return T1::computeSimulation(ptrNode);
            else
                return T2::computeSimulation(ptrNode); 
        }

        void computeBackpropagation(NodeRave * ptrNode, cell_t winCellt) 
            override
        {
            if (_currentPlayout == PLAYOUT1)
                T1::computeBackpropagation(ptrNode, winCellt);
            else
                T2::computeBackpropagation(ptrNode, winCellt);
        }
};

#endif  // PLAYER_ROULETTE_T_HPP_

