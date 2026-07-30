#ifndef BOARD_HPP
#define BOARD_HPP
#include <array>

#include "Square.hpp"
#include "Player.hpp"
#include "Piece.hpp"
#include "Move.hpp"

class Board{
    private:
        std::array<std::array<std::unique_ptr<Square>,8>,8> squares_;
        std::string chessboard_border_;
        std::string border_decoration_;
        bool print_board_;
        std::size_t round_;
        std::size_t max_turn_count_;
        std::vector<std::unique_ptr<Move>> move_history_;
        std::string file_string_;
    public:
        Board();
        Board(const Board&) = delete;
        ~Board() = default;

        /// @brief Getter for a sqaure.
        /// @param coordinates Coordinates at which the wanted sqaure is.
        /// @return Returns sqaure at given file and rank. If file or rank or both are invalid, it returns nullptr.
        Square* getSquare(Coordinates coordinates);

        /// @brief Sets the square in square_ array.
        /// @param coordinates Coordinates, that mark the position inside the array
        /// @param square Square to be set
        void setSquare(Coordinates coordinates, std::unique_ptr<Square> square);

        /// @brief Places a piece on the given square and updates the piece's coordinates.
        /// @param coordiantes The target square coordinates where the piece is placed.
        /// @param piece Pointer to the piece being placed
        void placePiece(Coordinates coordiantes, Piece* piece);

        /// @brief Assignes max_turn_count_ member variable a value, read from game config file
        /// @param max_turn_count Value to be assigned
        void setMaxTurnCount(std::size_t max_turn_count) {max_turn_count_ = max_turn_count;}

        /// @brief Getter for max_turn_count_
        /// @return Returns max_turn_count_, as std::size_t
        std::size_t getMaxTurnCount() const {return max_turn_count_;}

        /// @brief Prints all board relevant information, and calls the print method for each square
        /// @param active_player Pointer to the active player
        /// @param opponent_player Pointer to the opponent player
        void printBoard(Player* active_player, Player* opponent_player);

        /// @brief Setter for a chessboard border
        /// @param chessboard_border Chessboard border read from message config file
        void setChessboardBorder(const std::string& chessboard_border);

        /// @brief Setter for a border decoration
        /// @param chessboard_border Border decoration read from message config file
        void setBorderDecoration(const std::string& boarder_decoration);

        /// @brief Getter for print_board_, which determines if the board will be printed or not.
        /// @return Returns either true or false.
        bool getPrintBoard(){return print_board_;};

        /// @brief Setter for print_board_
        /// @param print_board Value that determines if the board will be printed or not
        void setPrintBoard(bool print_board){print_board_ = print_board;};

        /// @brief Getter for squares_.
        /// @return Returns the referance to the array.
        const std::array<std::array<std::unique_ptr<Square>,8>,8>& getBoard() const {return squares_;};

        /// @brief Increments round by one
        void setNewRound(){round_++;};

        /// @brief Getter for round_
        /// @return Return round_, as std::size_t.
        std::size_t getRound() const {return round_;}

        /// @brief The function that gets called at the beginning of each new turn and activates the effects of 
        /// special squares.
        /// @param active_player Pointer to the currently active player.
        void triggerSpecialSquares(Player* active_player);

        /// @brief Adds a move to the game's move history.
        /// @param move A unique_ptr owning the Move to be recorded
        void addToMoveHistory(std::unique_ptr<Move> move) {move_history_.push_back(std::move(move));};

        /// @brief prints the move history
        /// @param output Parameter that decides, in which format the text will be printed. By default it is set to
        /// std::cout, but it is also relevant for the output file created at the end of the game.
        void printMoveHistory(std::ostream& output = std::cout);
};

#endif