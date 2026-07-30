#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <memory>
#include "Command.hpp"
#include "MoveCommand.hpp"
#include "SpecialCommand.hpp"
#include "UseCommand.hpp"

#include "Utils.hpp"

class Board;

class CommandLine
{
  private:
    std::map<std::string, std::string> error_messages_;
    std::map<std::string, std::string> messages_;
  
  public:
    CommandLine() = default;
    CommandLine(const CommandLine&) = delete;

    /// @brief Prints the prompt for the current player
    /// @param player Pointer to the current player
    void printPrompt(Player* current_player);

    /// @brief This function reads user input, splits it into command name and parameters, and based on command name
    /// it returns the unique pointer to that command.
    /// @return Returns a unique pointer to the specified command.
    std::unique_ptr<Command> getCommand(); // čita user input, vraca unique ptr u zavinosti od komande, printa eror 
    // ukoliko komanda nije validna, komande nasljedjuju sve moguce eror poruke i dalje u svojim klasama printaju erore

    /// @brief Setter for error messages.
    /// @param errors Map that will be assigned to member variable error_messages_.
    void setErrorMessages(std::map<std::string, std::string> errors) { error_messages_ = errors;}

    /// @brief Setter for non-error messages.
    /// @param messages Map that will be assigned to member variable messages_.
    void setMessages(std::map<std::string, std::string> messages) {messages_ = messages;}


    virtual ~CommandLine() = default;

    /// @brief Prints the message from the message config file.
    /// @param key Key in the map that determines which message will be printed.
    void printMessage(const std::string& key);

    /// @brief Prints the error message from message config file.
    /// @param key Key in the map that determines which error message will be printed.
    void printErrorMessage(const std::string& key);

    /// @brief Getter for non-error messages.
    /// @return Returns all messages, with their unique keys, stored in the map.
    std::map<std::string, std::string> getMessages() {return messages_;}
};

#endif