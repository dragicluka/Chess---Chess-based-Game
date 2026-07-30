#include "Game.hpp"
#include "Command.hpp"
#include <memory>
#include <cmath>
#include "SpawnSquare.hpp"

const std::string INVALID_FILE_MESSAGE = "Error: Invalid file (";
const std::size_t K = 32;
const std::string COMPLETED_DRAW = "you both agreed to a draw.";
const std::string NORMAL_WIN = "This was a great match! Well done to you both!\n";
const std::string RESIGNATION_WIN = "Oh I see one of you couldn't take the pressure...\n";
const std::string STALEMATE_DRAW = "of stalemate.";
const std::string KING_DRAW_MESSAGE = "you both lost your king.";
const std::string TURN_DRAW = "too many turns were played.";

Game::Game() : 
    command_line_(), board_(), 
    white_player_(PlayerColor::WHITE, 0, 0, 0), black_player_(PlayerColor::BLACK, 0, 0, 0),
    active_player_(nullptr), opponent_player_(nullptr), is_running_(true), final_message_(""), elo_message_("") {}



bool Game::checkMagicNumber(const std::string &file_path, const std::string& magic_number) 
{
  std::ifstream file(file_path);

  if(!file.is_open())
  {
    std::cout << INVALID_FILE_MESSAGE << file_path<<")!\n";
    return false;
  }

  std::string line;

  std::getline(file, line);

  if(line != magic_number)
  {
    std::cout << INVALID_FILE_MESSAGE << file_path<<")!\n";
    file.close();
    return false;
  }

  return true;
}



void Game::loadGameConfigFile(std::string &game_file_path) 
{
  std::ifstream game_file(game_file_path);

  if(!game_file.is_open())
  {
    std::cout << INVALID_FILE_MESSAGE << game_file_path<<")!\n";
    return;
  }

  parseGameSettings(game_file);

  parsePieces(game_file);

  parseSquares(game_file);
}


void Game::loadMessageConfigFile(std::string &message_file_path)
{
  std::ifstream message_file(message_file_path);

  if(!message_file.is_open())
  {
    std::cout << INVALID_FILE_MESSAGE << message_file_path<<")!\n";
    return;
  }

  std::string line;
  std::getline(message_file, line);
  std::map<std::string, std::string> error_messages;
  std::map<std::string, std::string> messages;

  while(std::getline(message_file, line))
  {
    if(line.empty())
      continue;

    bool is_trimmed = false;  
    std::vector<std::string> split_string = Utils::splitString(line, ':', is_trimmed);
    
    if(split_string.at(0)[0] == 'E')
    {
      std::string error_message = "[ERROR] " + split_string.at(1) + "\n" + "\n";
      error_messages.insert({split_string.at(0), error_message});
    }

    if(split_string.at(0)[0] == 'D')
    {
      std::string message = split_string.at(1) + "\n";
      if(split_string.at(0) == "D_CHESSBOARD_BORDER")
      {
        board_.setChessboardBorder(message);
      }

      if(split_string.at(0) == "D_BORDER_D")
      {
        board_.setBorderDecoration(message);
      }
      
      if(split_string.at(0) == "D_AMBIGUOUS_MOVE")
      {
        message += "\n";
      }
      messages.insert({split_string.at(0), message});
      
    }
  }

  command_line_.setErrorMessages(error_messages);
  command_line_.setMessages(messages);
}


void Game::start() 
{
  printWelcomeMessage();

  active_player_ = &white_player_;
  opponent_player_ = &black_player_;
  setPiecesOnBoard();
  bool new_round = true;
  while(is_running_)
  {
    if(new_round && !handleNewTurn())
      break;

    if(board_.getPrintBoard() && new_round)
    {
      board_.printBoard(active_player_, opponent_player_);
    }
    if(!board_.getPrintBoard())
      std::cout << std::endl;
    
    command_line_.printPrompt(active_player_);
    std::unique_ptr<Command> command;
    try
    {
      command = command_line_.getCommand();
    }
    catch(const std::invalid_argument& error)
    {
      std::cout << error.what();
      new_round = false;
      continue;
    }

    if(command == nullptr)
    {
      new_round = false;
      continue;
    }
    CommandReturnType command_return_type = command->execute(board_, *active_player_, *opponent_player_);

    Player* temp = active_player_; // mora van switch case, treba mi za active success
    switch (command_return_type)
    {
    case CommandReturnType::QUIT:
      is_running_ = false;
      break;

    case CommandReturnType::PASSIVE_SUCCESS:
      new_round = false;
      continue;

    case CommandReturnType::ACTIVE_SUCCESS:
      handleActiveSuccess(new_round, temp);
      continue;

    case CommandReturnType::ERROR:
      new_round = false;
      continue;
    
    case CommandReturnType::WIN:
      active_player_->setWon();
      endGamewithWin(NORMAL_WIN, active_player_, opponent_player_, false);
      finishGame();
      break;
    
    case CommandReturnType::RESIGN_NORMAL_WIN:
      opponent_player_->setWon();
      endGamewithWin(NORMAL_WIN, opponent_player_, active_player_, false);
      finishGame();
      break;
    
    case CommandReturnType::RESIGN_WIN:
      opponent_player_->setWon();
      endGamewithWin(RESIGNATION_WIN, opponent_player_, active_player_, true);
      finishGame();
      break;
    
    case CommandReturnType::RESIGN_DRAW:
      setBothPlayersToDraw();
      endGameWithDraw(STALEMATE_DRAW);
      finishGame();
      break;
    
    case CommandReturnType::KING_DRAW:
      setBothPlayersToDraw();
      endGameWithDraw(KING_DRAW_MESSAGE);
      finishGame();
      break;

    case CommandReturnType::DRAW:
      setBothPlayersToDraw();
      endGameWithDraw(COMPLETED_DRAW);
      finishGame();
      break;
    }
    is_running_ = false;
    continue;
  }
}

void Game::handleActiveSuccess(bool& new_round, Player* temp)
{
  if(active_player_->getCanMoveMultiple())
  {
    active_player_->setNumberOfMoves(active_player_->getNumberOfMoves() + 1);
    active_player_->setCanMoveMultiple(false);
    new_round = false;

    if(board_.getPrintBoard())
      board_.printBoard(active_player_, opponent_player_);

    return;
  }
  new_round = true;
  active_player_->setNumberOfMoves(0);
  active_player_->setLastMovedPiece(nullptr);
  active_player_ = opponent_player_;
  opponent_player_ = temp;
}


void Game::endGameWithDraw(const std::string& cause_string) 
{
  final_message_ = "This game ended in a draw because " + cause_string + "\n";
  final_message_ += "Thank you for playing " + std::to_string(board_.getRound()) + " turns." + "\n";

  std::cout << final_message_;
}


void Game::endGamewithWin(const std::string& cause_string, Player* winner, Player* loser, bool is_resign_win) 
{
  if(is_resign_win)
  {
    final_message_ = cause_string;
    final_message_ += "Well done to " + winner->getID() + " for making " + loser->getID() + " resign in " + 
    std::to_string(board_.getRound()) + " turns.\n";
  }
  else
  {
    final_message_ = cause_string;
    final_message_ += "Good job to " + winner->getID() + " for winning in " + std::to_string(board_.getRound()) +
    " turns.\n";
  }
  
  std::cout << final_message_;
}


void Game::printWelcomeMessage(){
  command_line_.printMessage("D_BORDER_D");
  command_line_.printMessage("D_WELCOME");
  command_line_.printMessage("D_BORDER_D");
}

void Game::setPiecesOnBoard()
{
  placeFrontRow();

  std::size_t white_placed = 0;
  std::size_t black_placed = 0;
  std::size_t move_counter = 0;
  while(white_placed < 8 || black_placed < 8)
  {
    Player* current_player = &white_player_;
    std::size_t* current_index = &white_placed;
    if(move_counter % 4 == 1 || move_counter % 4 == 2)
    {
      current_player = &black_player_;
      current_index = &black_placed;
    }
    
    if(*current_index >= 8) // ako su postavljene skip
    {
      move_counter++;
      continue;
    }
    Piece* current_piece = current_player->getPieces()[*current_index + 8].get();
    std::size_t piece_counter = 0;
    for( std::size_t i = *current_index; i < 8 ; i++)
    {
      if(current_piece->getId() == current_player->getPieces()[i + 8].get()->getId())
      {
        piece_counter++;
      }
    }
    std::cout << "Where do you want to place "<< current_piece->getIdString(current_piece->getId())
    << " (" << piece_counter << " remaining)?"<<std::endl;
    std::cout <<current_player->getID()<<" > ";

    bool placed = placingInput(current_player, current_index, current_piece);
    if(is_running_ == false)
    {
      return;
    }
    if(placed == false)
    {
      continue;
    }
   (*current_index)++;
    move_counter++;
    }
  
}

void Game::placeFrontRow()
{
  std::size_t placed_front_row_counter = 0;
  for(int i = 0; i < 8; i++)
  {
    Piece* white_current_piece = white_player_.getPieces()[i].get();
    Piece* black_current_piece = black_player_.getPieces()[i].get();
    board_.getSquare(Coordinates(placed_front_row_counter,1))->setPiece(std::move(white_current_piece));
    board_.getSquare(Coordinates(placed_front_row_counter ,6))->setPiece(std::move(black_current_piece));
    white_current_piece->getCoordinates().setRank(1);
    white_current_piece->getCoordinates().setFile(placed_front_row_counter);
    black_current_piece->getCoordinates().setRank(6);
    black_current_piece->getCoordinates().setFile(placed_front_row_counter);
    placed_front_row_counter++;
  }
}

bool Game::placingInput(Player* current_player, std::size_t* current_index, Piece* current_piece)
{
  std::string input;
  std::getline(std::cin, input);
  Utils::toLowerCase(input);
  bool is_trimmed = true;
  Utils::trim(input);
  std::vector<std::string> user_input = Utils::splitString(input, ' ', is_trimmed);

  if(user_input.empty())
  {
    command_line_.printErrorMessage("E_INV_PARAM_SQUARE");
    return false;
  }
  if(user_input[0] == "quit")
  {
    is_running_ = false;
    return false;
  }
  
  if( user_input[0] == "auto" && user_input.size() == 1)
  {
    autoCommand(current_player, *current_index);
    *current_index = 8;
    return true;
  }

  std::size_t target_file = user_input[0][0] - 'a';
  std::size_t target_rank = user_input[0][1] - '1';
  if((current_player->getID() == "White" && target_rank !=0 ) || (current_player->getID() == "Black"
    && target_rank !=7) || target_rank > 7 || target_rank < 0 ||
    board_.getSquare(Coordinates(target_file,target_rank))->getPiece() != nullptr)
  {
    command_line_.printErrorMessage("E_INV_PARAM_SQUARE");
    return false;
  }
  board_.getSquare(Coordinates(target_file,target_rank))->setPiece(std::move(current_piece));
  current_piece->getCoordinates().setFile(target_file);
  current_piece->getCoordinates().setRank(target_rank);
  if(current_piece->getTypeString() == "b")
  {
    SquareType type =  board_.getSquare(Coordinates(target_file,target_rank))->getSquareType();
    if(type == SquareType::WHITE)
    {
      current_piece->setColor(SquareType::WHITE);
    }
    else if(type == SquareType::BLACK)
    {
      current_piece->setColor(SquareType::BLACK);
    }
    else
    {
      if(current_player->getColor() == PlayerColor::WHITE)
      {
        current_piece->setColor(SquareType::WHITE);
      }
      else
      {
        current_piece->setColor(SquareType::BLACK);
      }
    }
  }
  return true;
}

void Game::autoCommand(Player* current_player, std::size_t current_index)
{
  std::size_t rank;
  if( current_player->getID() == "White")
  {
    rank = 0;
  }
  else
  {
    rank = 7;
  }
  for( int i = current_index; i < 8; i++)
  {
    Piece* current_piece = current_player->getPieces()[i + 8].get();
    for(int j = 0 ; j < 8; j++)
    {
      Square* target_square = board_.getSquare(Coordinates(j,rank));
      if(target_square->getPiece() == nullptr)
      {
        target_square->setPiece(std::move(current_piece));
        current_piece->getCoordinates().setFile(j); 
        current_piece->getCoordinates().setRank(rank);
         if(current_piece->getTypeString() == "b")
          {
            SquareType type =  target_square->getSquareType();
            if(type == SquareType::WHITE)
            {
              current_piece->setColor(SquareType::WHITE);
            }
            else if(type == SquareType::BLACK)
            {
              current_piece->setColor(SquareType::BLACK);
            }
            else
            {
              if(current_player->getColor() == PlayerColor::WHITE)
              {
                current_piece->setColor(SquareType::WHITE);
              }
              else
              {
                current_piece->setColor(SquareType::BLACK);
              }
            }
          }
        break;
      }
    }
  }
}



void Game::calculateElo()
{
  std::size_t Rw = white_player_.getElo();
  std::size_t Rb = black_player_.getElo();
  
  double Sw = white_player_.getActualScore();
  double Sb = black_player_.getActualScore();

  double Qw = std::pow(10.0, static_cast<double>(Rw) / 400);
  double Qb = std::pow(10.0, static_cast<double>(Rb) / 400);

  double Ew = Qw / (Qw + Qb);
  double Eb = 1 - Ew;

  double new_Rw_double = Rw + K * (Sw - Ew);
  double new_Rb_double = Rb + K * (Sb - Eb);

  std::size_t new_Rw = (std::size_t)std::floor(new_Rw_double);
  std::size_t new_Rb = (std::size_t)std::floor(new_Rb_double);

  white_player_.setElo(new_Rw);
  black_player_.setElo(new_Rb);

  printNewEloScores();
}



void Game::printNewEloScores()
{
  elo_message_ = "\n";
  elo_message_ += "The new Elo scores are:\n";
  elo_message_ += " - White: " + std::to_string(white_player_.getElo()) + "\n";
  elo_message_ += " - Black: " + std::to_string(black_player_.getElo()) + "\n";
  elo_message_ += "\n";
  
  std::cout << elo_message_;
}

void Game::takeInvincible()
{
  std::vector<std::unique_ptr<Piece>>& active_array = active_player_->getPieces();
  for(auto& piece : active_array)
  {
    if(piece.get()->getInvincible() != -1)
    {
      piece.get()->takeInvinicible();
    }
  }
  std::vector<std::unique_ptr<Piece>>& opponent_array = opponent_player_->getPieces();
  for(auto& piece : opponent_array)
    {
    if(piece.get()->getInvincible() != -1)
    {
      piece.get()->takeInvinicible();
    }
  }
}



void Game::createOutputFile()
{
  std::cout << "Enter the output file name" << std::endl;
  std::cout << " > ";

  std::string line;
  std::getline(std::cin, line);

  if(line.empty())
    return;
  
  std::ofstream output_file(line);

  while(!output_file.is_open())
  {
    command_line_.printErrorMessage("E_INVALID_PATH");
    std::cout << "Enter the output file name" << std::endl;
    std::cout << " > ";
    
    std::getline(std::cin, line);
    if(line.empty())
      return;
    
    output_file.open(line);
  }

  output_file << final_message_;
  output_file << elo_message_;
  output_file << command_line_.getMessages().at("D_BORDER_HISTORY") << "\n";
  output_file << command_line_.getMessages().at("D_HISTORY_HEADER");
  board_.printMoveHistory(output_file);
  output_file << command_line_.getMessages().at("D_BORDER_D");
}



void Game::parseGameSettings(std::ifstream& game_file)
{
  std::string line;

  for(std::size_t index = 0; index < 2; index++)
    std::getline(game_file, line);

  std::string turn_info;
  std::getline(game_file, turn_info);

  std::size_t max_turn_count = std::stoi(turn_info.substr(turn_info.find(':') + 2));
  board_.setMaxTurnCount(max_turn_count);

  std::string mana_info;
  std::getline(game_file, mana_info);

  std::size_t initial_mana = std::stoi(mana_info.substr(mana_info.find(':') + 2, mana_info.find('/')));
  std::size_t mana_pool_size = std::stoi(mana_info.substr(mana_info.find('/') + 1));
  white_player_.setMana(initial_mana);
  black_player_.setMana(initial_mana);
  white_player_.setManaPoolSize(mana_pool_size);
  black_player_.setManaPoolSize(mana_pool_size);
}



void Game::parsePieces(std::ifstream& game_file)
{
  std::string line;

  std::getline(game_file, line);

  for(int index = 0; index < 2; index++)
  {
    Player* current_player = nullptr;

    if(index == 0)
      current_player = &white_player_;

    else
      current_player = &black_player_;

    PlayerColor player_color = (index == 0) ? PlayerColor::WHITE : PlayerColor::BLACK;

    std::string elo_info;
    std::getline(game_file, elo_info);

    std::size_t elo = std::stoi(elo_info.substr(elo_info.find('=') + 2, elo_info.find(')')));
    current_player->setElo(elo);

    std::getline(game_file, line);
    
    std::string piece_front_rank_info;
    std::getline(game_file, piece_front_rank_info);
    
    std::vector<std::string> front_rank_ids = Utils::parseIdList(piece_front_rank_info);
    std::vector<std::unique_ptr<Piece>> front_rank_pieces;

    for(std::string& front_id : front_rank_ids)
    {
      auto piece = PieceFactory::makePiece(front_id, player_color, Coordinates(0, 0));

      if(player_color == PlayerColor::WHITE)
        piece->setOwner(&white_player_);
      else if(player_color == PlayerColor::BLACK)
        piece->setOwner(&black_player_);

      front_rank_pieces.push_back(std::move(piece));
    }

    std::string piece_back_rank_info;
    std::getline(game_file, piece_back_rank_info);

    std::vector<std::string> back_rank_ids = Utils::parseIdList(piece_back_rank_info);
    std::vector<std::unique_ptr<Piece>> back_rank_pieces;

    for(std::string& back_id : back_rank_ids)
    {
      auto piece = PieceFactory::makePiece(back_id, player_color, Coordinates(0, 0));

      if(player_color == PlayerColor::WHITE)
        piece->setOwner(&white_player_);
      else if(player_color == PlayerColor::BLACK)
        piece->setOwner(&black_player_);

      back_rank_pieces.push_back(std::move(piece));
    }

    for(auto& back_piece : back_rank_pieces)
    {
      front_rank_pieces.push_back(std::move(back_piece));
    }

    current_player->setPieces(std::move(front_rank_pieces));

    for(std::size_t i = 0; i < 2; i++)
      std::getline(game_file, line);
  }

  for(std::size_t index = 0; index < 2; index++)
    std::getline(game_file, line);
}



void Game::parseSquares(std::ifstream& game_file)
{
  std::string line;

  std::getline(game_file, line);
  Utils::trim(line);

  while (line != "}")
  {
    if(line.empty())
    {
      std::getline(game_file, line);
      Utils::trim(line);
      continue;
    }
      
    std::string square_coordinates = line.substr(0, line.find(':'));
    Coordinates coordinates = Coordinates(square_coordinates);

    std::getline(game_file, line);

    std::string square_id = line.substr(line.find(':') + 2);

    std::unique_ptr<Square> special_square = SquareFactory::makeSquare(square_id, coordinates);
    board_.setSquare(coordinates, std::move(special_square));

    if(square_id == "SPAWN")
    {
      std::getline(game_file, line);
      std::string item_list_info = line.substr(line.find(':') + 2);
      std::vector<std::string> item_ids = Utils::parseIdList(item_list_info);
      std::vector<std::unique_ptr<Item>> spawn_items;

      for(auto& item_id : item_ids)
      {
        spawn_items.push_back(ItemFactory::makeItem(item_id));
      }

      Square* square = board_.getSquare(coordinates);
      SpawnSquare* spawn_square = dynamic_cast<SpawnSquare*>(square);
      spawn_square->setItems(std::move(spawn_items));
    }

    std::getline(game_file, line);
    Utils::trim(line);
  }
}



void Game::finishGame()
{
  calculateElo();
  createOutputFile();
  is_running_ = false;
}



void Game::setBothPlayersToDraw()
{
  active_player_->setDraw();
  opponent_player_->setDraw(); 
}



bool Game::handleNewTurn()
{
  if(active_player_->getColor() == PlayerColor::WHITE)
  {
    takeInvincible();
    board_.setNewRound();
    if(board_.getRound() == board_.getMaxTurnCount())
    {
      setBothPlayersToDraw();
      endGameWithDraw(TURN_DRAW);
      finishGame();
      return false;
    }
  }

  for(auto& piece : active_player_->getPieces())
  {
    piece->unfreeze();
  }

  for(auto& piece : active_player_->getPieces())
  {
    piece->unclone();
    Coordinates coordinates = piece->getCoordinates();
    Square* square = board_.getSquare(coordinates);

    if(piece->getClonedFor() == 0)
    {
      square->removePiece();
    }
  }

  board_.triggerSpecialSquares(active_player_);
  active_player_->setMana(active_player_->getMana() + 1);
  return true;
}