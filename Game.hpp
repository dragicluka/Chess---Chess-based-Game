#ifndef GAME_HPP
#define GAME_HPP
#include "CommandLine.hpp"
#include "Board.hpp"
#include <vector>
#include <fstream>
#include "Move.hpp"
#include "Player.hpp"
#include "PieceFactory.hpp"
#include "SquareFactory.hpp"
#include "ItemFactory.hpp"

class Game{
    private:
        CommandLine command_line_;
        Board board_;
        Player white_player_;
        Player black_player_;
        Player* active_player_;
        Player* opponent_player_;
        bool is_running_;
        std::string final_message_;
        std::string elo_message_;
    public:
        Game();
        Game(const Game&) = delete;
        ~Game()= default;

        /// @brief Checks if magic number is correct.
        /// @param file_path Reference to either message or game config file
        /// @param magic_number Reference to the appropriate magic number.
        /// @return Returns true if maic number is correct, otherwise false.
        bool checkMagicNumber(const std::string &game_file_path, const std::string& magic_number);

        /// @brief Loads game config file with helper functions.
        /// @param game_file_path The path to game config file
        void loadGameConfigFile(std::string &game_file_path);

        /// @brief Loads messages to config file
        /// @param message_file_path The path to message config file
        void loadMessageConfigFile(std::string &message_file_path);

        /// @brief Contains the game logic.
        void start();

        /// @brief Prints the end message if draw occurs.
        /// @param cause_string The explanation for the draw
        void endGameWithDraw(const std::string& cause_string);

        /// @brief Prints the end message if win has occured
        /// @param cause_string The reason for win.
        /// @param winner Pointer to the winner of the game.
        /// @param loser Pointer to the loser of the game.
        /// @param is_resign_win Boolean that determines if the win came beacuse of resignation or no.
        void endGamewithWin(const std::string& cause_string, Player* winner, Player* loser, bool is_resign_win);

        /// @brief Handles setting pieces on board on game start
        void setPiecesOnBoard();

        /// @brief Placing pawns on the front row
        void placeFrontRow();

        /// @brief Reads user input and places piece on inputed square
        /// @param current_player Pointer to current Player
        /// @param current_index Pointer to current index in Players array of pieces
        /// @param current_piece Pointer to piece beeing placed
        /// @return True on success
        bool placingInput(Player* current_player, std::size_t* current_index, Piece* current_piece);

        /// @brief Prints the welcome message
        void printWelcomeMessage();

        /// @brief Handles auto command 
        /// @param current_player Pointer to current Player
        /// @param current_index  Current index in Players array of pieces
        void autoCommand(Player* current_player, std::size_t current_index);

        /// @brief Calcualtes new elo scores for both players. Called at the end of the game.
        void calculateElo();

        /// @brief Prints new elo scores, called inside calculateElo function.
        void printNewEloScores();

        /// @brief Decrements the invincibility counter for all pieces of both players.
        void takeInvincible();

        /// @brief Creates the output file at the end of the game.
        void createOutputFile();

        /// @brief Parses some basic game settings fro  game config file, like max turn count, mana and initial mana.
        /// @param game_file Reference to the game config file.
        void parseGameSettings(std::ifstream& game_file);

        /// @brief Parses pieces for both players from game config file and sets elo.
        /// @param game_file Reference to the game config file.
        void parsePieces(std::ifstream& game_file);

        /// @brief Parses special squares from game config file.
        /// @param game_file Reference to the game config file.
        void parseSquares(std::ifstream& game_file);

        /// @brief Calls the functions that calculate new elo score and create an output file and ends the game.
        void finishGame();

        /// @brief Sets the status of both players to draw.
        void setBothPlayersToDraw();

        /// @brief This function handles the events that occur at the beginning of the new round, such as checking if 
        /// the max turn count was reached, it triggers the effect of special squares and sets mana to active player.
        /// @return Returns false if the max turn count is reached and the game ends, otherwise true.
        bool handleNewTurn();
        
        /// @brief Handles  active success behaviour
        /// @param new_round reference to new round parameter
        /// @param temp POinter to the tempporary player, usedd to switch players
        void handleActiveSuccess(bool& new_round, Player* temp);
        
};


#endif