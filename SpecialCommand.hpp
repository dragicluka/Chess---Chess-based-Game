#ifndef SPECIALCOMMAND_HPP
#define SPECIALCOMMAND_HPP

#include "Command.hpp"
#include "Piece.hpp"

class Piece;
class Player;
class Board;
class Coordinates;
class Square;

class SpecialCommand : public Command
{
  public:
    SpecialCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    SpecialCommand(const SpecialCommand&) = delete;
    virtual ~SpecialCommand() = default;

    /// @brief Executes a special command: validates the piece and parameters, then triggers its power.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param opponent_player Reference to the opponent player
    /// @return ACTIVE_SUCCESS on success, WIN/KING_DRAW on a game-ending result, ERROR if invalid.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;

    /// @brief Performs the initial validation common to all special commands.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @return Pointer to the validated piece, or nullptr if validation fails.
    Piece* initialChecks(Player& active_player, Board& board);

    /// @brief Makes piece-specific precondition checks based on the piece's ID.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param opponent_player Reference to the opponent player
    /// @param current_piece Pointer to the piece using the special.
    /// @return true if the piece's precondition check passes, false otherwise.
    bool switchCase(Board& board, Player& active_player, Player& opponent_player, Piece* current_piece);

    /// @brief Validates that the parameter count matches the requirements of the given piece.
    /// @param current_piece_id The ID of the piece using the special.
    /// @return true if the parameter count is correct, false otherwise.
    bool parameterCount(PieceID current_piece_id);

    /// @brief Validates and prepares the Impatient Pawn's promotion target.
    /// @param current_piece Pointer to the Impatient Pawn.
    /// @param player Pointer to the active player.
    /// @param board Pointer to the game board.
    /// @return true if the promotion type is valid, false otherwise.
    bool checkPieceId(Piece* current_piece, Player* player, Board* board);

    /// @brief Validates the Stubborn Pawn's forward capture target.
    /// @param current_piece Pointer to the Stubborn Pawn.
    /// @param player Pointer to the active player.
    /// @param board Pointer to the game board.
    /// @return true if a valid forward capture exists, false otherwise.
    bool checkForwardSquare(Piece* current_piece, Player* player, Board* board);

    /// @brief Validates the Nervous Pawn's backward move target.
    /// @param current_piece Pointer to the Nervous Pawn.
    /// @param player Pointer to the active player.
    /// @param board Pointer to the game board.
    /// @return true if the square behind is empty, false otherwise.
    bool checkBackSquare(Piece* current_piece, Player* player, Board* board);

    /// @brief Validates the Invincible Rook's requested invincibility duration.
    /// @param current_piece Pointer to the Invincible Rook.
    /// @return true if the turn count is valid, false otherwise.
    bool checkTurnCount(Piece* current_piece);

    /// @brief Validates the target square for pieces that move to or capture on a specified square.
    /// @param current_piece Pointer to the piece using the special.
    /// @param player Pointer to the active player.
    /// @param board Pointer to the game board.
    /// @return true if the target square is valid for the piece, false otherwise.
    bool checkTargetSquare(Piece* current_piece, Player* player, Board* board);

    /// @brief Validates the Flipper Queen's bounce and target squares for its diagonal move.
    /// @param current_piece Pointer to the Flipper Queen.
    /// @param player Pointer to the active player.
    /// @param board Pointer to the game board.
    /// @return true if the bounce-and-target path is valid, false otherwise.
    bool checkTargetSquareAndBounceSquare(Piece* current_piece, Player* player, Board* board);

    /// @brief Checks that a diagonal path between two squares is clear, with optional bounce constraints.
    /// @param board Pointer to the game board.
    /// @param target_file File of the destination square.
    /// @param target_rank Rank of the destination square.
    /// @param piece_file File of the starting square.
    /// @param piece_rank Rank of the starting square.
    /// @param bounce Whether this leg starts from the bounce (edge) square.
    /// @return true if the diagonal path is clear and valid, false otherwise.
    bool canMoveDiagonal(Board* board, size_t target_file, size_t target_rank, size_t piece_file, size_t piece_rank, 
    bool bounce);

    /// @brief Transfers the Preacher Bishop's target piece from the opponent to the active player.
    /// @param current_piece Pointer to the Preacher Bishop.
    /// @param active_player Pointer to the player gaining the piece.
    /// @param opponent_player Pointer to the player losing the piece.
    void changeOwner(Piece* current_piece, Player* active_player, Player* opponent_player);

    /// @brief Checks whether a move between two squares follows a knight's L-shaped pattern.
    /// @param piece_file File of the starting square.
    /// @param piece_rank Rank of the starting square.
    /// @param target_file File of the destination square.
    /// @param target_rank Rank of the destination square.
    /// @return true if the move is a valid knight jump, false otherwise.
    bool checkKnightMove(size_t piece_file, size_t piece_rank, size_t target_file, size_t target_rank);

    /// @brief Validates the Archer King's target along its file (the arrow's flight path).
    /// @param current_piece Pointer to the Archer King.
    /// @param player Pointer to the active player.
    /// @param target_coordinates The square being targeted.
    /// @param board Pointer to the game board.
    /// @return true if the target is a valid archer shot, false otherwise.
    bool checkPath(Piece* current_piece, Player* player, Coordinates target_coordinates, Board* board);

    /// @brief Validates the Color Blind Bishop's one-square diagonal/adjacent move.
    /// @param current_piece Pointer to the Color Blind Bishop.
    /// @param target_square The square being moved to.
    /// @param target_file File of the target square.
    /// @param target_rank Rank of the target square.
    /// @param current_file File of the bishop's current square.
    /// @param current_rank Rank of the bishop's current square.
    /// @return true if the adjacent move is valid, false otherwise.
    bool colorBlindBishop(Piece* current_piece, Square* target_square, size_t target_file, size_t target_rank, 
    size_t current_file, size_t current_rank);

    /// @brief Validates the Explosive Pawn's capture target.
    /// @param current_piece Pointer to the Explosive Pawn.
    /// @param board Pointer to the game board.
    /// @param target_square The square being captured on.
    /// @param target_coordinates Coordinates of the target square.
    /// @return true if the capture target is valid, false otherwise.
    bool explosivePawn(Piece* current_piece, Board* board, Square* target_square, Coordinates target_coordinates);

    /// @brief Validates the Jumpy Queen's knight-style move.
    /// @param current_piece Pointer to the Jumpy Queen.
    /// @param target_square The square being moved to.
    /// @param target_file File of the target square.
    /// @param target_rank Rank of the target square.
    /// @param current_file File of the queen's current square.
    /// @param current_rank Rank of the queen's current square.
    /// @return true if the knight-style move is valid, false otherwise.
    bool jumpyQueen(Piece* current_piece, Square* target_square, size_t target_file, size_t target_rank, 
    size_t current_file, size_t current_rank);
};

#endif