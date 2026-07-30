#include "Command.hpp"
#include "Player.hpp"
#include "Utils.hpp"
#include "PieceFactory.hpp"
#include "CommandLine.hpp"
#include "Board.hpp"
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <random>


void Command::printErrorMessage(const std::string& key)
{
  std::cout << error_messages_.at(key);
}

void Command::printMessage(const std::string& key)
{
  std::cout << messages_.at(key);
}

CommandReturnType HelpCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void)board;
  (void)active_player;
  (void)opponent_player;

  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  std::cout << "=== Commands ============================================================================"<< std::endl;
  std::cout << "- help" << std::endl << "    Prints this help text." << std::endl << std::endl;
  std::cout << "- quit" << std::endl << "    Terminates the game." << std::endl << std::endl;
  std::cout << "- board" << std::endl << "    Toggles the board printing." << std::endl << std::endl;
  std::cout << "- info <PIECE_ID>" << std::endl << "    Prints piece information." << std::endl;
  std::cout << "    <PIECE_ID>: The piece ID to be explained." << std::endl << std::endl;
  std::cout << "- history" << std::endl << "    Prints the move history in modified chess notation." << std::endl;
  std::cout << std::endl << "- prison <PLAYER_ID>" << std::endl << "    Lists pieces captured by the specified player.";
  std::cout << std::endl << "    <PLAYER_ID>: [White/Black]" << std::endl << std::endl;
  std::cout << "- pass" << std::endl << "    Ends the current player's turn after a move or special ability.";
  std::cout << std::endl << std::endl << "- draw" << "\n" << "    Offers a draw to the opponent." << std::endl 
    << std::endl;
  std::cout << "- resign" << std::endl << "    Resigns the game (loss)." << std::endl << std::endl;
  std::cout << "- move <MOVE>" << std::endl << "    Moves a piece using simplified chess notation." << std::endl;
  std::cout << "    <MOVE>: a move in the simplified chess notation format." << std::endl << std::endl;
  std::cout << "- use <SQUARE> [...]" << std::endl << "    Uses a potion." << std::endl;
  std::cout << "    <SQUARE>: The location of the piece, whose potion will be used." << std::endl;
  std::cout << "    [...]: Variable amount of parameters depending on the potion." << std::endl << std::endl;
  std::cout << "- special <SQUARE> [...]" << std::endl << "    Activates a piece's special ability." << std::endl;
  std::cout << "    <SQUARE>: The square where the special piece is located." << std::endl;
  std::cout << "    [...]: Variable parameters depending on the piece (use info for a piece by piece description).";
  std::cout << std::endl << std::endl;
  std::cout << "=========================================================================================" << std::endl;
  std::cout << std::endl;

  return CommandReturnType::PASSIVE_SUCCESS;
}


CommandReturnType DrawCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void) board;
  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  std::cout << "Player " << active_player.getID() << " has offered a draw. Would you like to accept? (yes/no)" << 
  std::endl;

  std::cout << opponent_player.getID() << " > ";

  std::string opponent_answer;
  std::getline(std::cin, opponent_answer);
  Utils::trim(opponent_answer);
  Utils::toLowerCase(opponent_answer);

  if(opponent_answer == "yes")
  {
    return CommandReturnType::DRAW;
  }

  else if(opponent_answer == "no")
  {
    std::cout << std::endl;
    return CommandReturnType::PASSIVE_SUCCESS;
  }

  else
  {
    printErrorMessage("E_INV_PARAM_YES_NO");
    return CommandReturnType::ERROR;
  }
}


CommandReturnType HistoryCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void)active_player;
  (void)opponent_player;

  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }
  printMessage("D_BORDER_HISTORY");
  std::cout << std::endl;
  printMessage("D_HISTORY_HEADER");
  board.printMoveHistory();
  printMessage("D_BORDER_D");
  std::cout << std::endl;
  return CommandReturnType::PASSIVE_SUCCESS;
}


CommandReturnType InfoCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void) board;
  (void) active_player;
  (void) opponent_player;
  if(parameters_.size() != 1)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }


  std::string argument = parameters_[0];
  std::transform(argument.begin(), argument.end(), argument.begin(), ::toupper);
  std::unique_ptr<Piece> piece = PieceFactory::makePiece(argument, PlayerColor::BLACK, Coordinates(0,0));


  if(piece == nullptr)
  {
    printErrorMessage("E_INV_PARAM_PIECE");
    return CommandReturnType::ERROR;
  }


  printMessage("D_BORDER_INFO_B");
  std::cout<<"["<< piece->getIdString(piece->getId())<<" | "<<piece->getShortName()<<"] ";

  std::string key = "D_N_" + piece->getIdString(piece->getId());
  printMessage(key);

  if(piece->getPowerType() == PowerType::ACTIVE)
  {
    if (piece->getManaCost() == 0)
    {
      std::cout<< "Mana: "<<"XX"<< std::endl;
    }
    else
    {
      std::cout<< "Mana: " << std::right<< std::setfill('0') << std::setw(2) << piece->getManaCost() << std::endl;
    }
  }
  
  std::cout<<"Description: ";
  key ="D_I_" + piece->getIdString(piece->getId());
  printMessage(key);

  std::cout<<"Special: ";
  if(piece->getPowerType() == PowerType::ACTIVE)
  {
    key = "D_S_"+ piece->getIdString(piece->getId());
    printMessage(key);
  }
  else
  {
    std::cout<<"None\n";
  }

  printMessage("D_BORDER_INFO_E");
  std::cout << std::endl;
  return CommandReturnType::PASSIVE_SUCCESS;
}


CommandReturnType PassCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void)board;
  (void)opponent_player;

  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  if(active_player.getNumberOfMoves() == 0)
  {
    printErrorMessage("E_INVALID_PASS");
    return CommandReturnType::ERROR;
  }

  active_player.setCanMoveMultiple(false);
  return CommandReturnType::ACTIVE_SUCCESS;
}


CommandReturnType PrisonCommand::execute(Board& board, Player& active_player, Player& opponent_player) 
{
  (void) board;
  (void) opponent_player;

  if(parameters_.size() != 1)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  std::string id = parameters_[0];

  if(id != "white" && id != "black")
  {
    printErrorMessage("E_INV_PARAM_PLAYER");
    return CommandReturnType::ERROR;
  }

  std::string active_id = active_player.getID();
  Utils::toLowerCase(active_id);
  std::string player_id = "";

  if(id == active_id)
  {
    player_id = active_player.getID();
  }
  else
  {
    player_id = opponent_player.getID();
  }

  printMessage("D_BORDER_PRISON");
  std::cout << player_id << ":" << std::endl;
  
  if(id == active_id)
    opponent_player.printPrison(); 
  else
    active_player.printPrison();

  std::cout << "\n";
  printMessage("D_BORDER_D");
  std::cout << "\n";

  return CommandReturnType::PASSIVE_SUCCESS;
}


CommandReturnType QuitCommand::execute(Board& board, Player& active_player, Player& opponent_player) 
{
  (void)board;
  (void)active_player;
  (void)opponent_player;

  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  return CommandReturnType::QUIT;
}


CommandReturnType ResignCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  if(active_player.checkForStalemate(&opponent_player, &board))
    return CommandReturnType::RESIGN_DRAW;

  else if(active_player.checkForCheckmate(&opponent_player, &board))
    return CommandReturnType::RESIGN_WIN;
  
  else
    return CommandReturnType::RESIGN_WIN;
}


CommandReturnType BoardCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  if(parameters_.size() != 0)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }
  if(board.getPrintBoard())
  {
    board.setPrintBoard(false);
  }
  else
  {
    board.setPrintBoard(true);
    board.printBoard(&active_player,&opponent_player);
  }
  return CommandReturnType::PASSIVE_SUCCESS;
}


/*CommandReturnType FuseCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void)opponent_player;
  if(parameters_.size() != 2)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  std::string first_square_coordinates_string = parameters_.at(0);
  std::string second_square_coordinates_string = parameters_.at(1);

  Coordinates first_square_coordinates = Coordinates(first_square_coordinates_string);
  Coordinates second_square_coordinates = Coordinates(second_square_coordinates_string);

  Square* first_square = board.getSquare(first_square_coordinates);
  Square* second_square = board.getSquare(second_square_coordinates);

  if(first_square == nullptr || second_square == nullptr)
  {
    printErrorMessage("E_INV_PARAM_SQUARE");
    return CommandReturnType::ERROR;
  }

  Piece* piece_on_first_square = first_square->getPiece();
  Piece* piece_on_second_square = second_square->getPiece();

  if(piece_on_first_square == nullptr || piece_on_second_square == nullptr)
  {
    printErrorMessage("E_PLAYER_PIECE_NOT_FOUND");
    return CommandReturnType::ERROR;
  }

  if(piece_on_first_square->getColor() != active_player.getColor() || piece_on_second_square->getColor() != 
  active_player.getColor())
  {
    printErrorMessage("E_PLAYER_PIECE_NOT_FOUND");
    return CommandReturnType::ERROR;   
  }

  if(piece_on_first_square->getPieceType() != piece_on_second_square->getPieceType())
  {
    printErrorMessage("E_INVALID_FUSE");
    return CommandReturnType::ERROR;
  }

  std::size_t file_dif = std::abs((int)piece_on_first_square->getCoordinates().getFile() - 
  (int)piece_on_second_square->getCoordinates().getFile());

  std::size_t rank_dif = std::abs((int)piece_on_first_square->getCoordinates().getRank() - 
  (int)piece_on_second_square->getCoordinates().getRank()); 

  bool adjacent = (file_dif <= 1 && rank_dif <= 1) && !(file_dif == 0 && rank_dif == 0);

  if(!adjacent)
  {
    printErrorMessage("E_INVALID_FUSE");
    return CommandReturnType::ERROR;    
  }

  PieceType piece_type = piece_on_first_square->getPieceType();

  first_square->removePiece();

  if(piece_on_first_square->getItem() != nullptr)
  {
    piece_on_first_square->removeItem();
  }

  second_square->removePiece();

  if(piece_on_second_square->getItem() != nullptr)
  {
    piece_on_second_square->removeItem();
  }

  std::vector<std::unique_ptr<Piece>>& pieces = active_player.getPieces();

  auto iterator = pieces.begin();
  while(iterator != pieces.end())
  {
    if(iterator->get() == piece_on_first_square || iterator->get() == piece_on_second_square)
    {
      iterator = pieces.erase(iterator);  // erase vraća sljedeći
    }
    else
    {
      iterator++;  // samo ako NISI brisao
    }
  }

  if(piece_type == PieceType::P)
  {
    std::unique_ptr<Piece> new_knight = PieceFactory::makePiece("N", active_player.getColor(), first_square_coordinates);
    new_knight->setOwner(&active_player);

    Piece* raw_knight = new_knight.get();

    active_player.getPieces().push_back(std::move(new_knight));  

    first_square->setPiece(raw_knight); 
  }

  if(piece_type == PieceType::N)
  {
    std::unique_ptr<Piece> new_bishop = PieceFactory::makePiece("B", active_player.getColor(), first_square_coordinates);
    new_bishop->setOwner(&active_player);

    Piece* raw_bishop = new_bishop.get();

    active_player.getPieces().push_back(std::move(new_bishop));

    first_square->setPiece(raw_bishop);
  }

  if(piece_type == PieceType::B)
  {
    std::unique_ptr<Piece> new_rook = PieceFactory::makePiece("R", active_player.getColor(), first_square_coordinates);

    new_rook->setOwner(&active_player);

    Piece* raw_rook = new_rook.get();

    active_player.getPieces().push_back(std::move(new_rook));

    first_square->setPiece(raw_rook);
  }

  if(piece_type == PieceType::R)
  {
    std::unique_ptr<Piece> new_queen = PieceFactory::makePiece("Q", active_player.getColor(), first_square_coordinates);

    new_queen->setOwner(&active_player);

    Piece* raw_queen = new_queen.get();

    active_player.getPieces().push_back(std::move(new_queen));

    first_square->setPiece(raw_queen);
  }

  return CommandReturnType::ACTIVE_SUCCESS;
}


CommandReturnType GambleCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  if(!parameters_.empty())
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;    
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  Piece* chosen_piece;

  do
  {
    std::vector<std::unique_ptr<Piece>>& pieces = active_player.getPieces();

    std::uniform_int_distribution<> piece_dist(0, pieces.size() - 1);

    int random_index = piece_dist(gen);

    chosen_piece = pieces.at(random_index).get();

  } while (chosen_piece->isInPrison());
  

  std::uniform_int_distribution<> file_dist(0, 7);
  std::uniform_int_distribution<> rank_dist(0, 7);

  int file_random_index = file_dist(gen);
  int rank_random_index = rank_dist(gen);

  Square* chosen_square = board.getSquare(Coordinates(file_random_index, rank_random_index));

  Square* source_square = board.getSquare(chosen_piece->getCoordinates());

  if(chosen_square->getPiece() == nullptr)
  {
    source_square->removePiece();
    chosen_square->setPiece(chosen_piece);
    chosen_piece->setCoordinates(chosen_square->getCoordinates());
    return CommandReturnType::ACTIVE_SUCCESS;
  }

  if(chosen_square->getPiece()->getColor() == chosen_piece->getColor())
  {
    std::cout << "Friendly piece already at the square" << std::endl;
    return CommandReturnType::ACTIVE_SUCCESS;
  }

  if(chosen_square->getPiece()->getColor() != chosen_piece->getColor())
  {
    Piece* opponent_piece = chosen_square->getPiece();

    if(opponent_piece->getPieceType() == PieceType::K)
    {
      return CommandReturnType::WIN;
    }

    chosen_square->removePiece();
    opponent_piece->sendToPrison();
    opponent_piece->getOwner()->addToPrison();

    chosen_square->setPiece(chosen_piece);
    chosen_piece->setCoordinates(chosen_square->getCoordinates());

    if(active_player.checkForCheckmate(&opponent_player, &board))
    {
      return CommandReturnType::WIN;
    }

    return CommandReturnType::ACTIVE_SUCCESS;
  }

  return CommandReturnType::ERROR;
}*/


CommandReturnType CloneCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
  (void)opponent_player;
  if(parameters_.size() != 2)
  {
    printErrorMessage("E_INVALID_PARAM_COUNT");
    return CommandReturnType::ERROR;
  }

  std::string source_square_string = parameters_.at(0);
  Coordinates source_square_coordinates = Coordinates(source_square_string);

  Square* source_square = board.getSquare(source_square_coordinates);

  if(source_square == nullptr)
  {
    printErrorMessage("E_INVALID_CMD");
    return CommandReturnType::ERROR;
  }

  Piece* piece_on_source_square = source_square->getPiece(); 

  if(piece_on_source_square == nullptr || piece_on_source_square->getColor() != active_player.getColor())
  {
    printErrorMessage("E_INVALID_CMD");
    return CommandReturnType::ERROR;
  }

  if(piece_on_source_square->getPieceType() == PieceType::K)
  {
    printErrorMessage("E_INVALID_CMD");
    return CommandReturnType::ERROR;
  }

  std::string target_square_string = parameters_.at(1);
  Coordinates target_square_coordinates = Coordinates(target_square_string);
  Square* target_square = board.getSquare(target_square_coordinates);

  if(target_square->isOccupied())
  {
    printErrorMessage("E_INVALID_CMD");
    return CommandReturnType::ERROR;
  }

  auto cloned_piece = PieceFactory::makePiece(piece_on_source_square->getIdString(piece_on_source_square->getId()), 
  active_player.getColor(), target_square->getCoordinates());
  cloned_piece->setOwner(&active_player);
  
  Piece* piece_cloned = cloned_piece.get();

  target_square->setPiece(piece_cloned);
  if(piece_on_source_square->getItem() != nullptr)
  {
    piece_cloned->setItem(piece_on_source_square->getItem());
  }
  
  piece_cloned->setCloned();

  active_player.getPieces().push_back(std::move(cloned_piece));
  return CommandReturnType::ACTIVE_SUCCESS;
}