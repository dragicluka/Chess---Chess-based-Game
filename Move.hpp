#ifndef MOVE_HPP
#define MOVE_HPP
#include <iomanip>

#include "Coordinates.hpp"
#include "Player.hpp"

class Move
{
    private: 
        std::size_t round_;
        PlayerColor color_;
        std::string to_;
        std::string potion_short_name_;
        std::string special_name_;

    public:
        Move(std::size_t round, PlayerColor color, std::string to);
        Move(const Move&) = delete;
        Move& operator=(const Move&) = delete;
        ~Move() = default;

        /// @brief Setter for the potion name
        /// @param potion_short_name Short name of the potion that was used.
        void setPotionName(std::string potion_short_name) {potion_short_name_ = potion_short_name;};

        /// @brief Setter for the special name.
        /// @param special_name S + coordinates of the square
        void setSpecialName(std::string special_name) {special_name_ = special_name;}

        /// @brief Getter for potion_short_name_.
        /// @return Returns potion_short_name_, as std::string.
        std::string getPotionName() const {return potion_short_name_;}

        /// @brief Getter for color_.
        /// @return Returns color_, as  PlayerColor.
        PlayerColor getPlayerColor() const {return color_;};

        /// @brief Getter for round_
        /// @return Returns round_, as std::size_t
        std::size_t getRound() const {return round_;};

        /// @brief Getter for the coordinates of the square that the piece moved to
        /// @return Returns those coordinate in form of std::string
        std::string getMoveString() const {return to_;};

        /// @brief Getter for the special_name_.
        /// @return Returns special_name_, as std::string.
        std::string getSpecialName() const {return special_name_;}

};

#endif
