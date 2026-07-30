#include "Tool.hpp"
#include "Square.hpp"
#include "Piece.hpp"



bool ShieldTool::triggerEffect(Board& board, Square* source_square, Square* target_square, 
const std::string& even_or_odd) 
{
  (void)board;
  (void)target_square;
  (void)even_or_odd;

  source_square->getPiece()->removeItem();
  return true;
}