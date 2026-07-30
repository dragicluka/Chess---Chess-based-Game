#include "Bishop.hpp"
#include "Board.hpp"


bool Bishop::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
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
    if(!checkColor(board, to))
    {
        return false;
    }
    checked_path_.clear();
    if(target_file == piece_file)
    {
        if(target_rank > piece_rank)
        {
            for(std::size_t i = piece_rank + 1; i < target_rank; i++)
            {
                 if(!checkSquare(board, piece_file, i))
                {
                    return false;
                }
            }
        }
        else
        {
            for(std::size_t i = target_rank + 1; i < piece_rank; i++)
            {
                 if(!checkSquare(board, piece_file, i))
                {
                    return false;
                }
            }
        } 
        return true;
    }
    if(target_rank == piece_rank)
    {
         if(target_file > piece_file)
        {
            for(std::size_t i = piece_file + 1; i < target_file; i++)
            {
                if(!checkSquare(board, i, piece_rank))
                {
                    return false;
                }
            }
        }
        else
        {
            for(std::size_t i = target_file + 1; i < piece_file; i++)
            {
                if(!checkSquare(board, i, piece_rank))
                {
                    return false;
                }
            }
        } 
        return true;
    }
    if(file_dif != rank_dif)
    {
        return false;
    }
    return checkDiagonaly(board, to);
}

bool Bishop::checkColor(Board& board, Coordinates to)
{
    if(inherent_color_ == SquareType::WHITE && board.getSquare(to)->getSquareType() == SquareType::BLACK)
    {
        return false;
    }
    if(inherent_color_ == SquareType::BLACK && board.getSquare(to)->getSquareType() == SquareType::WHITE)
    {    
        return false;
    }
    return true;
}

bool Bishop::checkDiagonaly(Board& board, Coordinates to)
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
        if(!checkSquare(board, current_file, current_rank))
        {
            return false;
        }
        current_file += file_step;
        current_rank += rank_step;
    }
    //end
    return true;
}

bool Bishop::checkSquare(Board& board, std::size_t file, std::size_t rank)
{
    Square* square = board.getSquare(Coordinates(file, rank));
    if (square->getPiece() != nullptr)
    {
        return false;
    }
    if(!checkColor(board, Coordinates(file, rank)))
    {
        return false;
    }
    checked_path_.push_back(Coordinates(file, rank));
    return true;
}