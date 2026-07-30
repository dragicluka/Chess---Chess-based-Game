#ifndef PIECE_HPP
#define PIECE_HPP

#include "memory"
#include <string>
#include "Player.hpp"
#include "Coordinates.hpp"
#include "Item.hpp"
#include "Square.hpp"

class Player;
class Board;

enum class PieceType
{
  Q, K, R, P, N, B
};

enum class PieceID
{
  P, PGLD, PIPT, PSTB, PNRV, PEXP,
  R, RINV, RPNT,
  N, NJMP, NICE,
  B, BCLR, BPRC,
  Q, QFLP, QJMP, QHNGR,
  K, KFRT, KARC
};

enum class PowerType
{
  PASSIVE, ACTIVE, NONE
};

class Piece
{
  protected:
    PieceType type_;
    PieceID id_;
    PlayerColor owner_color_;
    Player* owner_;
    Coordinates coordinates_; 
    size_t value_;
    PowerType power_;
    std::string short_name_;
    std::unique_ptr<Item> item_;
    bool is_frozen_;
    std::unique_ptr<Piece> target_piece_;
    Square* target_square_;
    Square* bounce_square_;
    bool is_in_prison_;
    int initially_invincible_for_;
    int invincible_for_;
    bool is_even_odd_triggered_;
    std::string even_or_odd_;
    SquareType inherent_color_;
    bool has_moved_;
    int frozen_for_;
    Piece* castling_rook_;
    std::vector<Coordinates> checked_path_;
    bool pawn_moved_two_squares;
    bool moving_en_passant_;
    bool is_cloned_;
    bool was_cloned_;
    int cloned_for_;
  public:
    Piece(PieceType type, PieceID id, PlayerColor owner_color, Coordinates coordinates, size_t value, PowerType power);
    Piece(const Piece&) = delete;
    Piece& operator=(const Piece&) = delete; 

    /// @brief Getter for owner_color_
    /// @return Returns owner_color_, as PlayerColor
    PlayerColor getColor() const {return owner_color_;}

    /// @brief Getter for an item
    /// @return Returns a pointer to the item
    Item* getItem() const {return item_.get();}

    /// @brief Setter for an item.
    /// @param item New item to be set
    void setItem(Item* item) {item_ = item->clone();}

    /// @brief Getter for piece's short name
    /// @return Returns shorrt_name_, as std::string.
    std::string getShortName() const {return short_name_;}

    /// @brief Virtual method, overriden in subclasses.
    /// @param board Pointer to the board, unused
    /// @param opponent Pointer to the opponent, unused.
    virtual void special(Board* board, Player* opponent) {(void) board; (void) opponent;}

    virtual ~Piece() = default;

    /// @brief Getter for coordinates
    /// @return Returns reference to coordinates_.
    Coordinates& getCoordinates() {return coordinates_;}

    /// @brief Getter for id_
    /// @return Returns id_, as PieceID.
    PieceID getId() const {return id_;}

    /// @brief Getter for IdString
    /// @param id PieceID that determines which value will be returned
    /// @return Returns id, as std::string
    std::string getIdString(PieceID id) const;

    /// @brief Getter for type_
    /// @return Returns type_, as PieceType
    PieceType getPieceType() const {return type_;}

    /// @brief Getter for value of piece
    /// @return Returns value_, as size_t
    size_t getValue() const {return value_;}

    /// @brief Virtual method that is overriden by King, by all other pieces it is same.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns always false
    virtual bool isInCheckMate(Player* opponent, Board* board) {(void) opponent; (void) board; return false;}

    /// @brief Virtual method that is overriden by King, by all other pieces it is same.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns always false
    virtual bool isInStalemate(Player* opponent, Board* board) {(void) opponent; (void) board; return false;}

    /// @brief Virtual method that is overriden by King, by all other pieces it is same.
    /// @param opponent Pointer to the opponent player
    /// @param board Pointer to the board
    /// @return Returns always false
    virtual bool isInCheck(Player* opponent, Board* board) {(void) opponent; (void) board; return false;}

    /// @brief Virtual method, overriden by some special pieces. Returns mana cost for activating special power
    /// @return Here it returns 0.
    virtual std::size_t getManaCost() {return 0;}

    /// @brief Getter for power_
    /// @return Returns power_, as PowerType
    PowerType getPowerType() const {return power_;}

    /// @brief Setter for owner_
    /// @param player Player either white or black that now owns this piece
    void setOwner(Player* player) {owner_ = player;}

    /// @brief Getter for owner_
    /// @return Returns pointer to Player
    Player* getOwner() const {return owner_;}

    /// @brief Setter for piece that this piece targets
    /// @param piece Piece that is being targeted by this piece
    void setTargetPiece(std::unique_ptr<Piece> piece) {target_piece_ = std::move(piece);}

    /// @brief Setter for square that this piece targets
    /// @param square Square that is being targeted by this piece
    void setTargetSquare(Square* square) {target_square_ = square;}

    /// @brief Setter for bounce square, relevant for flipper queen.
    /// @param square Squaare to  be set as bounce_square_
    void setBounceSquare(Square* square) {bounce_square_ = square;}

    /// @brief Getter for target square_
    /// @return Returns pointer to target_square_
    Square* getTargetSquare() const {return target_square_;}

    /// @brief Pure virtual method implemented in subclasses
    /// @param board Reference to the game board.
    /// @param to Coordinates that piece wants to move to.
    /// @param is_capture Relevant for pawn, becauuse the movement pattern changes when capturing
    /// @return Returns true if the move is possible, false otherwise
    virtual bool canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture) = 0;

    /// @brief Getter for is_in_prison_
    /// @return Returns true if piece is in prison, false otherwise
    bool isInPrison() const {return is_in_prison_;}

    /// @brief Sets is_in_prison_ to true, which marks that piece is now in prison
    void sendToPrison() { is_in_prison_ = true;}

    /// @brief Getter for is_frozen_
    /// @return Returns true if piece is frozen, otherwise false.
    bool isFrozen() const {return is_frozen_;}

    /// @brief Setter for initially_invincible_for_ and invincible_for_
    /// @param turn_count Current turn_count that determines the value of the member variables, 
    /// initially_invincible_for_ and invincible_for_
    void setInitialInvincible(int turn_count);

    /// @brief Decrements the invincible_for_ for 1, if it is bigger than 0
    /// @return Returns true if decrementation  was possible, otherwise false
    bool takeInvinicible();

    /// @brief Getter for invincible_for_
    /// @return Returns invincible_for_ as int
    int getInvincible() const {return invincible_for_;}

    /// @brief Returns a type of piece in form of string based on member variable type_
    /// @return Returns string
    std::string getTypeString() const;

    /// @brief Setter for coordinates_
    /// @param coordinates New value of coordinates_
    void setCoordinates(Coordinates coordinates) {coordinates_ = coordinates;}

    /// @brief Function that determins if the piece can move after the the even odd potion effect was applied
    /// @param even_or_odd Strings that's either even or odd
    /// @param round Current round
    /// @return Returns true if piece can move, false otherwise
    bool canMove(const std::string& even_or_odd, std::size_t& round);

    /// @brief Sets is_even_odd_triggered_ to true and assigns the member variable even_or_odd_ the value of the 
    /// parameter even_or_odd
    /// @param even_or_odd New value of even_or_odd_.
    void setEvenOdd(const std::string& even_or_odd);

    /// @brief Checks if even odd potion effect is triggered
    /// @param round Current round
    /// @return Returns true if the effect is triggered, otherwise false
    bool isEvenOddTriggered(std::size_t round);

    /// @brief Setter for inherent_color_
    /// @param color New value of inherent_color_
    void setColor(SquareType color){inherent_color_ = color;};

    /// @brief Setter for owner_color_
    /// @param color New value of owner_color_
    void setOwnerColor(PlayerColor color) {owner_color_ = color;}

    /// @brief Creates a new piece and sets a new short name
    /// @param id Id of the new piece
    /// @param color Color of the new piece, in order to determine ownership
    /// @param coordinates Coordinates of the new piece
    void setNewShortName(std::string id, PlayerColor color, Coordinates coordinates);

    /// @brief Removes the item from piece's inventory
    void removeItem() {item_.reset();}

    /// @brief Checks if piece has certain item
    /// @param item_id Id of the item used for check
    /// @return Returns true if piece has the item of the same id as item_id
    bool hasItem(const std::string& item_id);

    /// @brief Getter for inherent color
    /// @return Returns inherent_color, as SquareType
    SquareType getInherentColor() const {return inherent_color_;}

    /// @brief Getter for has_moved_
    /// @return Returns has_moved_
    bool getHasMoved() { return has_moved_; }

    /// @brief Setter for has_moved_
    /// @param has_moved New value of has_moved_
    void setHasMoved(bool has_moved) { has_moved_ = has_moved; }

    /// @brief Applies freeze effect
    void setFrozen() {is_frozen_ = true; frozen_for_ = 2;}

    /// @brief Checks if frozen_for_ is bigger than 0, if it is it decremnts it for 1, checks if frozen_for is now 0
    /// and if it is sets is_frozen_ to false
    void unfreeze();

    /// @brief Setter for castling_rook_
    /// @param rook New value of castling_rook_
    void setCastlingRook(Piece* rook) {castling_rook_ = rook;}

    /// @brief Getter for castling_rook_
    /// @return Returns piece pointer
    Piece* getCaslintgRook() const {return castling_rook_;}

    /// @brief Getter for checked_path_
    /// @return Returns vector of coordinates
    std::vector<Coordinates> getCheckedPath() { return checked_path_;}

    /// @brief Getter for pawn_moved_two_squares
    /// @return Returns pawn_moved_two_squares as bool
    bool getPawnMovedTwoSquares() const {return pawn_moved_two_squares;}

    /// @brief Setter for pawn_moved_two_squares
    /// @param moved New value of pawn_moved_two_squares
    void setPawnMovedTwoSqures(bool moved) {pawn_moved_two_squares = moved;}

    /// @brief Getter for moving_en_passant_
    /// @return Returns moving_en_passant_ as bool
    bool getMovingEnPassant() const {return moving_en_passant_;}

    /// @brief Setter for moving_en_passant_
    /// @param moving New value of moving_en_passant_
    void setMovingEnPassant(bool moving) {moving_en_passant_ = moving;}

    void setCloned();

    void unclone();

    bool getIsCloned() const {return is_cloned_;}

    bool getWasCloned() const {return was_cloned_;}

    int getClonedFor() const {return cloned_for_;}

};


#endif
