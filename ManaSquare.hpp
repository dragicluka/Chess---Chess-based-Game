#ifndef MANASQUARE_HPP
#define MANASQUARE_HPP
#include "Square.hpp"

class ManaSquare : public Square{
    public:
    ManaSquare(Coordinates coordinates) : Square(coordinates) {square_type_ = SquareType::MANA;}

    /// @brief Gives active player an additional mana at each turn if their piece is currently on it.
    /// @param round Represents the number of the current round
    /// @param active_player Pointer to the active player
    /// @return Returns false if there is no piece on mana square and if the sqaure type has been changed due to the 
    /// painter rook f.e., otherwise true. 
    bool triggerEffect(std::size_t round, Player* active_player) override;
};


#endif