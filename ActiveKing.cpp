#include "ActiveKing.hpp"

size_t ActiveKing::getManaCost()
{
  return mana_cost_;
}

size_t ArcherKing::getManaCost()
{
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

void ArcherKing::special(Board* board, Player* opponent)
{
  (void) board;
  (void) opponent;
  target_square_->getPiece()->setFrozen();
}
