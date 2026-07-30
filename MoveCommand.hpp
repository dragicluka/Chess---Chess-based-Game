#ifndef MOVECOMMAND_HPP
#define MOVECOMMAND_HPP

#include "Command.hpp"
#include "Piece.hpp"

class MoveCommand : public Command
{
  private:
    bool move_valid_;
    bool is_pawn_move_;
    bool is_capture_;
    bool is_promotion_;
    char pawn_file_;
    std::string piece_type_;
    std::string target_square_;
    std::string promotion_piece_;
    bool blocked_;
    Piece* en_passant_pawn_;
  public:
    MoveCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages), move_valid_(true),
    is_pawn_move_(false), is_capture_(false), is_promotion_(false), pawn_file_('\0'), piece_type_(""),
     target_square_(""), promotion_piece_(""), blocked_(false), en_passant_pawn_(nullptr) {}
    MoveCommand(const MoveCommand&) = delete;
    virtual ~MoveCommand() = default;

    /// @brief Executes a move command: validates input, resolves the piece, and performs the move.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param opponent_player Reference to the opponent player
    /// @return ACTIVE_SUCCESS if the move is completed, WIN on a winning move, otherwise ERROR.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;

    /// @brief Finds all of the active player's pieces that could legally make the requested move.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param target The destination coordinates of the move.
    /// @param type The piece type string to match for example, "p", "r", "q".
    /// @param is_capture Boolean that determines if the move is capture attempt or not.
    /// @param pawn_file Source file for a disambiguated pawn capture, or '\0' if unspecified.
    /// @return A vector of pieces that could legally perform the move.
    std::vector<Piece*> getPotentialPiecesMove(Board& board, Player& active_player, Coordinates target,
    std::string type, bool is_capture, char pawn_file);

    /// @brief Resolves an ambiguous move by prompting the player to choose which piece to move
    /// @param potential_pieces The candidate pieces that can all legally make the move
    /// @param active_player Reference to the active player
    /// @param piece  Output reference set to the piece the player selects
    /// @return ACTIVE_SUCCESS if a valid piece is chosen, QUIT on "quit", or ERROR
    CommandReturnType ambiguousMove(const std::vector<Piece*>& potential_pieces, Player& active_player, Piece*& piece);

    /// @brief Promotes a pawn to a new piece of the chosen type at the target square.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param piece Pointer to the pawn being promoted.
    /// @param target_coordinates The coordinates of square where promotion occurs.
    void promotion(Board& board, Player& active_player, Piece* piece, const Coordinates& target_coordinates);

    /// @brief Handles capturing of a piece during a move
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param opponent_player Reference to the opponent player
    /// @param piece Pointer to the attacking piece.
    /// @param target_square The square where the piece to be captured is at.
    /// @return ACTIVE_SUCCESS if the capture succeeds (or is blocked by a shield), ERROR if invalid.
    CommandReturnType capture(Board& board, Player& active_player, Player& opponent_player, Piece* piece, 
    Square* target_square);

    /// @brief Performs castling by validating the king's path and moving the rook accordingly.
    /// @param board Reference to the board
    /// @param opponent_player Reference to the opponent player
    /// @param piece Pointer to the castling king.
    /// @param target_square The king's destination square.
    /// @return ACTIVE_SUCCESS if castling is legal and completed, ERROR otherwise.
    CommandReturnType castling(Board& board, Player& opponent_player, Piece* piece, Square* target_square);

    /// @brief Parses a move string ("e4", "Nf3", "exd5", "e8=Q") and sets the command's member variables
    /// @param s The move string to parse.
    void parseMove(const std::string& s);

    /// @brief Checks if the entered square is valid
    /// @param s Coordinates of the square that user entered
    /// @return Returns true if the coordinates are valid, false otherwise.
    bool isValidSquare(const std::string& s) const;

    /// @brief Checks whether a character represents a valid (non-pawn) piece type.
    /// @param c The character to check.
    /// @return true if c is one of 'r', 'n', 'b', 'q', or 'k', false otherwise.
    bool isValidPieceType(char c) const;

    /// @brief Checks whether a character represents a valid promotion piece type
    /// @param c The character to check.
    /// @return true if c is one of 'r', 'n', 'b', 'q', false otherwise
    bool isValidPromotionPiece(char c) const;

    /// @brief Parses non capture move 
    /// @param main parameter string 
    /// @return true if valid
    bool parseNonCapture(const std::string& main);
    CommandReturnType valdateAndParse();

    /// @brief Identifies the piece that will make the move and applies pre-move checks.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param target_coordinates The destination coordinates of the move.
    /// @param piece Output reference set to the resolved piece.
    /// @return ACTIVE_SUCCESS if a valid piece is resolved, QUIT if user entered quit while choosing between pieces 
    /// beacuse of ambiguity, ERROR otherwise.
    CommandReturnType findPiece(Board& board, Player& active_player, Coordinates target_coordinates, Piece*& piece);

    /// @brief Identifies the pawn to be captured via en passant
    /// @param board Reference to the board
    /// @param active_player Reference to the active player 
    /// @param target_coordinates 
    /// @param piece Pointer to the pawn performing the en passant capture.
    void handleEnPassant(Board& board, Player& active_player, Coordinates target_coordinates, Piece* piece);

    /// @brief Validates the target square and resolves a capture, then checks for special outcomes.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param opponent_player Reference to the opponent player
    /// @param piece Pointer to the piece making the move.
    /// @param target_square The square where piece is being moved to.
    /// @param is_castling Boolean that checks whether the current move is a castling move.
    /// @return ACTIVE_SUCCESS if the move is valid, WIN on checkmate, or ERROR if invalid.
    CommandReturnType checkTargetSquare(Board& board, Player& active_player, Player& opponent_player, Piece* piece,
    Square* target_square, bool is_castling);

    /// @brief Performs the move of a piece and applies all square effects, if there are an.
    /// @param board Reference to the board
    /// @param active_player Reference to the active player
    /// @param opponent_player Reference to the opponent player
    /// @param piece Pointer to the piece being moved.
    /// @param target_square The square the piece moves to.
    /// @return ACTIVE_SUCCESS once the move and all effects are applied. 
    CommandReturnType handleMove(Board& board, Player& active_player,Player& opponent_player, Piece* piece,
    Square* target_square);
};

#endif