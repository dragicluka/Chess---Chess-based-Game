#ifndef ACTIVEKING_HPP
#define ACTIVEKING_HPP

#include "King.hpp"

class ActiveKing : public King
{
  protected:
    size_t mana_cost_;
  public:
    ActiveKing(PieceID id,PlayerColor owner_color, Coordinates coordinates, size_t mana_cost) : 
      King::King(id, owner_color, coordinates, PowerType::ACTIVE) , mana_cost_(mana_cost) {}


    /// @brief Getter for mana cost
    /// @return Returns mana cost as size_t.
    size_t getManaCost() override;

    /// @brief Pure virtual method implemented in subclasses.
    /// @param board Pointer to the board
    /// @param opponent Pointer to the opponent player.
    virtual void special(Board* board, Player* opponent) override = 0;
    virtual ~ActiveKing() = default;
};

class ArcherKing : public ActiveKing
{
  public:
    ArcherKing(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveKing::ActiveKing(PieceID::KARC, owner_color, coordinates, 0) 
    {
      short_name_ += "a";
    }

    /// @brief Freezes the opponent piece on target square
    /// @param board Pointer to the game board, unused
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;

    /// @brief Calculates the cost for activating special power of archer king
    /// @return Returns mana cost, as size_t.
    size_t getManaCost() override;
};

#endif