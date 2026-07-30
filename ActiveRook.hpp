#ifndef ACTIVEROOK_HPP
#define ACTIVEROOK_HPP

#include "Rook.hpp"

class ActiveRook : public Rook
{
  protected:
    size_t mana_cost_;
  public:
    ActiveRook(PieceID id,PlayerColor owner_color, Coordinates coordinates, size_t mana_cost) : 
      Rook::Rook(id, owner_color, coordinates, PowerType::ACTIVE) , mana_cost_(mana_cost) {}

    /// @brief Getter for mana cost
    /// @return Returns mana cost as size_t.
    size_t getManaCost() override;

    /// @brief Pure virtual method implemented in subclasses.
    /// @param board Pointer to the board
    /// @param opponent Pointer to the opponent player.
    virtual void special(Board* board, Player* opponent) override = 0;
    virtual ~ActiveRook() = default;

};

class InvincibleRook : public ActiveRook
{
  public:
    InvincibleRook(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveRook::ActiveRook(PieceID::RINV, owner_color, coordinates, 0) 
    {
      short_name_ += "i";
    }

    /// @brief Calculates the cost for activating special power of invincible rook
    /// @return Returns mana cost, as size_t.
    size_t getManaCost() override;

    /// @brief Executes the Invincible Rook's special, making it temporarily unable to be captured.
    /// @param board Pointer to the game board, unused
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;
};

class PainterRook : public ActiveRook
{
  public:
    PainterRook(PlayerColor owner_color, Coordinates coordinates) : 
      ActiveRook::ActiveRook(PieceID::RPNT, owner_color, coordinates, 0) 
    {
      short_name_ += "p";
    }

    /// @brief Calculates the mana cost for the Painter Rook's special based on travel distance
    /// @return Returns mana cost, as size_t.
    size_t getManaCost() override;

    /// @brief Executes the Painter Rook's special: paints all squares along its path to its inherent color, 
    /// moves to the target square, capturing piece there and collecting its item.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;
};

#endif
