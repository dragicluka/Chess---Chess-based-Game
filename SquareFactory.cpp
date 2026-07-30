#include "SquareFactory.hpp"
#include "BoostSquare.hpp"
#include "ManaSquare.hpp"
#include "SpawnSquare.hpp"



std::unique_ptr<Square> SquareFactory::makeSquare(std::string& square_id, Coordinates coordinates)
{
  try
  {
    if(square_id == "BOOST")
      return std::make_unique<BoostSquare>(coordinates);
    
    if(square_id == "MANA")
      return std::make_unique<ManaSquare>(coordinates);
    
    if(square_id == "SPAWN")
      return std::make_unique<SpawnSquare>(coordinates);

    return nullptr;
  }
  catch(std::bad_alloc& exception)
  {
    throw;
  }
}
