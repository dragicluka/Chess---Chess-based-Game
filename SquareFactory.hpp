#ifndef SQUAREFACTORY_HPP
#define SQUAREFACTORY_HPP

#include "Square.hpp"

class SquareFactory
{
  public:
    SquareFactory() = default;
    SquareFactory(const SquareFactory&) = delete;
    ~SquareFactory() = default;
    SquareFactory& operator=(const SquareFactory&) = delete;

    /// @brief This function allocates special squares.
    /// @param square_id Reference to the square id.
    /// @param coordinates Coordinates on which special sqaures should be on board.
    /// @return Returns unique ptr for corresponding special square.
    static std::unique_ptr<Square> makeSquare(std::string& square_id, Coordinates coordinates);
};

#endif