#include "CommandLine.hpp"
#include "Player.hpp"


void CommandLine::printPrompt(Player* player)
{
  std::cout << player->getID() << " > ";
}


std::unique_ptr<Command> CommandLine::getCommand()
{
  std::string user_input;
  std::getline(std::cin, user_input);

  Utils::trim(user_input);

  Utils::toLowerCase(user_input);

  bool is_trimmed = true;
  std::vector<std::string> input_tokens = Utils::splitString(user_input, ' ', is_trimmed);

  if(input_tokens.empty())
  {
    printErrorMessage("E_UNKNOWN_COMMAND");
    return nullptr;
  }

  std::string command = input_tokens.at(0);
  std::vector<std::string> parameters(input_tokens.begin() + 1, input_tokens.end());

  try
  {
    if(command == "quit")
      return std::make_unique<QuitCommand>(parameters, error_messages_, messages_);
    
    else if(command == "board")
      return std::make_unique<BoardCommand>(parameters, error_messages_, messages_);

    else if(command == "help")
      return std::make_unique<HelpCommand>(parameters, error_messages_, messages_);

    else if(command == "info")
      return std::make_unique<InfoCommand>(parameters, error_messages_, messages_);

    else if(command == "prison")
      return std::make_unique<PrisonCommand>(parameters, error_messages_, messages_);

    else if(command == "special")
      return std::make_unique<SpecialCommand>(parameters, error_messages_, messages_);

    else if(command == "move")
      return std::make_unique<MoveCommand>(parameters, error_messages_, messages_);

    else if(command == "use")
      return std::make_unique<UseCommand>(parameters, error_messages_, messages_);

    else if(command == "pass")
      return std::make_unique<PassCommand>(parameters, error_messages_, messages_);

    else if(command == "resign")
      return std::make_unique<ResignCommand>(parameters, error_messages_, messages_);

    else if(command == "draw")
      return std::make_unique<DrawCommand>(parameters, error_messages_, messages_);

    else if(command == "history")
      return std::make_unique<HistoryCommand>(parameters, error_messages_, messages_);
    
    else if(command == "clone")
      return std::make_unique<CloneCommand>(parameters, error_messages_, messages_);

    throw std::invalid_argument(error_messages_.at("E_UNKNOWN_COMMAND"));

    return nullptr;
  }
  catch(std::bad_alloc& exception1)
  {
    throw;
  }
}


void CommandLine::printErrorMessage(const std::string& key)
{
  std::cout << error_messages_.at(key);
}


void CommandLine::printMessage(const std::string& key)
{
  std::cout << messages_.at(key);
}