#include "SpawnSquare.hpp"
#include "Piece.hpp"


bool SpawnSquare::triggerEffect(std::size_t round, Player* active_player)
{
  if(getSquareType() != SquareType::SPAWN)
    return false;
  
  if(active_player->getColor() != PlayerColor::WHITE)
    return false;

  if(round < 3 || round % 3 != 0)
    return false;
  
  if(getItem() != nullptr)
    return false;
  
  if(piece_.has_value() && piece_.value()->getItem() != nullptr)
    return false;
  
  setItem(spawnItem());

  if(piece_.has_value())
  {
    piece_.value()->setItem(item_.value());
    removeItem();
  }

  setCurrentItemIndex();
  return true;
}


Item* SpawnSquare::spawnItem()
{
  return items_.at(current_item_index).get();
}


void SpawnSquare::setCurrentItemIndex()
{
  if(current_item_index == items_.size() - 1)
  {
    current_item_index = 0;
    return;
  }

  if(current_item_index < items_.size())
  {
    current_item_index++;
  }
}