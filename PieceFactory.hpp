#ifndef PIECEFACTORY_HPP
#define PIECEFACTORY_HPP

#include <memory>
#include <string>
#include <iostream>
#include "Piece.hpp"
#include "Player.hpp"
#include "Coordinates.hpp"

class PieceFactory
{
  public:
    /// @brief This function allocates pieces.
    /// @param id Piece id read from game_config file, that determines, which piece will be allocated.
    /// @param owner_color Color of the player, which determines, who the owner of the piece will be.
    /// @param coordinates Coordinates where piece will be positioned.
    /// @return Returns unique ptr for the allocated piece.
    static std::unique_ptr<Piece> makePiece(const std::string& id, PlayerColor owner_color, Coordinates coordinates);
    PieceFactory(const PieceFactory&) = delete;
    PieceFactory& operator=(const PieceFactory&) = delete;
    ~PieceFactory() = default;
};

#endif