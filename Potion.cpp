#include "Potion.hpp"
#include "Board.hpp"

bool FreezePotion::triggerEffect(Board& board, Square* source_square, Square* target_square, 
const std::string& even_or_odd)
{
  (void)source_square;
  (void)even_or_odd;
  (void)board;
  Piece* target_piece = target_square->getPiece();
  target_piece->setFrozen();
  return true;
}

bool TeleportPotion::triggerEffect(Board& board, Square* source_square, Square* target_square, 
const std::string& even_or_odd)
{
  (void)even_or_odd;
  (void)board;
  Piece* piece_on_source_square = source_square->getPiece();
  source_square->removePiece();
  target_square->setPiece(piece_on_source_square);
  piece_on_source_square->setCoordinates(target_square->getCoordinates());
  return true;
}


bool EvenOddPotion::triggerEffect(Board& board, Square* source_square, Square* target_square, 
const std::string& even_or_odd)
{
  (void)board;
  (void)source_square;
  target_square->getPiece()->setEvenOdd(even_or_odd);
  return true;
}


bool SkyWalkerPotion::triggerEffect(Board& board, Square* source_square, Square* target_square, 
const std::string& even_or_odd)
{
  (void)target_square;
  (void)even_or_odd;

  Piece* piece_on_source_square = source_square->getPiece();

  int direction = piece_on_source_square->getColor() == PlayerColor::WHITE ? 1 : -1;
  
  std::vector<Square*> push_back_pieces;
  int current_rank = source_square->getCoordinates().getRank() + direction;
  int file = source_square->getCoordinates().getFile();

  while(current_rank >= 0 && current_rank <= 7)
  {
    Square* current_sqaure = board.getSquare(Coordinates(file, current_rank));
    if(current_sqaure->getPiece() == nullptr)
      break;
    
    push_back_pieces.push_back(current_sqaure);
    current_rank += direction;
  }

  if(push_back_pieces.empty())
    return false;
  
  Square* last_square = push_back_pieces.back();
  std::size_t last_square_rank = last_square->getCoordinates().getRank();

  if((last_square_rank == 7) || (last_square_rank == 0))
  {
    return false;
  }

  for(int index = push_back_pieces.size() - 1; index >= 0; index--)
  {
    Square* from = push_back_pieces.at(index);
    Coordinates to_coordinates = Coordinates(from->getCoordinates().getFile(), 
    from->getCoordinates().getRank() + direction);
    Square* to = board.getSquare(to_coordinates);

    Piece* push_back_piece = from->getPiece();
    from->removePiece();
    to->setPiece(push_back_piece);
    push_back_piece->setCoordinates(to_coordinates);
  }

  return true;
}


/*bool MirrorPotion::triggerEffect(Board& board, Square* source_square, Square* target_square, const std::string& even_or_odd)
{
  (void)even_or_odd;
  (void)board;
  Piece* source_square_piece = source_square->getPiece();
  Piece* target_square_piece = target_square->getPiece();

  source_square->removePiece();
  target_square->removePiece();

  source_square->setPiece(target_square_piece);
  target_square->setPiece(source_square_piece);
  source_square_piece->setCoordinates(target_square->getCoordinates());
  target_square_piece->setCoordinates(source_square->getCoordinates());

  return true;
}*/