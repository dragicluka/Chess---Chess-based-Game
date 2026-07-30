#ifndef SPAWNSQUARE_HPP
#define SPAWNSQUARE_HPP
#include "Square.hpp"

class SpawnSquare : public Square{
  private:
    std::vector<std::unique_ptr<Item>> items_;
    std::size_t current_item_index;
  public:
    SpawnSquare(Coordinates coordinates) : Square(coordinates), current_item_index(0) 
    {square_type_ = SquareType::SPAWN;}

    /// @brief Triggers the effect of the spawn square.
    /// @param round Represents the current number of the round
    /// @param active_player Pointer to the active player
    /// @return Returns true if the item was spawned, otherwise false.
    bool triggerEffect(std::size_t round, Player* active_player) override;

    /// @brief Setter for the items, read from the game config file.
    /// @param items Items to be assigned to member variable items_.
    void setItems(std::vector<std::unique_ptr<Item>> items) {items_ = std::move(items);}

    /// @brief Gets an item from the items vector at the current index
    /// @return Returns item pointer that will now be spawned on the square.
    Item* spawnItem();

    /// @brief Getter for items_.
    /// @return Returns the vector of unique pointers of items.
    std::vector<std::unique_ptr<Item>>& getItems() {return items_;}

    /// @brief Sets the current item index, needed to determeine which item will be spawned.
    void setCurrentItemIndex();
};


#endif