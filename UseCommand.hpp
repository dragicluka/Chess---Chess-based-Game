#ifndef USECOMMAND_HPP
#define USECOMMAND_HPP

#include "Command.hpp"
#include "Potion.hpp"

class UseCommand : public Command
{
  public:
    UseCommand(std::vector<std::string> parameters, std::map<std::string, std::string> error_messages, 
    std::map<std::string, std::string> messages) : Command(parameters, error_messages, messages) {}
    UseCommand(const UseCommand&) = delete;
    virtual ~UseCommand() = default;

    /// @brief Executes the effect of use command.
    /// @param board Reference to the board
    /// @param active_player Reference to active player
    /// @param opponent_player Reference to opponent player
    /// @return Returns ERROR if ERROR has occur, otherwise ACTIVE_SUCCESS if the command was properly executed.
    CommandReturnType execute(Board& board, Player& active_player, Player& opponent_player) override;

    /// @brief Function that validates all potential errors before the execution of the potion.
    /// @param board Reference to the board
    /// @param active_player Reference to active player
    /// @param potion Reference to the pointer to the potion declared in execute function and modified here.
    /// @param source_square Reference to the pointer to the source sqaure declared in execute function and modified here.
    /// @param piece_on_square Reference to the pointer to the piece on source square declared in execute function 
    /// and modified here.
    /// @return Returns ERROR if some error has occured, otherwise ACTIVE_SUCCESS, that is later processed in execute 
    /// function
    CommandReturnType validate(Board& board, Player& active_player, Potion*& potion, Square*& source_square, 
    Piece*& piece_on_square);

    /// @brief This function handles the logic for execution of the freeze potion.
    /// @param board Reference to the board.
    /// @param active_player Refrence to the active player.
    /// @param potion Pointer to the potion, in this case it's freeze.
    /// @param source_square Pointer to the source square.
    /// @param piece_on_square Pointer to the piece on source sqaure, who is the owner of the potion.
    /// @return Returns ACTIVE_SUCCESS, after successful execution, otherwise ERROR.
    CommandReturnType handleFreeze(Board& board, Player& active_player, Potion* potion, Square* source_square, 
    Piece* piece_on_square);

    /// @brief This function handles the logic for execution of the freeze potion.
    /// @param board Reference to the board.
    /// @param active_player Refrence to the active player.
    /// @param potion Pointer to the potion, in this case it's teleport.
    /// @param source_square Pointer to the source square.
    /// @param piece_on_square Pointer to the piece on source sqaure, who is the owner of the potion.
    /// @return Returns ACTIVE_SUCCESS, after successful execution, otherwise ERROR.
    CommandReturnType handleTP(Board& board, Player& active_player, Potion* potion, Square* source_square, 
    Piece* piece_on_square);

    /// @brief This function handles the logic for execution of the freeze potion.
    /// @param board Reference to the board.
    /// @param active_player Refrence to the active player.
    /// @param potion Pointer to the potion, in this case it's even odd.
    /// @param source_square Pointer to the source square.
    /// @param piece_on_square Pointer to the piece on source sqaure, who is the owner of the potion.
    /// @return Returns ACTIVE_SUCCESS, after successful execution, otherwise ERROR.
    CommandReturnType handleEvenOdd(Board& board, Player& active_player, Potion* potion, Square* source_square, 
    Piece* piece_on_square);

    /// @brief This function handles the logic for execution of the freeze potion.
    /// @param board Reference to the board.
    /// @param active_player Refrence to the active player.
    /// @param potion Pointer to the potion, in this case it's skywalker.
    /// @param source_square Pointer to the source square.
    /// @param piece_on_square Pointer to the piece on source sqaure, who is the owner of the potion.
    /// @return Returns ACTIVE_SUCCESS, after successful execution, otherwise ERROR.
    CommandReturnType handleLuke(Board& board, Player& active_player, Potion* potion, Square* source_square, 
    Piece* piece_on_square);

    /*CommandReturnType handleMirror(Board& board, Player& active_player, Potion* potion, Square* source_square, 
    Piece* piece_on_square);*/
};


#endif