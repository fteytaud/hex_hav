
#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <hex_hav/BoardHex.hpp>
#include <hex_hav/BoardHavannah.hpp>
#include <hex_hav/Prng.hpp>

#include <list>
#include <map>
#include <memory>

// abstract class to implement players (human, AIs)
class Player 
{
    private:
        int _indexToPlay;
        bool _hasIndexToPlay;

    public:
        // TODO store player parameters as strings then convert them in player
        // implementation
        std::map<std::string, double> _parameters;

    public:
        Player();
        virtual ~Player() = default;

        virtual void newTurn(const BOARD_TYPE & refBoard) = 0;
        virtual void clickIndex(int index) = 0;

        void resetTurn();
        bool hasIndexToPlay() const;
        int getIndexToPlay() const;

        void setIndexToPlay(int index);
};

// human player : play the cells clicked by the user
class PlayerHuman : public Player 
{
    public:
        void newTurn(const BOARD_TYPE & refBoard) override ;
        void clickIndex(int index) override ;
};

// to implement an AI, derive from this class and define newTurn
class PlayerAi : public Player 
{
    public:
        void clickIndex(int index) override ;
};

// play randomly
class PlayerRandom : public PlayerAi 
{
    protected:
        Prng _prng;
    public:
        PlayerRandom();
        void newTurn(const BOARD_TYPE & refBoard) override ;
};

// basic Monte Carlo
class PlayerMc : public PlayerAi 
{
    protected:
        Prng _prng;
    public:
        PlayerMc();
        void newTurn(const BOARD_TYPE & refBoard) override ;
    private:
        cell_t playRandomGameWithFirstIndex(const BOARD_TYPE & refBoard, 
                int firstIndex);
};

#endif  // PLAYER_HPP_

