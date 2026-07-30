#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Piece.hpp"

class Coordinates;

const std::string QUEEN_SHORT_NAME = "♛";

class Queen : public Piece
{
  public:
    Queen(PieceID id, PlayerColor owner_color, Coordinates coordinates, PowerType power_type) : 
      Piece::Piece(PieceType::Q, id, owner_color, coordinates, 9, power_type) 
    {
      short_name_ = QUEEN_SHORT_NAME + (owner_color == PlayerColor::WHITE ? "Q" : "q");
    }

    /// @brief Checks whether the queen can legally move to the target square.
    /// @param board Reference to the game board.
    /// @param to Coordinates that queen wants to move to.
    /// @param is_capture Unused for the queen, movementt and capture follow the same rules
    /// @return true if the queen can reach the target square, false otherwise.
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;

    /// @brief Checks if queen can move diagonaly
    /// @param board Board refernce
    /// @param to Target cooridinates
    /// @return True if can
    bool checkDiagonaly(Board& board, Coordinates to);

    /// @brief Checks if queen can move verticaly
    /// @param board Board refrence
    /// @param piece_file File of queen
    /// @param piece_rank Rank of queen
    /// @param target_rank Rank of target square
    /// @return True if can
    bool checkVerticaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_rank);

    
    /// @brief Check if queen can mvoe horisontaly
    /// @param board Board refrence
    /// @param piece_file File of queen
    /// @param piece_rank Rank of queen
    /// @param target_file File 
    /// @return True if can
    bool checkHorisontaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file);
};

class BasicQueen : public Queen
{
  public:
    BasicQueen(PlayerColor owner_color, Coordinates coordinates) : 
      Queen::Queen(PieceID::Q, owner_color, coordinates, PowerType::NONE) {}
};

class HungryQueen : public Queen
{
  public :
    HungryQueen(PlayerColor owner_color, Coordinates coordinates) : 
      Queen::Queen(PieceID::QHNGR, owner_color, coordinates, PowerType::PASSIVE) 
    {
      short_name_ += "h";
    }
};

#endif