#include "Square.hpp"
#include "Piece.hpp"
#include <iomanip>

Square::Square(Coordinates coordinates): coordinates_(coordinates), item_(std::nullopt), piece_(std::nullopt),
    under_attack_(false) {
        if(coordinates.getFile() % 2 == 1 && coordinates.getRank() % 2 == 1)
        {
            square_type_ = SquareType::BLACK;
        }
        else if(coordinates.getFile() % 2 == 0 && coordinates.getRank() % 2 == 1)
        {
            square_type_ = SquareType::WHITE;
        }
        else if(coordinates.getFile() % 2 == 0 && coordinates.getRank() % 2 == 0)
        {
            square_type_ = SquareType::BLACK;
        }
        else
        {
            square_type_ = SquareType::WHITE;
        }
    }

bool Square::isOccupied() {
    return piece_.has_value();
}

Piece* Square::getPiece() {
    if (piece_.has_value()) 
    {
        return piece_.value();
    }
    return nullptr;
}

void Square::setPiece(Piece* piece) {
    piece_ = piece;
}

void Square::removePiece() {
    piece_ = std::nullopt;
}

Item* Square::getItem() {
    if (item_.has_value()) 
    {
        return item_.value();
    }
    return nullptr;
}

bool Square::triggerEffect(std::size_t round, Player* active_player) { 
    (void) active_player;
    (void) round;
    return false;
}

void Square::print(Player* opponent, Player* active, Board* board)
{
    if(square_type_ == SquareType::MANA)
    {
        std::cout << "\033[48;5;32m";
    }
    else if(square_type_ == SquareType::BOOST)
    {
        std::cout << "\033[48;5;226m";
    }
    else if(square_type_ == SquareType::SPAWN)
    {
        std::cout << "\033[48;5;28m";
    }
    else if(square_type_ == SquareType::BLACK)
    {
        std::cout << "\033[48;5;94m";
    }
    else if(square_type_ == SquareType::WHITE)
    {
        std::cout << "\033[48;5;223m";
    }

    Player* kings_opponent = active;
    if(piece_.has_value() && piece_.value()->getColor() == active->getColor())
        kings_opponent = opponent;

    if(!(piece_.has_value() && piece_.value()->hasItem("CLOAK") && piece_.value()->getColor() != active->getColor()) 
        && piece_.has_value() && piece_.value()->getPieceType() == PieceType::K 
        && piece_.value()->isInCheck(kings_opponent, board))
    {
        std::cout << "!";
    }
    else if(!(piece_.has_value() && piece_.value()->hasItem("CLOAK") && piece_.value()->getColor() != active->getColor()) 
        && piece_.has_value() && piece_.value()->getPieceType() == PieceType::K 
        && piece_.value()->isInStalemate(kings_opponent, board))
    {
        std::cout << "?";
    }
    else if(!(piece_.has_value() && piece_.value()->hasItem("CLOAK") 
        && piece_.value()->getColor() != active->getColor()) &&  piece_.has_value() 
        && piece_.value()->getPieceType() == PieceType::K && piece_.value()->isInCheckMate(kings_opponent, board))
    {
        std::cout << "#";
    }
    else if(item_.has_value() ) 
    {
        std::cout << item_.value()->getDisplayName();
    }
    else if(!(piece_.has_value() && piece_.value()->hasItem("CLOAK") 
    && piece_.value()->getColor() != active->getColor())  && piece_.has_value() && piece_.value()->getItem() != nullptr)
    {
        std::cout << piece_.value()->getItem()->getDisplayName();
    } 
    else
    {
        std::cout << " ";
    }

    if(piece_.has_value() && !(piece_.has_value() && piece_.value()->hasItem("CLOAK") && 
    piece_.value()->getColor() != active->getColor()))
    {
        if(piece_.value()->getColor() == PlayerColor::BLACK)
        {
            std::cout << "\033[1;38;5;16m";
        }
        else if(piece_.value()->getColor() == PlayerColor::WHITE)
        {
            std::cout << "\033[1;38;5;247m";
        }
        std::cout << std::left << std::setw(5) << piece_.value()->getShortName();
    }
    else
    {
        std::cout << "   ";
    }

    std::cout << "\033[0m";
}


bool Square::isInOffside(std::size_t active_player_furthest_rank, PlayerColor player_color)
{
    if(player_color == PlayerColor::WHITE)
    {
        if(coordinates_.getRank() >= active_player_furthest_rank)
        {
            return true;
        }
    }
    else
    {
        if(coordinates_.getRank() <= active_player_furthest_rank)
        {
            return true;
        }
    }
    

    return false;
}