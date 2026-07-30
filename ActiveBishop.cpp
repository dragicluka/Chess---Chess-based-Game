#include "ActiveBishop.hpp"
#include "Square.hpp"
#include "Board.hpp"

size_t PreacherBishop::getManaCost()
{
  if(target_square_ == nullptr)
  {
    return 0;
  }
  Piece* piece = target_square_->getPiece();
  size_t cost = 3 * piece->getValue();
  return cost;
}

void ColorBlindBishop::special(Board* board, Player* opponent)
{
  (void) opponent;
  Square* current_square = board->getSquare(coordinates_);
  current_square->removePiece();

  if(inherent_color_ != target_square_->getSquareType())
  {
    if(target_square_->getSquareType() == SquareType::BLACK || target_square_->getSquareType() == SquareType::WHITE)
      inherent_color_ = target_square_->getSquareType();
  }

  Piece* piece = target_square_->getPiece();
  
  if(piece == nullptr)
  {
    target_square_->setPiece(this);
    coordinates_ = target_square_->getCoordinates();
  }
  else
  {
    if(piece->getItem() != nullptr)
    {
      auto item = piece->getItem();
      piece->removeItem();
      this->setItem(item);
    }
    target_square_->removePiece();
    target_square_->setPiece(this);
    piece->sendToPrison();
    coordinates_ = target_square_->getCoordinates();
  }

  if(target_square_->getItem() != nullptr)
  {
    auto item = target_square_->getItem();
    target_square_->removeItem();
    this->setItem(item);
  }
}

void PreacherBishop::special(Board* board, Player* opponent)
{
  (void) opponent;
  (void) board;
  Piece* piece = target_square_->getPiece();
  std::string piece_id = piece->getIdString(piece->getId());
  Coordinates piece_coordinates = piece->getCoordinates();
  piece->setOwnerColor(owner_color_);
  piece->setNewShortName(piece_id, owner_color_, piece_coordinates);
}

