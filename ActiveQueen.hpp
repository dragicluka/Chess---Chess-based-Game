#ifndef ACTIVEQUEEN_HPP
#define ACTIVEQUEEN_HPP

#include "Queen.hpp"

class ActiveQueen : public Queen
{
  protected:
    size_t mana_cost_;
  public:
    ActiveQueen(PieceID id,PlayerColor owner_color, Coordinates coordinates, size_t mana_cost) : 
      Queen::Queen(id, owner_color, coordinates, PowerType::ACTIVE) , mana_cost_(mana_cost) {}

    /// @brief Getter for mana cost
    /// @return Returns mana cost as size_t.
    size_t getManaCost() override;

    /// @brief Pure virtual method implemented in subclasses.
    /// @param board Pointer to the board
    /// @param opponent Pointer to the opponent player.
    virtual void special(Board* board, Player* opponent) override = 0;
    virtual ~ActiveQueen() = default;
};

class FlipperQueen : public ActiveQueen
{
  public:
    FlipperQueen(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveQueen::ActiveQueen(PieceID::QFLP, owner_color, coordinates, 0) 
    {
      short_name_ += "f";
    }
    
    /// @brief Calculates the cost for activating special power of flipper queen.
    /// @return Returns mana cost, as size_t.
    size_t getManaCost() override;

    /// @brief Executes the Flipper Queen's special: relocates to the target square, capturing any enemy piece
    /// there and collecting items.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;
};

class JumpyQueen : public ActiveQueen
{
  public:
    JumpyQueen(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveQueen::ActiveQueen(PieceID::QJMP, owner_color, coordinates, 2) 
    {
      short_name_ += "j";
    }

    /// @brief Executes the Jumpy Queen's special: relocates to the target square,  capturing any enemy piece 
    /// there and collecting items.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused    
    void special(Board* board, Player* opponent) override;
};
#endif