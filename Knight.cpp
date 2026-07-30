#include "Knight.hpp"
#include "Board.hpp"


bool Knight::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
{
    (void) board;
    (void) is_capture;
    std::size_t piece_file = coordinates_.getFile();
    std::size_t piece_rank = coordinates_.getRank();
    std::size_t target_file = to.getFile();
    std::size_t target_rank = to.getRank();

    std::size_t file_dif = std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file));
    std::size_t rank_dif = std::abs(static_cast<int>(piece_rank) - static_cast<int>(target_rank));

    if(file_dif == 1 && rank_dif == 2)
    {
        return true;
    }
    if(file_dif == 2 && rank_dif == 1)
    {
        return true;
    }

    return false;

}

bool JumpyKnight::canMoveToTargetSquare(Board& board, Coordinates to, bool is_capture)
{
    (void) board;
    (void) is_capture;
    std::size_t piece_file = coordinates_.getFile();
    std::size_t piece_rank = coordinates_.getRank();
    std::size_t target_file = to.getFile();
    std::size_t target_rank = to.getRank();

    std::size_t file_dif = std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file));
    std::size_t rank_dif = std::abs(static_cast<int>(piece_rank) - static_cast<int>(target_rank));

    if(file_dif == 2 && rank_dif == 3)
    {
        return true;
    }
    if(file_dif == 3 && rank_dif == 2)
    {
        return true;
    }

    return false;
}


void IceKnight::special(Board* board, Player* opponent)
{
    (void) opponent;
    std::size_t piece_file = coordinates_.getFile();
    std::size_t piece_rank = coordinates_.getRank();
    std::size_t target_file = target_square_->getCoordinates().getFile();
    std::size_t target_rank = target_square_->getCoordinates().getRank();
    std::size_t file_dif = std::abs(static_cast<int>(piece_file) - static_cast<int>(target_file));
    std::size_t rank_dif = std::abs(static_cast<int>(piece_rank) - static_cast<int>(target_rank));

    const std::array<std::array<std::unique_ptr<Square>,8>,8>& squares = (*board).getBoard();

    if(file_dif > rank_dif)
    {
        if(piece_file > target_file) 
        {
            freezePieces(squares, piece_rank, piece_file - 1, piece_rank, piece_file -2);
        }
        else 
        {
            freezePieces(squares,piece_rank, piece_file + 1 , piece_rank, piece_file + 2);
        }

    }
    else
    {
        if(piece_rank > target_rank) 
        {
            freezePieces(squares, piece_rank - 1, piece_file, piece_rank - 2, piece_file);
        }
        else
        {
            freezePieces(squares, piece_rank + 1, piece_file, piece_rank + 2, piece_file);
        }

    }
}

void IceKnight::freezePieces(const std::array<std::array<std::unique_ptr<Square>,8>,8>& squares,std::size_t rank1,
     std::size_t file1, std::size_t rank2, std::size_t file2)
{
    Piece* piece = squares[rank1][file1]->getPiece();
    if(piece != nullptr)
    {
        piece->setFrozen();
    }
    piece = squares[rank2][file2]->getPiece();
    if(piece != nullptr)
    {
        piece->setFrozen();
    }
}