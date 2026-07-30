#include "ActivePawn.hpp"
#include "Coordinates.hpp"
#include "Square.hpp"
#include "Board.hpp"

size_t ActivePawn::getManaCost()
{
  return mana_cost_;
}

size_t ImpatientPawn::getManaCost() 
{
  size_t rank_difference  = 0; 
  size_t back_rank = 0;
  if(owner_color_ == PlayerColor::WHITE)
  {
    back_rank = 7;
  }
  else
  {
    back_rank = 0;
  }
  size_t current_rank = coordinates_.getRank();

  if(current_rank > back_rank)
  {
    rank_difference = current_rank - back_rank;
  }
  else
  {
    rank_difference = back_rank - current_rank;
  }
  if(target_piece_ == nullptr)
    return 0;

  size_t value = target_piece_->getValue();
  return rank_difference * value;
}

void ImpatientPawn::special(Board* board, Player* opponent)
{
  (void) opponent;
  board->getSquare(coordinates_)->removePiece();
  coordinates_ = target_square_->getCoordinates();
  target_square_->removePiece();
  target_square_->setPiece(target_piece_.get());
  promote();
}

void StubbornPawn::special(Board* board, Player* opponent)
{
  (void) opponent;
  board->getSquare(coordinates_)->removePiece();
  Piece* piece = target_square_->getPiece();
  if(piece->getItem() != nullptr)
  {
    auto item = piece->getItem();
    piece->removeItem();
    this->setItem(item);
  }

  if(target_square_->getItem() != nullptr)
  {
    auto item = target_square_->getItem();
    target_square_->removeItem();
    this->setItem(item);
  }

  coordinates_ = target_square_->getCoordinates();
  piece->sendToPrison();
  target_square_->removePiece();
  target_square_->setPiece(this);
}

void NervousPawn::special(Board* board, Player* opponent)
{
  (void) opponent;
  board->getSquare(coordinates_)->removePiece();

  target_square_->setPiece(this);
  coordinates_ = target_square_->getCoordinates();

  if(target_square_->getItem() != nullptr)
  {
    auto item = target_square_->getItem();
    target_square_->removeItem();
    this->setItem(item);
  }
  
  Pawn::checkPromotion();
}

void ExplosivePawn::special(Board* board, Player* opponent)
{
  if(target_square_->getPiece() != nullptr && target_square_->getPiece()->hasItem("SHIELD"))
  {
    Item* shield_tool = target_square_->getPiece()->getItem();
    shield_tool->triggerEffect(*board, target_square_, nullptr, "");
    target_square_ = board->getSquare(coordinates_);
  }

  handlePieces(target_square_, opponent, board);

  int target_file = static_cast<int>(target_square_->getCoordinates().getFile());
  int target_rank = static_cast<int>(target_square_->getCoordinates().getRank());


  Square* to_be_checked = board->getSquare(Coordinates(target_file, (target_rank - 1)));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  to_be_checked = board->getSquare(Coordinates(target_file, (target_rank + 1)));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  to_be_checked = board->getSquare(Coordinates((target_file + 1), (target_rank - 1)));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  to_be_checked = board->getSquare(Coordinates((target_file + 1), target_rank));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  to_be_checked = board->getSquare(Coordinates((target_file + 1), (target_rank + 1)));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }
  
  to_be_checked = board->getSquare(Coordinates((target_file - 1), (target_rank - 1)));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  to_be_checked = board->getSquare(Coordinates((target_file - 1), target_rank));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  to_be_checked = board->getSquare(Coordinates((target_file - 1), (target_rank + 1)));
  if(to_be_checked != nullptr)
  {
    handlePieces(to_be_checked, opponent, board);
  }

  owner_->addToMyPrison(this, opponent);
  board->getSquare(coordinates_)->removePiece();
  if(target_square_ != board->getSquare(coordinates_) && target_square_->getPiece() != nullptr)
  {
    if(target_square_->getPiece()->hasItem("SHIELD"))
    {
      Item* shield_tool = target_square_->getPiece()->getItem();
      shield_tool->triggerEffect(*board, to_be_checked, nullptr, "");
    }
    target_square_->getPiece()->sendToPrison();
    target_square_->removePiece();
  }
}

void ExplosivePawn::handlePieces(Square* to_be_checked, Player* opponent, Board* board)
{
  Piece* piece = to_be_checked->getPiece();

  if(to_be_checked->getPiece() != nullptr)
  {
    if(piece->getPieceType() != PieceType::P)
    {
      if(piece->hasItem("SHIELD"))
      {
        Item* shield_tool = piece->getItem();
        shield_tool->triggerEffect(*board, to_be_checked, nullptr, "");
      }
      else if(piece->getColor() == owner_color_)
      {
        owner_->addToMyPrison(piece, opponent);
        to_be_checked->removePiece();
      }
      else
      {
        piece->sendToPrison();
        to_be_checked->removePiece();
      }
    }
    else if(piece->hasItem("SHIELD"))
    {
      Item* shield_tool = piece->getItem();
      shield_tool->triggerEffect(*board, to_be_checked, nullptr, "");
    }
  }
}

