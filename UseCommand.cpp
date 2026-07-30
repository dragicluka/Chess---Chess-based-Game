#include "UseCommand.hpp"
#include "Coordinates.hpp"
#include "Board.hpp"
#include "Potion.hpp"

const std::string EVEN = "even";
const std::string ODD = "odd";
const std::string FREEZE_ID = "FREEZE";
const std::string TP_ID = "TP";
const std::string EVENODD_ID = "EVENODD";
const std::string EMPTY_STRING = "";
const std::string LUKE_ID = "LUKE";
const std::string MIRROR_ID = "MIRROR";



CommandReturnType UseCommand::execute(Board& board, Player& active_player, Player& opponent_player) 
{
  (void) opponent_player;

  Square* source_square = nullptr;
  Piece* piece_on_square = nullptr;
  Potion* potion = nullptr;

  CommandReturnType validation = validate(board, active_player, potion, source_square, piece_on_square);

  if(validation != CommandReturnType::ACTIVE_SUCCESS)
    return validation;

  if(potion->getId() == FREEZE_ID)
    return handleFreeze(board, active_player, potion, source_square, piece_on_square);
  
  if(potion->getId() == TP_ID)
    return handleTP(board, active_player, potion, source_square, piece_on_square);
  
  if(potion->getId() == EVENODD_ID)
    return handleEvenOdd(board, active_player, potion, source_square, piece_on_square);
  
  if(potion->getId() == LUKE_ID)
    return handleLuke(board, active_player, potion, source_square, piece_on_square);
  
  return CommandReturnType::ERROR;
}



CommandReturnType UseCommand::validate(Board& board, Player& active_player, Potion*& potion, Square*& source_square, 
Piece*& piece_on_square)
{
  if(parameters_.empty())
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  if(active_player.getNumberOfMoves() != 0)
  {
    printErrorMessage("E_SPECIAL_USE_UNAVAILABLE");
    return CommandReturnType::ERROR;
  }

  std::string source_square_string = parameters_.at(0);
  Coordinates source_square_coordinates = Coordinates(source_square_string);

  source_square = board.getSquare(source_square_coordinates);

  if(source_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_SQUARE");
    return CommandReturnType::ERROR;
  }

  piece_on_square = source_square->getPiece(); 

  if(piece_on_square == nullptr || piece_on_square->getColor() != active_player.getColor())
  {
    printErrorMessage("E_PLAYER_PIECE_NOT_FOUND");
    return CommandReturnType::ERROR;
  }

  Item* item = piece_on_square->getItem();

  if(item == nullptr)
  {
    printErrorMessage("E_NO_POTION_FOUND");
    return CommandReturnType::ERROR;
  }

  potion = dynamic_cast<Potion*>(item);

  if(potion == nullptr)
  {
    printErrorMessage("E_NO_POTION_FOUND");
    return CommandReturnType::ERROR;
  }

  std::size_t expected_number_of_params = potion->getRequiredParameterCount() + 1;

  if(parameters_.size() != expected_number_of_params)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT_USE");
    return CommandReturnType::ERROR;
  }

  return CommandReturnType::ACTIVE_SUCCESS;
}



CommandReturnType UseCommand::handleFreeze(Board& board, Player& active_player, Potion* potion, Square* source_square, 
Piece* piece_on_square)
{
  std::string target_square_string = parameters_.at(1);
  Coordinates target_square_coordinates = Coordinates(target_square_string);
  Square* target_square = board.getSquare(target_square_coordinates);

  if(target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  Piece* piece_on_target_square = target_square->getPiece();
  //Da li player moze da zamrzne sopstveni piece?
  if(piece_on_target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  auto move = std::make_unique<Move>(board.getRound(), active_player.getColor(), parameters_[0]);
  move->setPotionName(potion->getDisplayName());
  board.addToMoveHistory(std::move(move));
  

  potion->triggerEffect(board, source_square, target_square, EMPTY_STRING);
  piece_on_square->removeItem();
  return CommandReturnType::ACTIVE_SUCCESS;
}



CommandReturnType UseCommand::handleTP(Board& board, Player& active_player, Potion* potion, Square* source_square, 
Piece* piece_on_square)
{
  std::string target_square_string = parameters_.at(1);
  Coordinates target_square_coordinates = Coordinates(target_square_string);
  Square* target_square = board.getSquare(target_square_coordinates);

  if(target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  std::size_t active_player_furthest_rank = active_player.getFurthestRank();
  PlayerColor active_player_color = active_player.getColor();

  if(target_square->isInOffside(active_player_furthest_rank, active_player_color))
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  if(target_square->getPiece() != nullptr)
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  auto move = std::make_unique<Move>(board.getRound(), active_player.getColor(), parameters_[0]);
  move->setPotionName(potion->getDisplayName());
  board.addToMoveHistory(std::move(move));
  

  potion->triggerEffect(board, source_square, target_square, EMPTY_STRING);
  piece_on_square->removeItem();
  return CommandReturnType::ACTIVE_SUCCESS;
}



CommandReturnType UseCommand::handleEvenOdd(Board& board, Player& active_player, Potion* potion, Square* source_square, 
Piece* piece_on_square)
{
  std::string target_square_string = parameters_.at(1);
  Coordinates target_square_coordinates = Coordinates(target_square_string);
  Square* target_square = board.getSquare(target_square_coordinates);
  std::string parity = parameters_.at(2);

  if(target_square == nullptr || target_square->getPiece() == nullptr || 
  target_square->getPiece()->getColor() == active_player.getColor() || (parity != EVEN && parity != ODD))
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  auto move = std::make_unique<Move>(board.getRound(), active_player.getColor(), parameters_[0] );
  move->setPotionName(potion->getDisplayName());
  board.addToMoveHistory(std::move(move));
  

  potion->triggerEffect(board, source_square, target_square, parity);
  piece_on_square->removeItem();
  return CommandReturnType::ACTIVE_SUCCESS;
}



CommandReturnType UseCommand::handleLuke(Board& board, Player& active_player, Potion* potion, Square* source_square, 
Piece* piece_on_square)
{
  if(!potion->triggerEffect(board, source_square, nullptr, EMPTY_STRING))
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;
  }

  auto move = std::make_unique<Move>(board.getRound(), active_player.getColor(), parameters_[0]);
  move->setPotionName(potion->getDisplayName());
  board.addToMoveHistory(std::move(move));
  

  piece_on_square->removeItem();
  return CommandReturnType::ACTIVE_SUCCESS;
}


/*CommandReturnType UseCommand::handleMirror(Board& board, Player& active_player, Potion* potion, Square* source_square, 
Piece* piece_on_square)
{
  (void)piece_on_square;
  (void)active_player;
  
  std::string target_square_string = parameters_.at(1);
  Coordinates target_square_coordinates = Coordinates(target_square_string);
  Square* target_square = board.getSquare(target_square_coordinates);

  if(target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;    
  }

  Piece* piece_on_target_square = target_square->getPiece();

  if(piece_on_target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;    
  }

  if((source_square->getCoordinates().getFile() == target_square->getCoordinates().getFile()) && 
  (source_square->getCoordinates().getRank() == target_square->getCoordinates().getRank()))
  {
    printErrorMessage("E_INV_PARAM_USE");
    return CommandReturnType::ERROR;  
  }

  potion->triggerEffect(board, source_square, target_square, "");
  return CommandReturnType::ACTIVE_SUCCESS;
}*/