#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <memory>
#include <string>

class Piece;
class Board;

enum class PlayerColor
{
  WHITE, BLACK
};

class Player
{
  private:
    PlayerColor color_;
    std::size_t mana_;
    std::size_t mana_pool_size_;
    std::size_t elo_;
    std::vector<std::unique_ptr<Piece>> pieces_;
    std::vector<std::unique_ptr<Piece>> prison_pieces_;
    std::string id_;
    std::size_t number_of_moves_;
    bool has_won_;
    bool is_draw_;
    //from Claude
    //begin
    bool can_move_multiple_;
    Piece* last_moved_piece_;
    //end
  public:
    Player(PlayerColor color, size_t mana, size_t elo, std::size_t mana_pool_size);
    Player(const Player& other) = delete;
    virtual ~Player();

    /// @brief Goes through all pieces in order to check if the king is in check.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns true if the king is in check, otherwise false.
    bool checkForCheck(Player* opponent, Board* board);

    /// @brief Goes through all pieces in order to check if the king is in stalemate.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns true if the king is in stalemate, otherwise false.
    bool checkForStalemate(Player* opponent, Board* board);

    /// @brief Goes through all pieces in order to check if the king is in checkmate.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns true if the king is in checkmate, otherwise false.
    bool checkForCheckmate(Player* opponent, Board* board);

    /// @brief This function goes through opponents's prison pieces and checks if one of them is the king
    /// @param opponent Pointer to the opponent player
    /// @return Returns true if king of active player is in prison, otherwise false.
    bool automaticWin(Player* opponent);

    /// @brief This function goes through opponents's prison pieces and active player's prison pieces and checks if
    /// both of them have king.
    /// @param opponent Pointer to the opponent player
    /// @return Returns true if king if both players have king in their prisons, otherwise false.
    bool automaticDraw(Player* opponent);

    /// @brief Adds piece to the prison.
    void addToPrison();

    /// @brief Moves a captured friendly piece into the opponent's prison.
    /// @param piece Pointer to the piece being captured.
    /// @param opponent_player Pointer to the opponent player.
    void addToMyPrison(Piece* piece, Player* opponent_player);

    /// @brief Prints the player's prison pieces, sorted by value and grouped by type.
    void printPrison();

    /// @brief Setter for mana
    /// @param mana Mana value to be set
    void setMana(std::size_t mana) {mana_ = mana;}

    /// @brief Sets the mana pool size read from game config file
    /// @param mana_pool_size Value from game config file.
    void setManaPoolSize(std::size_t mana_pool_size) {mana_pool_size_ = mana_pool_size;}

    /// @brief Setter for elo
    /// @param elo Value to be assigned to member variable elo_.
    void setElo(std::size_t elo) {elo_ = elo;}

    /// @brief Getter for elo
    /// @return Returns the value of elo_.
    std::size_t getElo() const {return elo_;}

    /// @brief Setter for pieces_ member variable
    /// @param pieces Pieces read from game config file.
    void setPieces(std::vector<std::unique_ptr<Piece>> pieces);

    /// @brief Getter for player id (either black or white).
    /// @return Returns the value of id_.
    std::string getID() const {return id_;} 

    /// @brief Getter for mana.
    /// @return Returns mana_, as std::size_t
    std::size_t getMana() const {return mana_;}

    /// @brief Getter for mana pool size
    /// @return Returns mana_pool_size_, as std::size_t
    std::size_t getManaPoolSize() const {return mana_pool_size_;}

    /// @brief Getter for player's pieces.
    /// @return Reference to vector of unique pointers
    std::vector<std::unique_ptr<Piece>>& getPieces(){return pieces_;}

    /// @brief Getter for player's prison pieces.
    /// @return Reference to vector of unique pointers
    std::vector<std::unique_ptr<Piece>>& getPrisonPieces(){return prison_pieces_;}

    /// @brief Getter for player color
    /// @return Returns color_, as PlayerColor.
    PlayerColor getColor() const {return color_;}

    /// @brief Getter for number of moves made so far during the current turn.
    /// @return number_of_moves_, as std::size_t
    std::size_t getNumberOfMoves() const {return number_of_moves_;}

    /// @brief Setter for number_of_moves_.
    /// @param number_of_moves New value for member variable
    void setNumberOfMoves(std::size_t number_of_moves) {number_of_moves_ = number_of_moves;}

    /// @brief Goes through the ranks of all pieces, and returns the furthest rank, which is highest for white player,
    /// and lowest for black player.
    /// @return value of the furthest rank, as std::size_t
    std::size_t getFurthestRank();

    /// @brief Returns the actual score, needed for the calculation of new elo values.
    /// @return In case of draw, 0.5 is returned, in case of win 1.0, and in case of loss 0.0.
    double getActualScore();

    /// @brief In case of win it sets has_won_to true, which is later needed for the calculation of new elo values.
    void setWon() {has_won_ = true;}

    /// @brief In case of draw it sets has_draw_ to true, which is later needed for the calculation of new elo values.
    void setDraw() {is_draw_ = true;}

    /// @brief Getter for can_move_multiple_ variable.
    /// @return Returns can_move_multiple_.
    bool getCanMoveMultiple() const {return can_move_multiple_;}

    /// @brief Getter for last_moved_piece_. Used in cases when piece steps on boost square, gets addiotional move...
    /// @return Returns the last_moved_piece_ as Piece pointer.
    Piece* getLastMovedPiece() const {return last_moved_piece_;}

    /// @brief Setter fro can_move_multiple_
    /// @param can_move Param that dtermines if the piece can move multiple times, or not
    void setCanMoveMultiple(bool can_move) {can_move_multiple_ = can_move;}

    /// @brief Setter for last_moved_piece_.
    /// @param last_moved_piece PIece that moved last.
    void setLastMovedPiece(Piece* last_moved_piece) {last_moved_piece_ = last_moved_piece;}
};
#endif