#include "ItemFactory.hpp"
#include "Potion.hpp"
#include "Tool.hpp"

const std::string FREEZE_ID = "FREEZE";
const std::string FREEZE_DISPLAY_NAME = "*";
const std::string TELEPORT_ID = "TP";
const std::string TELEPORT_DISPLAY_NAME = "→";
const std::string EVENODD_ID = "EVENODD";
const std::string EVENODD_DISPLAY_NAME = "½";
const std::string SKYWALKER_ID = "LUKE";
const std::string SKYWALKER_DISPLAY_NAME = "↑";
const std::string SHIELD_ID = "SHIELD";
const std::string SHIELD_DISPLAY_NAME = "□";
const std::string INVISIBILITY_CLOAK_ID = "CLOAK";
const std::string INVISIBILITY_CLOAK_DISPLAY_NAME = "⌂";
const std::string QUEEN_REPELLANT_ID = "REPEL";
const std::string QUEEN_REPELLANT_DISPLAY_NAME = "R";


std::unique_ptr<Item> ItemFactory::makeItem(std::string& id)
{
  try
  {
    if(id == FREEZE_ID)
      return std::make_unique<FreezePotion>(FREEZE_ID, FREEZE_DISPLAY_NAME);
    
    if(id == TELEPORT_ID)
      return std::make_unique<TeleportPotion>(TELEPORT_ID, TELEPORT_DISPLAY_NAME);
    
    if(id == EVENODD_ID)
      return std::make_unique<EvenOddPotion>(EVENODD_ID, EVENODD_DISPLAY_NAME);
    
    if(id == SKYWALKER_ID)
      return std::make_unique<SkyWalkerPotion>(SKYWALKER_ID, SKYWALKER_DISPLAY_NAME);
    
    if(id == SHIELD_ID)
      return std::make_unique<ShieldTool>(SHIELD_ID, SHIELD_DISPLAY_NAME);
    
    if(id == INVISIBILITY_CLOAK_ID)
      return std::make_unique<InvisibilityCloakTool>(INVISIBILITY_CLOAK_ID, INVISIBILITY_CLOAK_DISPLAY_NAME);
    
    if(id == QUEEN_REPELLANT_ID)
      return std::make_unique<QueenRepellantTool>(QUEEN_REPELLANT_ID, QUEEN_REPELLANT_DISPLAY_NAME);

    return nullptr;
  }
  catch(std::bad_alloc& exception)
  {
    throw;
  }
}
