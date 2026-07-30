#ifndef ROOK_HPP
#define ROOK_HPP

#include "Piece.hpp"

const std::string ROOK_SHORT_NAME = "♜";

class Rook : public Piece 
{
  public:
    Rook(PieceID id, PlayerColor owner_color, Coordinates coordinates, PowerType power_type) : 
      Piece::Piece(PieceType::R, id, owner_color, coordinates, 5, power_type) 
    {
      short_name_ = ROOK_SHORT_NAME + (owner_color == PlayerColor::WHITE ? "R" : "r");
    }

    /// @brief Checks whether the rook can legally move to the target square.
    /// @param board Reference to the game board.
    /// @param to Coordinates that rook wants to move to.
    /// @param is_capture Unused for the rook, movementt and capture follow the same rules
    /// @return true if the rook can reach the target square, false otherwise.
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;

   /// @brief Checks if rook can move verticaly
    /// @param board Board refrence
    /// @param piece_file File of queen
    /// @param piece_rank Rank of queen
    /// @param target_rank Rank of target square
    /// @return True if can
    bool checkVerticaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_rank);

    /// @brief Check if rook can mvoe horisontaly
    /// @param board Board refrence
    /// @param piece_file File of queen
    /// @param piece_rank Rank of queen
    /// @param target_file File 
    /// @return True if can
    bool checkHorisontaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file);
};

class BasicRook : public Rook
{
  public:
    BasicRook(PlayerColor owner_color, Coordinates coordinates) : 
      Rook::Rook(PieceID::R, owner_color, coordinates, PowerType::NONE) {}
};


#endif