#include "King.hpp"
#include "Board.hpp"


bool King::isInCheckMate(Player* opponent, Board* board)
{
  if(!(isInCheck(opponent, board)))
    return false;

  Coordinates original = coordinates_;
  int target_file = static_cast<int>(coordinates_.getFile());
  int target_rank = static_cast<int>(coordinates_.getRank());

  //claude
  //begin
  int offsets[8][2] = {{0,-1},{0,1},{1,-1},{1,0},{1,1},{-1,-1},{-1,0},{-1,1}};
  //end
  for(auto& offset : offsets)
  {
    Square* target_square= board->getSquare(Coordinates(target_file + offset[0], target_rank + offset[1]));
    if(target_square != nullptr && !checkKingsSurrondings(board, original, target_square, opponent))
    {
      return false;
    }
  }

  if(checkForDefense(opponent, board))
    return false;
  
  return true;
}

bool King::isInStalemate(Player* opponent, Board* board)
{
  if(isInCheck(opponent, board))
    return false;
  
  if(owner_->getNumberOfMoves() > 0)
    return false;


  for(auto& piece : owner_->getPieces())
  {
    Coordinates piece_original_coordinates = piece->getCoordinates();

    if(piece.get()->getPieceType() == PieceType::K) 
      continue;

    for(int file = 0; file < 8; file++)
    {
      for(int rank = 0; rank < 8; rank++)
      {
        Coordinates target(file, rank);
        Square* target_square = board->getSquare(target);

        if(target_square == nullptr) 
          continue;
        if(target_square->getPiece() != nullptr && target_square->getPiece()->getColor() == owner_->getColor()) 
          continue;

        if(piece->canMoveToTargetSquare(*board, target, false) &&
        !simulateAMove(board, piece_original_coordinates, target, target_square, opponent, piece.get()))
        {
          return false;
        }

        if(target_square->getPiece() != nullptr && piece->getPieceType() == PieceType::P &&
        piece->canMoveToTargetSquare(*board, target, true) &&
        !simulateAMove(board, piece_original_coordinates, target, target_square, opponent, piece.get()))
        {
          return false;
        }
      }
    }
  }
  
  Coordinates original = coordinates_;

  int target_file = static_cast<int>(coordinates_.getFile());
  int target_rank = static_cast<int>(coordinates_.getRank());

  //claude
  //begin
  int offsets[8][2] = {{0,1},{0,-1},{1,-1},{1,0},{1,1},{-1,-1},{-1,0},{-1,1}};
  //end

  for(auto& offset : offsets)
  {
    Square* target_square= board->getSquare(Coordinates(target_file + offset[0], target_rank + offset[1]));
    if(target_square != nullptr && !checkKingsSurrondings(board, original, target_square, opponent))
    {
      return false;
    }
  }
  return true;
}

bool King::isInCheck(Player* opponent, Board* board)
{
  std::vector<std::unique_ptr<Piece>>& opponent_pieces = opponent->getPieces();
  for(auto& piece : opponent_pieces)
  {
    if(piece.get()->getPieceType() == PieceType::P)
    {
      if(piece.get()->canMoveToTargetSquare(*board, coordinates_, true))
        return true;
    }
    else
    {
      if(piece.get()->canMoveToTargetSquare(*board, coordinates_, false))
        return true;
    }
  }
  return false;
}

bool King::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
{
  (void) board;
  (void) is_capture;
  std::size_t piece_file = coordinates_.getFile();
  std::size_t piece_rank = coordinates_.getRank();
  std::size_t target_file = to.getFile();
  std::size_t target_rank = to.getRank();

  std::size_t file_dif = std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file));
  std::size_t rank_dif = std::abs(static_cast<int>(piece_rank) - static_cast<int>(target_rank));
  
  castling_rook_ = nullptr;

  if(file_dif == 0 && rank_dif == 0)
  {
    return false;
  }
  if(file_dif <= 1 && rank_dif <= 1)
  {
    return true;
  }

  if(rank_dif == 0 && file_dif == 2 && !has_moved_)
  {
    if(target_file > piece_file)
    {
      for(std::size_t i = piece_file + 1; i < 8; i++)
      {
        auto piece = board.getSquare(Coordinates(i, piece_rank))->getPiece();
        if (piece != nullptr)
        { 
          if(piece->getPieceType() == PieceType::R && !piece->getHasMoved())
          {
            auto target_piece = board.getSquare(to)->getPiece();
            if(target_piece != nullptr && target_piece != piece)
            {
                return false;
            }
            castling_rook_ = piece;
            return true;
          }
          return false;
        }
      }
    }
    else
    {
      for(int i = static_cast<int>(piece_file) - 1 ; i >= 0; i--)
      {
        auto piece = board.getSquare(Coordinates(i, piece_rank))->getPiece();
        if (piece != nullptr)
        { 
          if(piece->getPieceType() == PieceType::R && !piece->getHasMoved())
          {
            auto target_piece = board.getSquare(to)->getPiece();
            if(target_piece != nullptr && target_piece != piece)
            {
              return false;
            }
            castling_rook_ = piece;
            return true;
          }
          return false;
        }
      }
    }
  }
  return false;
}

bool King::checkForDefense(Player* opponent, Board* board)
{
  int threats = 0;
  int solutions = 0;
  std::vector<std::unique_ptr<Piece>>& opponent_pieces = opponent->getPieces();
  for(auto& opps : opponent_pieces)
  {
    if(opps.get()->getPieceType() == PieceType::P)
    {
      if(opps.get()->canMoveToTargetSquare(*board, coordinates_, true))
      {
        threats ++;
        for(auto& piece : owner_->getPieces())
        {
          if(piece.get()->getPieceType() == PieceType::P)
          {
            if(piece.get()->canMoveToTargetSquare(*board, opps.get()->getCoordinates(), true))
              solutions++;
          }
          else if(piece.get()->canMoveToTargetSquare(*board, opps.get()->getCoordinates(), false))
          {
            solutions++;
          }
          else if(blockThePath(board, opps.get()))
          {
            solutions++;
          }
        }
      }
    }
    else
    {
      if(opps.get()->canMoveToTargetSquare(*board, coordinates_, false))
      {
        threats++;
        for(auto& piece : owner_->getPieces())
        {
          if(piece.get()->getPieceType() == PieceType::P)
          {
            if(piece.get()->canMoveToTargetSquare(*board, opps.get()->getCoordinates(), true))
              solutions++;
          }
          else if(piece.get()->canMoveToTargetSquare(*board, opps.get()->getCoordinates(), false))
          {
            solutions++;
          }
          else if(blockThePath(board, opps.get()))
          {
            solutions++;
          }
        }
      }
    }
  }

  if(threats > 1)
    return false;

  if(threats != 0 && solutions == 0)
    return false;
  
  return true;
}

bool King::blockThePath(Board* board, Piece* piece)
{
  std::vector<Coordinates> path = piece->getCheckedPath();
  if(path.empty())
  {
    return false;
  }
  for(auto way : path)
  {
    for(auto& owner_pieces : owner_->getPieces())
    {
      if(owner_pieces->canMoveToTargetSquare(*board, way, false))
      {
        return true;
      }
    }
  }
  return false;
}

bool King::checkKingsSurrondings(Board* board, Coordinates original, Square* target_square, Player* opponent)
{
  Square* original_square = board->getSquare(original);

  Piece* possible_move = target_square->getPiece();
  if(possible_move != nullptr && possible_move->getColor() == owner_color_)
    return true;
    
  if(possible_move != nullptr && possible_move->getColor() != owner_color_)
  {
    target_square->removePiece();
  }
  original_square->removePiece();
  target_square->setPiece(this);
  coordinates_ = target_square->getCoordinates();
  if(!isInCheck(opponent, board))
  {
    target_square->removePiece();
    if(possible_move != nullptr)
      target_square->setPiece(possible_move);
    original_square->setPiece(this);
    coordinates_ = original;
    return false;
  }
  target_square->removePiece();
  if(possible_move != nullptr)
    target_square->setPiece(possible_move);
  original_square->setPiece(this);
  coordinates_ = original;
  return true;
}

bool King::simulateAMove(Board* board, Coordinates piece_original_coordinates, Coordinates target,
Square* target_square, Player* opponent, Piece* piece)
{
  Square* original_square = board->getSquare(piece_original_coordinates);

  Piece* possible_move = target_square->getPiece();

  if(possible_move != nullptr && possible_move->getColor() == owner_color_)
    return true;

  if(possible_move != nullptr && possible_move->getColor() != owner_color_)
  {
    target_square->removePiece();
  }
  original_square->removePiece();
  target_square->setPiece(piece);
  piece->setCoordinates(target);
  if(!isInCheck(opponent, board))
  {
    target_square->removePiece();
    if(possible_move != nullptr)
      target_square->setPiece(possible_move);
    original_square->setPiece(piece);
    piece->setCoordinates(piece_original_coordinates);
    return false;
  }
  target_square->removePiece();
  if(possible_move != nullptr)
    target_square->setPiece(possible_move);
  original_square->setPiece(piece);
  piece->setCoordinates(piece_original_coordinates);
  return true;
}
