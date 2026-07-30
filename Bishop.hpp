#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "Piece.hpp"

const std::string BISHOP_SHORT_NAME = "♝";

class Bishop : public Piece
{
  public:
    Bishop(PieceID id, PlayerColor owner_color, Coordinates coordinates, PowerType power_type) : 
      Piece::Piece(PieceType::B, id, owner_color, coordinates, 3, power_type) 
    {
      short_name_ = BISHOP_SHORT_NAME + (owner_color == PlayerColor::WHITE ? "B" : "b");
    }

    /// @brief Checks if piece can go to target square
    /// @param board Reference to Board
    /// @param to Target Coordinates
    /// @param is_capture Flag for capturing
    /// @return True if can
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;

    /// @brief Check if square is valid to move to
    /// @param board Reference to Board
    /// @param to Target Coordinates
    /// @param is_capture Flag for capturing
    /// @return True if valid
    bool checkSquare(Board& board, std::size_t file, std::size_t rank);

    /// @brief Check diagonal movement
    /// @param board Reference to board
    /// @param to Target Coordinates
    /// @return True if valid
    bool checkDiagonaly(Board& board, Coordinates to);

    /// @brief Checks if square color is valid to move
    /// @param board Reference to board
    /// @param to Target Coordinates
    /// @return True if valid
    bool checkColor(Board& board, Coordinates to);
};

class BasicBishop : public Bishop
{
  public:
    BasicBishop(PlayerColor owner_color, Coordinates coordinates) : 
      Bishop::Bishop(PieceID::B, owner_color, coordinates, PowerType::NONE) {}
};

#endif