#ifndef BOOSTSQUARE_HPP
#define BOOSTSQUARE_HPP
#include "Square.hpp"

class BoostSquare : public Square{
    public:
    BoostSquare(Coordinates coordinates) : Square(coordinates) {square_type_ = SquareType::BOOST;}

    /// @brief This method only returns false, it is valid for other special squares.
    /// @param round Current round given as std::size_t
    /// @param active_player Pointer to the active player.
    /// @return Returns false, because its effect is being triggered by movemenent, by either special or move command,
    /// not at the beginning of each round.
    bool triggerEffect(std::size_t round, Player* active_player) override;
};


#endif