#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
class Board;
class Player;

enum class CommandReturnType
{
  PASSIVE_SUCCESS,
  ACTIVE_SUCCESS,
  ERROR,
  WIN,
  RESIGN_WIN,
  RESIGN_NORMAL_WIN,
  RESIGN_DRAW,
  KING_DRAW,
  DRAW,
  QUIT
};

class Command
{
  protected:
    std::vector<std::string> parameters_;
    std::map<std::string, std::string> error_messages_;
    std::map<std::string, std::string> messages_;
  
  public:
    Command(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : parameters_(parameters), error_messages_(error_messages),
    messages_(messages) {}
    Command(const Command&) = delete;
    virtual ~Command() = default;

    /// @brief Pure virtual method implemented differenlty for each command. It executes the effect of the command.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponnent player.
    /// @return Returns the value from defined enum class, called CommandReturyType.
    virtual CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) = 0;

    /// @brief Prints the message for the given key.
    /// @param key Key that determines which message will be printed.
    virtual void printMessage(const std::string& key);

    /// @brief Prints the error message for the given key.
    /// @param key Key that determines which error message will be printed.
    virtual void printErrorMessage(const std::string& key);
};


class HelpCommand : public Command
{
  public:
    HelpCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    HelpCommand(const HelpCommand&) = delete;
    virtual ~HelpCommand() = default;

    /// @brief Executes the help command. Checks for the number of parameters and then prints the text.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return It either returns ERROR, meaning that invalid number of parameters has been entered, or PASSIVE_SUCESS,
    /// which does not change the active player.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class DrawCommand : public Command
{
  public:
    DrawCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    DrawCommand(const DrawCommand&) = delete;
    virtual ~DrawCommand() = default;

    /// @brief Executes draw command. If no parameters were entered, draw is offered to opponent player.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return If opponennt player accepted the draw, DRAW is returned, so that the appropriate message could be 
    /// printed, if no is entered PASSIVE_SUCCESS is returned,, beacuse the turn will not be changed. ERROR will be
    /// returned when there are additional parameters entered, and when the opponent answer isn't yes or no.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class HistoryCommand : public Command
{
  public:
    HistoryCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    HistoryCommand(const HistoryCommand&) = delete;
    virtual ~HistoryCommand() = default;

    /// @brief Prints the history of all moves made so far.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns ERROR if an invalid number of parameters has been entered, otherwise PASSIVE_SUCCESS, as 
    /// the active player's turn will not be ended.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class InfoCommand : public Command
{
  public:
    InfoCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    InfoCommand(const InfoCommand&) = delete;
    virtual ~InfoCommand() = default;

    /// @brief Prints the detailed information about the specified piece.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns PASSIVE_SUCCESS, if the command was properly executed, in case of invalid count of parameters
    /// and in case of invalid piece id.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class PassCommand : public Command
{
  public:
    PassCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    PassCommand(const PassCommand&) = delete;
    virtual ~PassCommand() = default;

    /// @brief Gives an opportunity to active player to pass on their turn if they already have made a move.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns ACTIVE_SUCCESS if properly executed, which means that the next turn will begin, otherwise it
    /// returns ERROR in case of invalid count of parameters and if no moves have been made in the turn so far.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class PrisonCommand : public Command
{
  public:
    PrisonCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    PrisonCommand(const PrisonCommand&) = delete;
    virtual ~PrisonCommand() = default;

    /// @brief Prints the prison of either black or white player.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns PASSIVE_SUCCESS, if properly executed, otherwise ERROR in case of invalid parameter count and
    /// if the given parameter is not white or black.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class QuitCommand : public Command
{
  public:
    QuitCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    QuitCommand(const QuitCommand&) = delete;
    virtual ~QuitCommand() = default;

    /// @brief Quits the game.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns QUIT if properly executed, which is an indicator to the the start function in Game class to 
    /// stop the program. If any extra parameters have been entered, then ERROR is returned.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class ResignCommand : public Command
{
  public:
    ResignCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    ResignCommand(const ResignCommand&) = delete;
    virtual ~ResignCommand() = default;

    /// @brief Resign command ends the game immediantly, either in draw or in a win for an opponent.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns RESIGN_DRAW if the active player was in stalemate, otherwise RESIGN_WIN. If an invalid number
    /// of parameters was given, ERROR is returned.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class BoardCommand : public Command
{
  public:
    BoardCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    BoardCommand(const BoardCommand&) = delete;
    virtual ~BoardCommand() = default;

    /// @brief Toggles automatic board printing.
    /// @param board Reference to the board.
    /// @param active_player Reference to the active player.
    /// @param opponent_player Reference to the opponennt player.
    /// @return Returns ERROR if an invalid number of parameters has been entered, otherwise PASSIVE_SUCCESS.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class CloneCommand : public Command
{
  public:
    CloneCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    CloneCommand(const CloneCommand&) = delete;
    virtual ~CloneCommand() = default;

    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


/*class FuseCommand : public Command
{
  public:
  FuseCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
  std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
  FuseCommand(const FuseCommand&) = delete;
  virtual ~FuseCommand() = default;

  /// @brief Toggles automatic board printing.
  /// @param board Reference to the board.
  /// @param active_player Reference to the active player.
  /// @param opponent_player Reference to the opponennt player.
  /// @return Returns ERROR if an invalid number of parameters has been entered, otherwise PASSIVE_SUCCESS.
  CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};


class GambleCommand : public Command
{
  public:
  GambleCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
  std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
  GambleCommand(const GambleCommand&) = delete;
  virtual ~GambleCommand() = default;

  /// @brief Toggles automatic board printing.
  /// @param board Reference to the board.
  /// @param active_player Reference to the active player.
  /// @param opponent_player Reference to the opponennt player.
  /// @return Returns ERROR if an invalid number of parameters has been entered, otherwise PASSIVE_SUCCESS.
  CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;
};*/
#endif