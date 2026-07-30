#ifndef SQUARE_HPP
#define SQUARE_HPP
#include "Coordinates.hpp"
#include "Item.hpp"
#include <optional>
#include <memory>
#include <vector>

class Piece;
class Player;
enum class PlayerColor;

enum class SquareType{
    BLACK,
    WHITE,
    MANA,
    BOOST,
    SPAWN
};

class Square{
    protected:
        Coordinates coordinates_;
        std::optional<Item*> item_;
        std::optional<Piece*> piece_;
        bool under_attack_;
        SquareType square_type_;
    public: 
        Square(Coordinates);
        Square(const Square&) = delete;
        Square& operator=(const Square&) = delete;
        virtual ~Square() = default;

        /// @brief Checks if piece occupies the sqaure.
        /// @return Returns true if it does, otherwise false.
        bool isOccupied();

        /// @brief Getter for a piece on the square.
        /// @return Returns the pointer to the piece. If there is no piece on sqaure, nullptr is returned.
        Piece* getPiece();

        /// @brief Sets the piece on sqaure.
        /// @param piece Pointer to the piece, that will be placed on sqaure.
        void setPiece(Piece* piece);

        /// @brief Removes piece from the square. Sets the piece_ member variable to std::nullopt.
        void removePiece();

        /// @brief Removes item from the square. Sets the item_ member variable to std::nullopt.
        void removeItem() {item_ = std::nullopt;}
        
        /// @brief Getter for an item on the square.
        /// @return Returns the pointer to the item. If there is no item on sqaure, nullptr is returned.
        Item* getItem();

        /// @brief Sets the item on the square.
        /// @param item Pointer to the item, that will be placed on the square.
        void setItem(Item* item) {item_ = item;}

        /// @brief Triggers the square's special effect at the start of a turn. Base version does nothing.
        /// @param round Not used
        /// @param active_player Not used
        /// @return This base version always returns false.
        virtual bool triggerEffect(std::size_t round, Player* active_player);

        /// @brief Prints the square to the board, including its background color, any piece, item, and king status.
        /// @param opponent Pointer to the opponent player
        /// @param active Pointer to the active player
        /// @param board Pointer to the board.
        void print(Player* opponent, Player* active, Board* board);

        /// @brief Getter for coordinates.
        /// @return Returns Coordinates.
        Coordinates getCoordinates(){return coordinates_;}

        /// @brief Checks if the square is in offside, relevent for teleport potion.
        /// @param active_player_furthest_rank The active player piece with the furthest rank
        /// @param player_color color of the active player
        /// @return Returns true if square is in offside, otherwise false.
        bool isInOffside(std::size_t active_player_furthest_rank, PlayerColor player_color);

        /// @brief Getter for sqaure type
        /// @return Returns member variable square_type_
        SquareType getSquareType() const {return square_type_;}

        /// @brief Setter for a square type
        /// @param type Type that will be assigned to member variable square_type_.
        void setSquareType(SquareType type) {square_type_ = type;}
};


#endif