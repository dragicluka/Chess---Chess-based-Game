#include "Pawn.hpp"
#include "Player.hpp"
#include "Board.hpp"

void Pawn::checkPromotion()
{
  if((this->getColor() == PlayerColor::WHITE && this->coordinates_.getRank() == 7)
  || (this->getColor() == PlayerColor::BLACK && this->coordinates_.getRank() == 0))
  {
    //promote radi tako sto trenutni pawn promeni sa target_piece
    //kad procitas iz move commande setuj
    promote();
  }
}

void Pawn::promote()
{
  std::vector<std::unique_ptr<Piece>>& pieces = owner_->getPieces();

  if(target_piece_ == nullptr)
    return;

  for(auto& current : pieces)
  {
    if(current.get() == this)
    {
      current = std::move(target_piece_);
      break;
    }
  }
}



bool Pawn::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
{
  
  std::size_t piece_file = coordinates_.getFile();
  std::size_t piece_rank = coordinates_.getRank();
  std::size_t target_file = to.getFile();
  std::size_t target_rank = to.getRank();

  if(target_file == piece_file && target_rank == piece_rank)
  {
    return false;
  }

  if(!is_capture)
  {
    return canMoveForward(board, piece_file, piece_rank, target_file, target_rank);
  }
  else
  {
    return canCapture(board, piece_file, piece_rank, target_file, target_rank);
  }
}
  
bool Pawn::canMoveForward(Board& board, std::size_t piece_file, std::size_t piece_rank,std::size_t target_file, 
  std::size_t target_rank)
{
  if(piece_file != target_file)
  {
    return false;
  }
  if (board.getSquare(Coordinates(target_file, target_rank))->getPiece() != nullptr)
  {
    return false;
  }
  if(owner_color_ == PlayerColor::WHITE)
  {
    if(target_rank <= piece_rank)
    {
      return false;
    }
    if(target_rank ==3 && piece_rank == 1)
    {
      if(has_moved_)
      {
        return false;
      }
      if(board.getSquare(Coordinates(piece_file, 2))->getPiece() != nullptr)
      {
        return false;
      }
    }
    else if((target_rank - piece_rank) != 1)
    {
      return false;
    }
  }
  if(owner_color_ == PlayerColor::BLACK)
  {
    if(target_rank >= piece_rank)
    {
      return false;
    }
    if(target_rank == 4 && piece_rank == 6)
    {
      if (has_moved_)
      {
        return false;
      }
      if(board.getSquare(Coordinates(piece_file, 5))->getPiece() != nullptr)
      {
        return false;
      }
    }
    else if((piece_rank - target_rank) !=1)
    {
      return false;
    }
  }
  return true;
}

bool Pawn::canCapture(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file, 
  std::size_t target_rank)
{
  const std::array<std::array<std::unique_ptr<Square>,8>,8>& squares = board.getBoard();
  Piece* en_passant_pawn = nullptr;
  if(owner_color_ == PlayerColor::WHITE)
  {
    en_passant_pawn = squares[4][target_file]->getPiece();
  }
  else
  {
    en_passant_pawn = squares[3][target_file]->getPiece();
  }
  if(en_passant_pawn != nullptr && en_passant_pawn->getPawnMovedTwoSquares())
  {
    moving_en_passant_ = true;
  }


  if(!moving_en_passant_ &&  squares[target_rank][target_file]->getPiece() == nullptr)
  {
    return false;
  }

  if(piece_file == target_file)
  {
    return false;
  }
  if(owner_color_ == PlayerColor::WHITE)
  {
    if(target_rank <= piece_rank)
    {
      return false;
    }
    if((target_rank - piece_rank != 1) || std::abs(static_cast<int>(target_file) - static_cast<int>(piece_file)) != 1)
    {
      return false;
    }
  }
  if(owner_color_ == PlayerColor::BLACK)
  {
    if(target_rank >= piece_rank)
    {
      return false;
    }
    if(piece_rank - target_rank !=1 || std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file)) != 1)
    {
      return false;
    }
  }
  return true;
}