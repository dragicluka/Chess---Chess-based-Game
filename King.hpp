#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

class Coordinates;

const std::string KING_SHORT_NAME = "♚";

class King : public Piece
{
  public:
    King(PieceID id, PlayerColor owner_color, Coordinates coordinates, PowerType power_type) : 
      Piece::Piece(PieceType::K, id, owner_color, coordinates, 0, power_type) 
    {
      short_name_ = KING_SHORT_NAME + (owner_color == PlayerColor::WHITE ? "K" : "k");
    }

    /// @brief Checks if the king is in checkmate.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns true if king is in checkmate, otherwise false.
    bool isInCheckMate(Player* opponent, Board* board) override;

    /// @brief Checks if the king is in stalemate.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns true if king is in stalemate, otherwise false.
    bool isInStalemate(Player* opponent, Board* board) override;

    /// @brief Checks if the king is in check.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns true if king is in check, otherwise false.
    bool isInCheck(Player* opponent, Board* board) override;

    /// @brief Checks whether the king can legally move to the target square.
    /// @param board Reference to the game board, unused here
    /// @param to Coordinates that king wants to move to.
    /// @param is_capture Unused for the king, movementt and capture follow the same rules
    /// @return true if the king can reach the target square, false otherwise.
    bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) override;

    /// @brief Checks whether the king's friendly pieces can defend against the piece(s) putting it in check.
    /// @param opponent POinter to the opponent player
    /// @param board Pointer to the board.
    /// @return Returns true if check can be defended, otherwise false.
    bool checkForDefense(Player* opponent, Board* board);

    /// @brief Checks whether any friendly piece can block the path of a piece that puts the king in check.
    /// @param board Pointer to the board.
    /// @param piece Pointer to the attacking piece
    /// @return Returns true if the path can be blocked, otherwise false.
    bool blockThePath(Board* board, Piece* piece);

    /// @brief Tests whether moving the king to an adjacent square would leave it in check.
    /// @param board Pointer to the board
    /// @param original The king's original coordinates, used to restore its positionn.
    /// @param target_square POtential ending square for the king.
    /// @param opponent Pointer to the opponent player
    /// @return Returns true if king would still be in check, otherwise false.
    bool checkKingsSurrondings(Board* board, Coordinates original, Square* target_square, Player* opponent);

    /// @brief Simulates moving an arbitrary piece to a target square and reports if the king stays in check.
    /// @param board Pointer to the board.
    /// @param piece_original_coordinates The piece's original coordinates, used to restore its position.
    /// @param target Target coordinates to move the piece to.
    /// @param target_square The square corresponding to the target coordinates
    /// @param opponent Pointer to the opponent
    /// @param piece Pointer to the piece being moved
    /// @return true if the king would still be in check after the move, false otherwise.
    bool simulateAMove(Board* board, Coordinates piece_original_coordinates, Coordinates target,
    Square* target_square, Player* opponent, Piece* piece);
};

class BasicKing : public King
{
  public:
    BasicKing(PlayerColor owner_color, Coordinates coordinates) : 
      King::King(PieceID::K, owner_color, coordinates, PowerType::NONE) {}
};

class FrightenedKing : public King
{
  public :
    FrightenedKing(PlayerColor owner_color, Coordinates coordinates) : 
      King::King(PieceID::KFRT, owner_color, coordinates, PowerType::PASSIVE) 
    {
      short_name_ += "f";
    }
};

#endif