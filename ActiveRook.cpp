#include "ActiveRook.hpp"
#include "Square.hpp"
#include "Board.hpp"

size_t ActiveRook::getManaCost()
{
  return mana_cost_;
}

size_t InvincibleRook::getManaCost()
{
  return (size_t) initially_invincible_for_;
}

size_t PainterRook::getManaCost()
{
  if (target_square_ == nullptr)
  {
    return 0;
  }
  size_t current_file = this->getCoordinates().getFile();
  size_t current_rank = this->getCoordinates().getRank();
  
  size_t target_file = target_square_->getCoordinates().getFile();
  size_t target_rank = target_square_->getCoordinates().getRank();

  if(target_rank == current_rank)
  {
    if(current_file > target_file)
    {
      return (current_file - target_file);
    }

    return (target_file - current_file);
  }
  else if(target_file == current_file)
  {
    if(current_rank > target_rank)
    {
      return (current_rank - target_rank);
    }
    return (target_rank - current_rank);
  }

  return 0;
}

void InvincibleRook::special(Board* board, Player* opponent)
{
  (void) opponent;
  (void) board;
  is_frozen_ = true;
}

void PainterRook::special(Board* board, Player* opponent)
{
  (void) opponent;
  size_t current_file = coordinates_.getFile();
  size_t current_rank = coordinates_.getRank();

  Square* current_square = board->getSquare(coordinates_);

  if(current_square->getSquareType() != SquareType::WHITE && current_square->getSquareType() != SquareType::BLACK)
  {
    SquareType type = SquareType::WHITE;
    if(owner_color_ == PlayerColor::WHITE)
    {
      type = SquareType::WHITE;
    }
    else if(owner_color_ == PlayerColor::BLACK)
    {
      type = SquareType::BLACK;
    }
    current_square->setSquareType(type);
  }

  size_t target_file = target_square_->getCoordinates().getFile();
  size_t target_rank = target_square_->getCoordinates().getRank();

  int rank_count = 0;
  int file_count = 0;
  
  if(current_rank > target_rank)
    rank_count = -1;
  else
    rank_count = 1;

  if(current_file > target_file)
    file_count = -1;
  else
    file_count = 1;

  if(target_rank == current_rank)
  {
    do
    {
      Coordinates get = Coordinates(current_file, target_rank);
      Square* to_be_changed = board->getSquare(get);
      to_be_changed->setSquareType(current_square->getSquareType());
      current_file += file_count;
    }while(current_file != target_file + file_count);
  }
  else if(target_file == current_file)
  {
    do
    {
      Coordinates get = Coordinates(target_file, current_rank);
      Square* to_be_changed = board->getSquare(get);
      to_be_changed->setSquareType(current_square->getSquareType());
      current_rank += rank_count;
    }while(current_rank != target_rank + rank_count);
  }

  Piece* target_piece = target_square_->getPiece();

  if(target_piece != nullptr)
  {
    if(target_piece->getItem() != nullptr)
    {
      auto item = target_piece->getItem();
      target_piece->removeItem();
      this->setItem(item);
    }
    target_piece->sendToPrison();
    target_square_->removePiece();
  }

  current_square->removePiece();
  target_square_->setPiece(this);
  target_square_->setSquareType(current_square->getSquareType());
  coordinates_ = target_square_->getCoordinates();
}
