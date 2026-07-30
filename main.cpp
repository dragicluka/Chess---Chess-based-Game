#include "Game.hpp"

const std::size_t VALID_PARAMETER_COUNT = 3;


const std::string INVALID_ARGUMENT_COUNT_MESSAGE = "Error: Wrong number of arguments!\n";
const std::string MEMORY_ERROR_MESSAGE = "Error: Not enough memory!\n";
const std::string GAME_MAGIC_NUMBER = "GAME";
const std::string MESSAGE_MAGIC_NUMBER = "MESSAGE";

enum _ReturnValue_
{
    SUCCESS,
    MEMORY_ERROR,
    INVALID_PARAMETER_COUNT,
    INVALID_FILE
};

int main(int argc, char *argv[])
{
  try
  {
    if (argc != VALID_PARAMETER_COUNT)
    {
      std::cout << INVALID_ARGUMENT_COUNT_MESSAGE;
      return INVALID_PARAMETER_COUNT;
    }
    std::string game_file_path = argv[1];
    std::string message_file_path = argv[2];
    Game game;

    if (!game.checkMagicNumber(game_file_path, GAME_MAGIC_NUMBER) || 
    !game.checkMagicNumber(message_file_path, MESSAGE_MAGIC_NUMBER))
    {
      return INVALID_FILE;
    }

    game.loadGameConfigFile(game_file_path);
    game.loadMessageConfigFile(message_file_path);
    game.start();
    return SUCCESS;
  }
  catch(std::bad_alloc&)
  {
    std::cout << MEMORY_ERROR_MESSAGE;
    return MEMORY_ERROR;
  }
};