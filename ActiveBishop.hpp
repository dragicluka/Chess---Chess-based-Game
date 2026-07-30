#ifndef ACTIVEBISHOP_HPP
#define ACTIVEBISHOP_HPP

#include "Bishop.hpp"

class ActiveBishop : public Bishop
{
  protected:
    size_t mana_cost_;
  public:
    ActiveBishop(PieceID id,PlayerColor owner_color, Coordinates coordinates, size_t mana_cost) : 
      Bishop::Bishop(id, owner_color, coordinates, PowerType::ACTIVE) , mana_cost_(mana_cost) {}

    /// @brief Getter for mana cost
    /// @return Returns mana cost as size_t.
    size_t getManaCost() override {return mana_cost_;}

    /// @brief Pure virtual method implemented in subclasses.
    /// @param board Pointer to the board
    /// @param opponent Pointer to the opponent player.
    virtual void special(Board* board, Player* opponent) override = 0;
    virtual ~ActiveBishop() = default;
};

class ColorBlindBishop : public ActiveBishop
{
  public:
    ColorBlindBishop(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveBishop::ActiveBishop(PieceID::BCLR, owner_color, coordinates, 3) 
    {
      short_name_ += "c";
    }

    /// @brief Executes the Color Blind Bishop's special move: relocates to the target square, adopting its color, 
    /// capturing any enemy piece, and collecting items.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;
};

class PreacherBishop : public ActiveBishop
{
  public:
    PreacherBishop(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveBishop::ActiveBishop(PieceID::BPRC, owner_color, coordinates, 0) 
    {
      short_name_ += "p";
    }

    /// @brief Converts the targeted opponent piece to this player's color.
    /// @param board Pointer to the game board, unused
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;

    /// @brief Calculates the cost for activating special power of preacher bishop
    /// @return Returns the cost, as size_t
    size_t getManaCost() override ;
};

#endif