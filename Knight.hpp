#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

const std::string KNIGHT_SHORT_NAME = "♞";

class Knight : public Piece
{
  public:
    Knight(PieceID id, PlayerColor owner_color, Coordinates coordinates, PowerType power_type) : 
      Piece::Piece(PieceType::N, id, owner_color, coordinates, 3, power_type) 
    {
      short_name_ = KNIGHT_SHORT_NAME + (owner_color == PlayerColor::WHITE ? "N" : "n");
    }

    /// @brief Checks whether the knight can legally move to the target square.
    /// @param board Reference to the game board, unused here
    /// @param to Coordinates that knight wants to move to.
    /// @param is_capture Unused for the knight, movementt and capture follow the same rules
    /// @return true if the knight can reach the target square, false otherwise.
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;

};

class BasicKnight : public Knight
{
  public:
    BasicKnight(PlayerColor owner_color, Coordinates coordinates) : 
      Knight::Knight(PieceID::N, owner_color, coordinates, PowerType::NONE) {}
};

class JumpyKnight : public Knight
{
  public:
    JumpyKnight(PlayerColor owner_color, Coordinates coordinates) :
      Knight::Knight(PieceID::NJMP, owner_color, coordinates, PowerType::PASSIVE)
    {
      short_name_ += "j";
    }

    /// @brief Checks whether the jumpy knight can legally move to the target square.
    /// @param board Reference to the game board, unused here
    /// @param to Coordinates that jumpy knight wants to move to.
    /// @param is_capture Unused for the jumpy knight, movementt and capture follow the same rules
    /// @return true if the jumpy knight can reach the target square, false otherwise.
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;
};

class IceKnight : public Knight
{
  public:
    IceKnight(PlayerColor owner_color, Coordinates coordinates) :
      Knight::Knight(PieceID::NICE, owner_color, coordinates, PowerType::PASSIVE)
    {
      short_name_ += "i";
    }

    /// @brief Executes the Ice Knight's passive special: freezes the pieces on two squares it jumps over.
    /// @param board Pointer to the board
    /// @param opponent Pointer to the opponent player, unused here.
    void special(Board* board, Player* opponent) override;

    /// @brief Frezess pieces along the path
    /// @param squares Array of references to all squares
    /// @param rank1 Rank of first square to be frozen
    /// @param file1 File of first square to be frozen
    /// @param rank2 Rank of second square to be frozen
    /// @param file2 File of second square to be frozen
    void freezePieces(const std::array<std::array<std::unique_ptr<Square>,8>,8>& squares,
      std::size_t rank1, std::size_t file1, std::size_t rank2, std::size_t file2);
};

#endif
