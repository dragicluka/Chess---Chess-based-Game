#include "ManaSquare.hpp"
#include "Player.hpp"
#include "Piece.hpp"


bool ManaSquare::triggerEffect(std::size_t round, Player* active_player)
{
  (void)round;
  if(!isOccupied())
    return false;
  
  if(getSquareType() != SquareType::MANA)
    return false;
  
  Player* owner = piece_.value()->getOwner();

  if(owner->getColor() != active_player->getColor())
    return false;
  
  owner->setMana(owner->getMana() + 1);

  return true;
}