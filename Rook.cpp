#include "Rook.hpp"
#include "Board.hpp"


bool Rook::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
{
    (void) is_capture;
    std::size_t piece_file = coordinates_.getFile();
    std::size_t piece_rank = coordinates_.getRank();
    std::size_t target_file = to.getFile();
    std::size_t target_rank = to.getRank();

    if(invincible_for_ != -1)
    {
        return false;
    }

    if(target_file != piece_file && target_rank != piece_rank)
    {
        return false;
    }
    if(target_file == piece_file && target_rank == piece_rank)
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
    return true;
 }

 bool Rook::checkVerticaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_rank)
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
 bool Rook::checkHorisontaly(Board& board, std::size_t piece_file, std::size_t piece_rank, std::size_t target_file)
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
