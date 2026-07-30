#ifndef PAWN_HPP
#define PAWN_HPP

#include "Piece.hpp"
const std::string PAWN_SHORT_NAME = "♟"; 

class Pawn : public Piece
{
  public:
    Pawn(PieceID id, PlayerColor owner_color, Coordinates coordinates, PowerType power_type) : 
      Piece::Piece(PieceType::P, id, owner_color, coordinates, 1, power_type)
    {
      short_name_ = PAWN_SHORT_NAME + (owner_color == PlayerColor::WHITE ? "P" : "p");
    }

    /// @brief Handles pawn promotion when the pawn reaches the opponent's back rank.
    void checkPromotion();

    /// @brief Replaces this pawn with its promotion piece in the owner's piece list.
    virtual void promote();
    
    /// @brief Checks if Piece can move to enteret square
    /// @param board Reference to Board
    /// @param to Coordinates of target square
    /// @param is_capture Flag is piece capturing
    /// @return True if can
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;

    /// @brief Checks if pawn can move forward
    /// @param board Reference to Board
    /// @param piece_file Pawn file
    /// @param piece_rank Pawn rank
    /// @param target_file Target file
    /// @param target_rank Target rank
    /// @return True if can
    bool canMoveForward(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file, 
      std::size_t target_rank);

    /// @brief Checks if pawn can capture
   /// @param board Reference to Board
    /// @param piece_file Pawn file
    /// @param piece_rank Pawn rank
    /// @param target_file Target file
    /// @param target_rank Target rank
    /// @return True if can
    bool canCapture(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file, 
      std::size_t target_rank);
};

class BasicPawn : public Pawn
{
  public:
    BasicPawn(PlayerColor owner_color, Coordinates coordinates) : 
      Pawn::Pawn(PieceID::P, owner_color, coordinates, PowerType::NONE) {}
};

class GoldenPawn : public Pawn
{
  public :
    GoldenPawn(PlayerColor owner_color, Coordinates coordinates) : 
      Pawn::Pawn(PieceID::PGLD, owner_color, coordinates, PowerType::PASSIVE) 
    {
      short_name_ += "g";
    }
};

#endif