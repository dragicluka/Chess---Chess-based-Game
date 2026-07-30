#include "SpecialCommand.hpp"
#include "PieceFactory.hpp"
#include "Piece.hpp"
#include "Utils.hpp"
#include "Board.hpp"

CommandReturnType SpecialCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  Piece* current_piece = initialChecks(active_player, board);
  
  if(current_piece == nullptr)
    return CommandReturnType::ERROR;

  PieceID current_piece_id = current_piece->getId();

  if(!parameterCount(current_piece_id))
    return CommandReturnType::ERROR;

  if(current_piece->isFrozen())
  {
    printErrorMessage("E_PIECE_FROZEN");
    return CommandReturnType::ERROR;
  }

  if(!switchCase(board, active_player, opponent_player, current_piece))
    return CommandReturnType::ERROR;

  if(current_piece->getManaCost() > active_player.getMana())
  {
    printErrorMessage("E_INSUFFICIENT_MANA");
    return CommandReturnType::ERROR;
  }

  size_t mana_cost = current_piece->getManaCost();

  current_piece->special(&board, &opponent_player);
  if(current_piece_id != PieceID::PIPT)
  {
    Square* target_square = board.getSquare(current_piece->getCoordinates());

    if(target_square->getSquareType() == SquareType::MANA)
    {
      active_player.setMana(active_player.getMana() + 1);
    }

    if(target_square->getSquareType() == SquareType::BOOST)
    {
      active_player.setLastMovedPiece(current_piece);
      active_player.setCanMoveMultiple(true);
    }
  }
  
  size_t mana = active_player.getMana();
  active_player.setMana(mana - mana_cost);
  opponent_player.addToPrison();


  auto move = std::make_unique<Move>(board.getRound(), active_player.getColor(), parameters_[0]);
  move->setSpecialName("S"+parameters_[0]);
  board.addToMoveHistory(std::move(move));

  if(active_player.automaticDraw(&opponent_player))
    return CommandReturnType::KING_DRAW;

  if(active_player.automaticWin(&opponent_player))
    return CommandReturnType::WIN;
  
  for( auto& piece : opponent_player.getPrisonPieces())
  {
    if(piece.get()->getPieceType() == PieceType::K)
    {
      return CommandReturnType::WIN;
    }
  }
  
  return CommandReturnType::ACTIVE_SUCCESS;
}

Piece* SpecialCommand::initialChecks(Player& active_player, Board& board)
{
  if(parameters_.size() == 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return nullptr;
  }

  if(active_player.getNumberOfMoves() != 0)
  {
    printErrorMessage("E_SPECIAL_USE_UNAVAILABLE");
    return nullptr;
  }

  if(!(parameters_[0][0] >= 'a' && parameters_[0][0] <= 'h')
  && !(parameters_[0][1] >= '1' && parameters_[0][1] <= '8'))
  {
    printErrorMessage("E_INV_PARAM_SQUARE");
    return nullptr;
  }

  Coordinates coordinates = Coordinates(parameters_[0]);
  
  Square* square = board.getSquare(coordinates);

  if(square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_SQUARE");
    return nullptr;
  }

  Piece* current_piece = square->getPiece();

  if(current_piece == nullptr || current_piece->getColor() != active_player.getColor())
  {
    printErrorMessage("E_PLAYER_PIECE_NOT_FOUND");
    return nullptr;
  }

  if(current_piece->getPowerType() != PowerType::ACTIVE)
  {
    printErrorMessage("E_NO_SPECIAL_POWER");
    return nullptr;
  }

  return current_piece;
}

bool SpecialCommand::switchCase(Board& board, Player& active_player, Player& opponent_player, Piece* current_piece)
{
  PieceID current_piece_id = current_piece->getId();
  switch (current_piece_id)
  {
    case PieceID::PSTB:
      if(!(checkForwardSquare(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::PNRV:
      if(!(checkBackSquare(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::PIPT:
      if(!(checkPieceId(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::PEXP:
      if(!(checkTargetSquare(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::RINV:
      if(!(checkTurnCount(current_piece)))
        return false;
      break;

    case PieceID::RPNT:
      if(!(checkTargetSquare(current_piece, &active_player, &board)))
        return false;
      break;
      
    case PieceID::BCLR:
      if(!(checkTargetSquare(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::BPRC:
      if(!(checkTargetSquare(current_piece, &active_player, &board)))
        return false;
      else
        changeOwner(current_piece, &active_player, &opponent_player);
      break;

    case PieceID::QFLP:
      if(!(checkTargetSquareAndBounceSquare(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::QJMP:
      if(!(checkTargetSquare(current_piece, &active_player, &board)))
        return false;
      break;

    case PieceID::KARC:
      if(!(checkTargetSquare(current_piece, &active_player, &board)))
        return false;
      break;

    default:
      return true;
      break;
  }
  return true;
}

bool SpecialCommand::parameterCount(PieceID current_piece_id)
{
  if((current_piece_id == PieceID::PSTB || current_piece_id == PieceID::PNRV)
  && parameters_.size() != 1)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT_SPECIAL");
    return false;
  }
  else if(current_piece_id == PieceID::QFLP && parameters_.size() != 3)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT_SPECIAL");
    return false;
  }
  else if(current_piece_id != PieceID::PSTB && current_piece_id != PieceID::PNRV && current_piece_id != PieceID::QFLP
  &&  parameters_.size() != 2)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT_SPECIAL");
    return false;
  }
  return true;
}

bool SpecialCommand::checkPieceId(Piece* current_piece, Player* player, Board* board)
{
  std::string piece_id = parameters_[1];
  if(piece_id != "r" && piece_id != "n" && piece_id != "b" && piece_id != "q")
  {
    printErrorMessage("E_INV_PARAM_PIECE_TYPE");
    return false;
  }
  Utils::toUpperCase(parameters_[1]);

  std::unique_ptr<Piece> target = PieceFactory::makePiece(parameters_[1], player->getColor(), current_piece->getCoordinates());
  current_piece->setTargetPiece(std::move(target));
  
  Square* current_square = board->getSquare(current_piece->getCoordinates());
  current_piece->setTargetSquare(current_square);

  return true;
}

bool SpecialCommand::checkForwardSquare(Piece* current_piece, Player* player, Board* board)
{
  if(player->getColor() == PlayerColor::WHITE && current_piece->getCoordinates().getRank() == 7)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }
  else if(player->getColor() == PlayerColor::BLACK && current_piece->getCoordinates().getRank() == 0)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  Coordinates target_coordinates = current_piece->getCoordinates();

  if(player->getColor() == PlayerColor::WHITE)
  {
    target_coordinates.setRank(current_piece->getCoordinates().getRank() + 1);
  }
  else
  {
    target_coordinates.setRank(current_piece->getCoordinates().getRank() - 1);
  }

  Square* target_square = board->getSquare(target_coordinates);


  if((target_square->getPiece() == nullptr))
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  if(target_square->getPiece()->getColor() == player->getColor())
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  current_piece->setTargetSquare(target_square);

  return true;
}

bool SpecialCommand::checkBackSquare(Piece* current_piece, Player* player, Board* board)
{
  if(player->getColor() == PlayerColor::WHITE && current_piece->getCoordinates().getRank() == 0)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }
  else if(player->getColor() == PlayerColor::BLACK && current_piece->getCoordinates().getRank() == 7)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  Coordinates target_coordinates = Coordinates(current_piece->getCoordinates().getFile(), 0);

  if(player->getColor() == PlayerColor::WHITE)
  {
    target_coordinates.setRank(current_piece->getCoordinates().getRank() - 1);
  }
  else
  {
    target_coordinates.setRank(current_piece->getCoordinates().getRank() + 1);
  }

  Square* target_square = board->getSquare(target_coordinates);

  if(target_square->getPiece() != nullptr)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  current_piece->setTargetSquare(target_square);
  return true;
}

bool SpecialCommand::checkTurnCount(Piece* current_piece)
{
  std::string turn_count = parameters_[1];
  int count = stoi(turn_count);
  if(count <= 0)
  {
    printErrorMessage("E_INV_PARAM_TURN_COUNT");
    return false;
  }

  current_piece->setInitialInvincible(count);
  
  return true;
}

bool SpecialCommand::checkTargetSquare(Piece* current_piece, Player* player, Board* board)
{
  if(!(parameters_[1][0] >= 'a' && parameters_[1][0] <= 'h')
  && !(parameters_[1][1] >= '1' && parameters_[1][1] <= '8'))
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  Coordinates target_coordinates = Coordinates(parameters_[1]);
  
  Square* target_square = board->getSquare(target_coordinates);

  if(target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  size_t target_file = target_square->getCoordinates().getFile();
  size_t target_rank = target_square->getCoordinates().getRank();

  size_t current_file = current_piece->getCoordinates().getFile();
  size_t current_rank = current_piece->getCoordinates().getRank();

  if(target_file == current_file && target_rank == current_rank)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  PieceID id = current_piece->getId();

  if(id == PieceID::RPNT || id == PieceID::BPRC)
  {
    if(id == PieceID::BPRC && target_square->getPiece() != nullptr && target_square->getPiece()->getId() == PieceID::BPRC)
    {
      printErrorMessage("E_UNWAVERING_FAITH");
      return false;
    }
    else if(current_piece->canMoveToTargetSquare(*board, target_coordinates, false) &&
    (target_square->getPiece() == nullptr || (target_square->getPiece() != nullptr 
    && target_square->getPiece()->getColor() != current_piece->getColor())))
    {
      current_piece->setTargetSquare(target_square);
    } 
    else
    {
      printErrorMessage("E_INVALID_MOVE");
      return false;
    }
  }
  else if(id == PieceID::BCLR 
  && !colorBlindBishop(current_piece, target_square, target_file, target_rank, current_file, current_rank))
  {
   return false;
  }
  else if(id == PieceID::PEXP
  && !explosivePawn(current_piece, board, target_square, target_coordinates))
  {
   return false;
  }
  else if(id == PieceID::QJMP 
  && !jumpyQueen(current_piece, target_square, target_file, target_rank, current_file, current_rank))
  {
   return false;
  } 
  else if(id == PieceID::KARC)
  {
    if(!(checkPath(current_piece, player, target_coordinates, board)))
      return false;
  }
  current_piece->setTargetSquare(target_square);
  return true;
}

bool SpecialCommand::checkTargetSquareAndBounceSquare(Piece* current_piece, Player* player, Board* board)
{
  if(!(parameters_[1][0] >= 'a' && parameters_[1][0] <= 'h') && !(parameters_[1][1] >= '1' && parameters_[1][1] <= '8'))
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  Coordinates bounce_coordinates = Coordinates(parameters_[1]);
  
  Square* bounce_square = board->getSquare(bounce_coordinates);

  if(bounce_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  if(!(parameters_[2][0] >= 'a' && parameters_[2][0] <= 'h') && !(parameters_[2][1] >= '1' && parameters_[2][1] <= '8'))
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  Coordinates target_coordinates = Coordinates(parameters_[2]);
  
  Square* target_square = board->getSquare(target_coordinates);

  if(target_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  int current_file = static_cast<int>(current_piece->getCoordinates().getFile());
  int current_rank = static_cast<int>(current_piece->getCoordinates().getRank());
  int target_file = static_cast<int>(target_coordinates.getFile());
  int target_rank = static_cast<int>(target_coordinates.getRank());
  int bounce_file = static_cast<int>(bounce_coordinates.getFile());
  int bounce_rank = static_cast<int>(bounce_coordinates.getRank());

  if(bounce_file != 0 && bounce_file != 7 && bounce_rank != 0 && bounce_rank != 7)
  {
    printErrorMessage("E_INV_PARAM_SPECIAL_SQUARE");
    return false;
  }

  if(bounce_square->getPiece() != nullptr)
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  if((target_file == current_file && target_rank == current_rank) ||
  (target_file == bounce_file && target_rank == bounce_rank) ||
  (current_file == bounce_file && current_rank == bounce_rank))
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  if((abs(target_file - bounce_file)) != abs((target_rank - bounce_rank)) ||
  (abs(bounce_file - current_file) != abs(bounce_rank - current_rank)))
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  if(target_square->getPiece() != nullptr && target_square->getPiece()->getColor() == player->getColor())
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  if(!(canMoveDiagonal(board, bounce_file, bounce_rank, current_file, current_rank, false)) ||
  !(canMoveDiagonal(board, target_file, target_rank, bounce_file, bounce_rank, true)))
  {
    printErrorMessage("E_INVALID_MOVE");
    return false;
  }

  current_piece->setTargetSquare(target_square);
  current_piece->setBounceSquare(bounce_square);
  return true;
}

bool SpecialCommand::canMoveDiagonal(Board* board, size_t target_file, size_t target_rank, size_t piece_file, size_t piece_rank, bool bounce)
{
  if(bounce)
  {
    if((piece_file == 0 && piece_rank == 0) || (piece_file == 0 && piece_rank == 7) ||
    (piece_file == 7 && piece_rank == 0) || (piece_file == 7 && piece_rank == 7))
      return false;
    if(piece_file == 0 && target_file <= piece_file)
        return false;
    if(piece_file == 7 && target_file >= piece_file)
        return false;
    if(piece_rank == 0 && target_rank <= piece_rank)
        return false;
    if(piece_rank == 7 && target_rank >= piece_rank)
        return false;
  }
  // from Claude
  // begin
  int file_step = (target_file > piece_file) ? 1 : -1;
  int rank_step = (target_rank > piece_rank) ? 1 : -1;
  int current_file = static_cast<int>(piece_file) + file_step;
  int current_rank = static_cast<int>(piece_rank) + rank_step;

  while(current_file != static_cast<int>(target_file) && current_rank != static_cast<int>(target_rank))
  {
    if(board->getSquare(Coordinates(current_file, current_rank))->getPiece() != nullptr)
    {
      return false;
    }
    current_file += file_step;
    current_rank += rank_step;
  }
    //end
  return true;
}

void SpecialCommand::changeOwner(Piece* current_piece, Player* active_player, Player* opponent_player)
{
  Piece* piece = current_piece->getTargetSquare()->getPiece();

  std::vector<std::unique_ptr<Piece>>& active_pieces = active_player->getPieces();
  std::vector<std::unique_ptr<Piece>>& opponent_pieces = opponent_player->getPieces();

  auto current = opponent_pieces.begin();
  while(current != opponent_pieces.end())
  {
    if(current->get() == piece)
    {
      active_pieces.push_back(std::move(*current));
      current = opponent_pieces.erase(current);
    }
    else
    {
      current++;
    }
  }

  piece->setOwner(active_player);
}

bool SpecialCommand::checkKnightMove(size_t piece_file, size_t piece_rank, size_t target_file, size_t target_rank)
{
  std::size_t file_dif = std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file));
  std::size_t rank_dif = std::abs(static_cast<int>(piece_rank) - static_cast<int>(target_rank));

  if(file_dif == 1 && rank_dif == 2)
  {
    return true;
  }
  if(file_dif == 2 && rank_dif == 1)
  {
    return true;
  }

  return false;
}

bool SpecialCommand::checkPath(Piece* current_piece, Player* player, Coordinates target_coordinates, Board* board)
{
  size_t archer_file = current_piece->getCoordinates().getFile();
  size_t archer_rank = current_piece->getCoordinates().getRank();

  size_t target_file = target_coordinates.getFile();
  size_t target_rank = target_coordinates.getRank();

  if(target_file != archer_file)
  {
    printErrorMessage("E_INVALID_ARCHER_TARGET");
    return false;
  }

  if(player->getColor() == PlayerColor::WHITE && target_rank < archer_rank)
  {
    printErrorMessage("E_INVALID_ARCHER_TARGET");
    return false;
  }

  if(player->getColor() == PlayerColor::BLACK && target_rank > archer_rank)
  {
    printErrorMessage("E_INVALID_ARCHER_TARGET");
    return false;
  }

  if(board->getSquare(target_coordinates)->getPiece() != nullptr &&
  board->getSquare(target_coordinates)->getPiece()->getColor() == player->getColor())
  {
    printErrorMessage("E_INVALID_ARCHER_TARGET");
    return false;
  }

  return true;

}

bool SpecialCommand::colorBlindBishop(Piece* current_piece, Square* target_square, size_t target_file, 
size_t target_rank, size_t current_file, size_t current_rank)
{
    int file_difference = static_cast<int>(target_file) - static_cast<int>(current_file);
    int rank_difference = static_cast<int>(target_rank) - static_cast<int>(current_rank);
    if(abs(file_difference) <= 1 && abs(rank_difference) <= 1 && !(file_difference == 0 && rank_difference == 0))
    {
        if(target_square->getPiece() != nullptr && current_piece->getColor() == target_square->getPiece()->getColor())
        {
            printErrorMessage("E_INVALID_MOVE");
            return false;
        }
    }
    else
    {
        printErrorMessage("E_INVALID_MOVE");
        return false;
    }
    return true;
}

bool SpecialCommand::explosivePawn(Piece* current_piece, Board* board, Square* target_square, Coordinates target_coordinates)
{
    if(target_square->getPiece() == nullptr)
    {
        printErrorMessage("E_INVALID_MOVE");
        return false;
    }
    if(!(current_piece->canMoveToTargetSquare(*board, target_coordinates, true)))
    {
        printErrorMessage("E_INVALID_MOVE");
        return false;
    }
    return true;
}

bool SpecialCommand::jumpyQueen(Piece* current_piece, Square* target_square, size_t target_file,
size_t target_rank, size_t current_file, size_t current_rank)
{
    if(!(checkKnightMove(current_file, current_rank, target_file, target_rank)))
    {
        printErrorMessage("E_INVALID_MOVE");
        return false;
    }
    else if(target_square->getPiece() != nullptr && target_square->getPiece()->getColor() == current_piece->getColor())
    {
        printErrorMessage("E_INVALID_MOVE");
        return false;
    }
    return true;
}
