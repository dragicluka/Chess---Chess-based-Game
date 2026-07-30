#include "Player.hpp"
#include "Piece.hpp"

#include <utility>
#include <algorithm>


Player::Player(PlayerColor color, size_t mana, size_t elo, std::size_t mana_pool_size) : 
color_(color), mana_(mana), mana_pool_size_(mana_pool_size), elo_(elo), number_of_moves_(0), has_won_(false), 
is_draw_(false), can_move_multiple_(false), last_moved_piece_(nullptr)
{
  id_ = (color_ == PlayerColor::WHITE ? "White" : "Black");
}

Player::~Player() = default;

void Player::setPieces(std::vector<std::unique_ptr<Piece>> pieces)
{
  pieces_ = std::move(pieces);
}


bool Player::checkForCheck(Player* opponent, Board* board)
{
  for(auto& piece : pieces_)
  {
    if(piece.get()->isInCheck(opponent, board))
    {
      return true;
    }
  }
  return false;
}

bool Player::checkForStalemate(Player* opponent, Board* board)
{
  for(auto& piece : pieces_)
  {
    if(piece.get()->isInStalemate(opponent, board))
    {
      return true;
    }
  }
  return false;
}

bool Player::checkForCheckmate(Player* opponent, Board* board)
{
  for(auto& piece : pieces_)
  {
    if(piece.get()->isInCheckMate(opponent, board))
    {
      return true;
    }
  }
  return false;
}

bool Player::automaticWin(Player* opponent)
{
  std::vector<std::unique_ptr<Piece>>& opponent_prison = opponent->getPrisonPieces();
  for(auto& piece : opponent_prison)
  {
    if(piece.get()->getPieceType() == PieceType::K)
    {
      return true;
    }
  }
  return false;
}

bool Player::automaticDraw(Player* opponent)
{
  int count = 0;
  for(auto& piece : prison_pieces_)
  {
    if(piece.get()->getPieceType() == PieceType::K)
    {
      count++;
    }
  }
  std::vector<std::unique_ptr<Piece>>& opponent_prison = opponent->getPrisonPieces();
  for(auto& piece : opponent_prison)
  {
    if(piece.get()->getPieceType() == PieceType::K)
    {
      count++;
    }
  }
  if(count == 2)
    return true;

  return false;
}

void Player::addToPrison()
{
  auto current = pieces_.begin();
  while(current != pieces_.end())
  {
    if(current->get()->isInPrison())
    {
      prison_pieces_.push_back(std::move(*current));
      current = pieces_.erase(current);
    }
    else
    {
      current++;
    }
  }
}

void Player::addToMyPrison(Piece* piece, Player* opponent_player)
{
  std::vector<std::unique_ptr<Piece>>& opponent_prison_pieces = opponent_player->getPrisonPieces();

  auto current = pieces_.begin();
  while(current != pieces_.end())
  {
    if(current->get() == piece)
    {
      opponent_prison_pieces.push_back(std::move(*current));
      current = pieces_.erase(current);
    }
    else
    {
      current++;
    }
  }
}

void Player::printPrison()
{
  if(prison_pieces_.empty())
    return;

  size_t size = prison_pieces_.size();

  for(size_t i = 0; i < (size - 1); i ++)
  {
    for(size_t j = (i + 1); j < size; j ++)
    {
      if(prison_pieces_[i]->getValue() < prison_pieces_[j]->getValue())
      {
        std::swap(prison_pieces_[i], prison_pieces_[j]);
      }
      else if(prison_pieces_[i]->getValue() == prison_pieces_[j]->getValue())
      {
        if(prison_pieces_[i]->getIdString(prison_pieces_[i]->getId()) > 
        prison_pieces_[j]->getIdString(prison_pieces_[j]->getId()))
        {
          std::swap(prison_pieces_[i], prison_pieces_[j]);
        }
      }
    }
  }

  size_t i = 0;

  while(i < size)
  {
    int count = 1;

    while((i + 1) < size && prison_pieces_[i]->getId() == prison_pieces_[i+1]->getId())
    {
      count++;
      i++;
    }

    if(count == 1)
    {
      std::cout << prison_pieces_[i]->getIdString(prison_pieces_[i]->getId());
    }
    else
    {
      std::cout << count << "x" << prison_pieces_[i]->getIdString(prison_pieces_[i]->getId());
    }

    i++;
    if(i < size)
    {
      std::cout << ", ";
    }
  }

}


std::size_t Player::getFurthestRank()
{
  std::vector<std::size_t> pieces_ranks;

  for(auto& piece : pieces_)
  {
    pieces_ranks.push_back(piece.get()->getCoordinates().getRank());
  }

  std::size_t furthest_rank = pieces_ranks[0];
  if(color_ == PlayerColor::WHITE)
  {
    for(std::size_t rank : pieces_ranks)
    {
      if(rank > furthest_rank)
      {
        furthest_rank = rank;
      }
    }
  }

  else
  {
    for(std::size_t rank : pieces_ranks)
    {
      if(rank < furthest_rank)
      {
        furthest_rank = rank;
      }
    }
  }

  return furthest_rank;
}


double Player::getActualScore()
{
  if(is_draw_)
    return 0.5;
  
  if(has_won_)
    return 1.0;
  
  return 0.0;
}
