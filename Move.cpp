#include "Move.hpp"
#include <iomanip>


Move::Move(std::size_t round, PlayerColor color, std::string to)
    : round_(round), color_(color), to_(to), potion_short_name_("")
{
}
