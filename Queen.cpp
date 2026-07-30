#include "Queen.hpp"
#include "Board.hpp"


bool Queen::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
{
    (void) is_capture;
    std::size_t piece_file = coordinates_.getFile();
    std::size_t piece_rank = coordinates_.getRank();
    std::size_t target_file = to.getFile();
    std::size_t target_rank = to.getRank();

    std::size_t file_dif = std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file));
    std::size_t rank_dif = std::abs(static_cast<int>(piece_rank) - static_cast<int>(target_rank));

    if(target_file == piece_file && target_rank == piece_rank)
    {
    return false;
    }

    if (target_file != piece_file && target_rank != piece_rank && file_dif != rank_dif)
    {
        return false;
    }

    checked_path_.clear();

    if(target_file == piece_file)
    {
        return checkVerticaly(board, piece_file, piece_rank, target_rank);
    }
    if(target_rank == piece_rank)
    {
        return checkHorisontaly(board, piece_file, piece_rank, target_file);
    }
    if(file_dif == rank_dif)
    {
       return checkDiagonaly(board, to);
    }
    return true;
}

bool Queen::checkDiagonaly(Board& board, Coordinates to)
{
    std::size_t piece_file = coordinates_.getFile();
    std::size_t piece_rank = coordinates_.getRank();
    // from Claude
    // begin
    int file_step = (to.getFile() > piece_file) ? 1 : -1;
    int rank_step = (to.getRank() > piece_rank) ? 1 : -1;
    int current_file = static_cast<int>(piece_file) + file_step;
    int current_rank = static_cast<int>(piece_rank) + rank_step;

    while(current_file != static_cast<int>(to.getFile()) && current_rank != static_cast<int>(to.getRank()))
    {
    Square* square = board.getSquare(Coordinates(current_file, current_rank));
    if(square->getPiece() != nullptr)
        return false;
    current_file += file_step;
    current_rank += rank_step;
    }
    //end
    return true;
}

bool Queen::checkVerticaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_rank)
{
    if(target_rank > piece_rank)
    {
        for(std::size_t i = piece_rank + 1; i < target_rank; i++)
        {
            if (board.getSquare(Coordinates(piece_file, i))->getPiece() != nullptr)
            {
                return false;
            }
            checked_path_.push_back(Coordinates(piece_file, i));
        }
    }
    else
    {
        for(std::size_t i = target_rank + 1; i < piece_rank; i++)
        {
            if (board.getSquare(Coordinates(piece_file, i))->getPiece() != nullptr)
            {
                return false;
            }
            checked_path_.push_back(Coordinates(piece_file, i));
        }
    } 
    return true;
}
 bool Queen::checkHorisontaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file)
 {
     if(target_file > piece_file)
    {
        for(std::size_t i = piece_file + 1; i < target_file; i++)
        {
            if (board.getSquare(Coordinates(i, piece_rank))->getPiece() != nullptr)
            {
                return false;
            }
            checked_path_.push_back(Coordinates(i, piece_rank));
        }
    }
    else
    {
        for(std::size_t i = target_file + 1; i < piece_file; i++)
        {
            if (board.getSquare(Coordinates(i, piece_rank))->getPiece() != nullptr)
            {
                return false;
            }
            checked_path_.push_back(Coordinates(i, piece_rank));
        }
    } 
    return true;
 }