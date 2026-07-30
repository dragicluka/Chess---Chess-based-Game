#include "Piece.hpp"
#include "PieceFactory.hpp"

const std::string EVEN = "even";
const std::string ODD = "odd";

Piece::Piece(PieceType type, PieceID id, PlayerColor owner_color, Coordinates coordinates, size_t value, 
    PowerType power) : type_(type), id_(id), owner_color_(owner_color), owner_(nullptr), coordinates_(coordinates),
     value_(value), power_(power),item_(nullptr), is_frozen_(false), target_piece_(nullptr), target_square_(nullptr), 
     bounce_square_(nullptr) ,is_in_prison_(false) , initially_invincible_for_(-1), invincible_for_(-1)
     , is_even_odd_triggered_(false), even_or_odd_(""), inherent_color_(SquareType::BLACK), has_moved_(false), 
     frozen_for_(0), castling_rook_(nullptr), checked_path_(), pawn_moved_two_squares(false), 
     moving_en_passant_(false), is_cloned_(false), was_cloned_(false), cloned_for_(-1) {}

std::string Piece::getIdString(PieceID id) const
{
    switch (id)
    {
        case PieceID::P:    return "P";
        case PieceID::PGLD: return "PGLD";
        case PieceID::PIPT: return "PIPT";
        case PieceID::PSTB: return "PSTB";
        case PieceID::PNRV: return "PNRV";
        case PieceID::PEXP: return "PEXP";

        case PieceID::R:    return "R";
        case PieceID::RINV: return "RINV";
        case PieceID::RPNT: return "RPNT";

        case PieceID::N:    return "N";
        case PieceID::NJMP: return "NJMP";
        case PieceID::NICE: return "NICE";

        case PieceID::B:    return "B";
        case PieceID::BCLR: return "BCLR";
        case PieceID::BPRC: return "BPRC";

        case PieceID::Q:    return "Q";
        case PieceID::QFLP: return "QFLP";
        case PieceID::QJMP: return "QJMP";
        case PieceID::QHNGR:return "QHNGR";

        case PieceID::K:    return "K";
        case PieceID::KFRT: return "KFRT";
        case PieceID::KARC: return "KARC";
    }
}

void Piece::setInitialInvincible(int turn_count)
{
    initially_invincible_for_ = turn_count;
    invincible_for_ = turn_count;
}

bool Piece::takeInvinicible()
{
    if(invincible_for_ >= 0)
    {
        invincible_for_ -= 1;
        return true;
    }
    return false;
}

std::string Piece::getTypeString() const
{
    if(type_ == PieceType::P)
    {
        return "p";
    }
    if(type_ == PieceType::B)
    {
        return "b";
    }
    if(type_ == PieceType::Q)
    {
        return "q";
    }
    if(type_ == PieceType::K)
    {
        return "k";
    }
    if(type_ == PieceType::N)
    {
        return "n";
    }
    else
    {
        return "r";
    }
}


bool Piece::canMove(const std::string& even_or_odd, std::size_t& round)
{
    if(even_or_odd == EVEN && round % 2 == 0)
    {
        return true;
    }

    if(even_or_odd == ODD && round % 2 == 1)
    {
        return true;
    }

    return false;
}


void Piece::setEvenOdd(const std::string& even_or_odd)
{
    is_even_odd_triggered_ = true;
    even_or_odd_ = even_or_odd;
}

bool Piece::isEvenOddTriggered(std::size_t round)
{
    if(!is_even_odd_triggered_)
        return false;
    
    if(!canMove(even_or_odd_, round))
        return true;
    
    return false;
}

void Piece::setNewShortName(std::string id, PlayerColor color, Coordinates coordinates)
{
    std::unique_ptr<Piece> piece = PieceFactory::makePiece(id, color, coordinates);
    short_name_ = piece->getShortName();
}


bool Piece::hasItem(const std::string& item_id)
{
    if(item_ == nullptr)
        return false;
    
    if(item_->getId() == item_id)
        return true;
    
    return false;
}


void Piece::unfreeze()
{
    if(frozen_for_> 0)
    {
        frozen_for_--;
        if(frozen_for_ == 0)
        {
            is_frozen_ = false;
        }
    }
}

void Piece::setCloned()
{
    cloned_for_ = 2;
    is_cloned_ = true;
}


void Piece::unclone()
{
    if(cloned_for_ > 0)
    {
        cloned_for_--;

        if(cloned_for_ == 0)
        {
            is_cloned_ = false;
        }
    }

}
