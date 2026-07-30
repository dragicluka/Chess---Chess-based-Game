#ifndef ACTIVEPAWN_HPP
#define ACTIVEPAWN_HPP

#include "Pawn.hpp"

class Square;
class Piece;

class ActivePawn : public Pawn 
{
  protected:
    size_t mana_cost_;
  public:
    ActivePawn(PieceID id,PlayerColor owner_color, Coordinates coordinates, size_t mana_cost) : 
      Pawn::Pawn(id, owner_color, coordinates, PowerType::ACTIVE) , mana_cost_(mana_cost) {}

    /// @brief Getter for mana cost
    /// @return Returns mana cost as size_t.
    size_t getManaCost() override;

    /// @brief Pure virtual method implemented in subclasses.
    /// @param board Pointer to the board
    /// @param opponent Pointer to the opponent player.
    virtual void special(Board* board, Player* opponent) override = 0;
    virtual ~ActivePawn() = default;

};

class ImpatientPawn : public ActivePawn
{
  public:
    ImpatientPawn(PlayerColor owner_color, Coordinates coordinates) : 
      ActivePawn::ActivePawn(PieceID::PIPT, owner_color, coordinates, 0) 
    {
      short_name_ += "i";
    }

    /// @brief Calculates the cost for activating special power of impatient pawn
    /// @return Returns mana cost, as size_t.
    size_t getManaCost() override;

    /// @brief Promotes the pawn before it reaches the opponents back rank.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;
};

class StubbornPawn : public ActivePawn
{
  public:
    StubbornPawn(PlayerColor owner_color, Coordinates coordinates) : 
      ActivePawn::ActivePawn(PieceID::PSTB, owner_color, coordinates, 5) 
    {
      short_name_ += "+";
    }

    /// @brief Enables pawn to capture directly forward.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;
};

class NervousPawn : public ActivePawn
{
  public:
    NervousPawn(PlayerColor owner_color, Coordinates coordinates) : 
      ActivePawn::ActivePawn(PieceID::PNRV, owner_color, coordinates, 1) 
    {
      short_name_ += "-";
    }

    /// @brief Executes the Nervous Pawn's special: moves one square backward.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent, unused
    void special(Board* board, Player* opponent) override;

};

class ExplosivePawn : public ActivePawn
{
  public:
    ExplosivePawn(PlayerColor owner_color, Coordinates coordinates) : 
      ActivePawn::ActivePawn(PieceID::PEXP, owner_color, coordinates, 3) 
    {
      short_name_ += "!";
    }

    /// @brief Executes the Explosive Pawn's special: captures the target and detonates, capturing all non-pawn pieces
    /// in the 8 surrounding squares, then self-destructs.
    /// @param board Pointer to the game board
    /// @param opponent Pointer to the opponent   
    void special(Board* board, Player* opponent) override;

    /// @brief Resolves the explosion's effect on a single square.
    /// @param to_be_checked The square being affected by the explosion.
    /// @param opponent Pointer to the opponent player.
    /// @param board Pointer to the game board.
    void handlePieces(Square* to_be_checked, Player* opponent, Board* board);
};

#endif
