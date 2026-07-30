#include "ActiveQueen.hpp"
#include "Board.hpp"

std::size_t ActiveQueen::getManaCost()
{
  return mana_cost_;
}

size_t FlipperQueen::getManaCost()
{
  if(target_square_ == nullptr)
  {
    return 0;
  }
  size_t target_file = target_square_->getCoordinates().getFile();
  size_t target_rank = target_square_->getCoordinates().getRank();
  size_t bounce_file = bounce_square_->getCoordinates().getFile();
  size_t bounce_rank = bounce_square_->getCoordinates().getRank();

  size_t max = 0;

  if((abs(static_cast<int>(target_file) - static_cast<int>(bounce_file))) > 
    (abs(static_cast<int>(target_rank) - static_cast<int>(bounce_rank))))
    max = abs((static_cast<int>(target_file) - static_cast<int>(bounce_file)));
  else
    max = abs((static_cast<int>(target_rank) - static_cast<int>(bounce_rank)));

  return max;
}

void FlipperQueen::special(Board* board, Player* opponent)
{
  (void) opponent;
  board->getSquare(coordinates_)->removePiece();

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

void JumpyQueen::special(Board* board, Player* opponent)
{
  (void) opponent;
  board->getSquare(coordinates_)->removePiece();

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