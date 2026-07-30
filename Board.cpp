#include "Board.hpp"

#include <iomanip>

Board::Board() : print_board_(true),round_(0), max_turn_count_(0){
    try
    {
        for (std::size_t rank = 0; rank < 8; rank++) 
        {
            for (std::size_t file = 0; file < 8; file++) 
            {
                squares_[rank][file] = std::make_unique<Square>(Coordinates(file, rank));
            }
        }
    }
    catch(std::bad_alloc& exception)
    {
        throw;
    }
}

Square* Board::getSquare(Coordinates coordinates) {
    std::size_t rank = coordinates.getRank();
    std::size_t file = coordinates.getFile();
    if(rank > 7 || file > 7)
    {
        return nullptr;
    }
    return squares_[rank][file].get();
}

void Board::setSquare(Coordinates coordinates, std::unique_ptr<Square> square)
{
  squares_[coordinates.getRank()][coordinates.getFile()] = std::move(square);
}


void Board::placePiece(Coordinates coordinates, Piece* piece) {
    squares_[coordinates.getRank()][coordinates.getFile()]->setPiece(piece);
    piece->getCoordinates().setFile(coordinates.getFile());
    piece->getCoordinates().setRank(coordinates.getRank());
}

void Board::printBoard(Player* active_player, Player* opponent_player)
{
    std::cout << chessboard_border_ ;
    std::cout << border_decoration_ ;
    std::cout << "Turn " << round_ << " / " << max_turn_count_ << "\n\n";
    std::cout << opponent_player->getID() << " mana: " 
    << opponent_player->getMana() << "/" << opponent_player->getManaPoolSize() << "\n\n";

    int file_start = 0;
    int file_end = 0;
    int file_count = 0;
    int rank_start = 0;
    int rank_end = 0;
    int rank_count = 0;

    if(active_player->getID() == "White")
    {
        rank_start = 7;
        rank_end = -1;
        rank_count = -1;
        file_start = 0;
        file_end = 8;
        file_count = 1;
    }
    else
    {
        rank_start = 0;
        rank_end = 8;
        rank_count = 1;
        file_start = 7;
        file_end = -1;
        file_count = -1;
    }

    for(int i = rank_start; i != rank_end; i += rank_count)
    {
        std::cout << (i + 1) << " " ;
        for(int j = file_start; j != file_end; j += file_count)
        {
            squares_[i][j]->print(opponent_player, active_player, this);
        }
        std::cout << "\n";
    }

    std::cout << " ";

    for(int i = file_start; i != file_end; i += file_count)
    {
        std::cout << "   " << (char)('A' + i) ;
    }

    std::cout << "\n\n";

    std::cout << active_player->getID() << " mana: "
    << active_player->getMana() << "/" << active_player->getManaPoolSize() << "\n";
    std::cout << border_decoration_;
    std::cout << "\n";
}

void Board::setChessboardBorder(const std::string& chessboard_border)
{
    chessboard_border_ = chessboard_border;
}

void Board::setBorderDecoration(const std::string& boarder_decoration)
{
    border_decoration_ = boarder_decoration;
}


void Board::triggerSpecialSquares(Player* active_player)
{
    for(auto& rank_array : squares_)
    {
        for(auto& square : rank_array)
        {
            square->triggerEffect(round_, active_player);
        }
    }
}

void Board::printMoveHistory(std::ostream& output){
    if(move_history_.empty())
    {
        return;
    }
    //from Claude
    //begin
    std::size_t i = 0;
    while (i < move_history_.size())
    {
        Move* white_move = nullptr;
        Move* black_move = nullptr;

        // uzmi white move
        if (i < move_history_.size() && move_history_[i]->getPlayerColor() == PlayerColor::WHITE)
            white_move = move_history_[i++].get();

        // uzmi black move
        if (i < move_history_.size() && move_history_[i]->getPlayerColor() == PlayerColor::BLACK)
            black_move = move_history_[i++].get();

        std::size_t round = white_move ? white_move->getRound() : black_move->getRound();

        // round number, sirina 3
        output << std::left << std::setw(3) << round << "| ";
        // white move
        if (white_move)
        {
            std::string white_str;

            if (white_move->getPotionName() != "")
                white_str += white_move->getPotionName();

            if (white_move->getSpecialName() != "")
                white_str += white_move->getSpecialName();
            else
                white_str += white_move->getMoveString();

            output << std::left << std::setw(8) << white_str;
        }
        else
            output << std::left << std::setw(8) << "";

        output << " | ";

        // black move
        if (black_move)
        {
            std::string black_str;

            if (black_move->getPotionName() != "")
                black_str += black_move->getPotionName();

            if (black_move->getSpecialName() != "")
                black_str += black_move->getSpecialName();
            else
                black_str += black_move->getMoveString();

            output << std::left << std::setw(8) << black_str;
        }
        else
            output << std::left << std::setw(8) << "";

        output << " |\n";
    }
    output <<"\n";
    //end
}