#ifndef ITEM_HPP
#define ITEM_HPP
#include <iostream>
#include <string>
#include <memory>

class Board;
class Square;

class Item
{
  protected:
    std::string id_;
    std::string display_name_;
  public:
    Item(const std::string& id , const std::string& display_name) : id_(id), display_name_(display_name) {}
    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;
    virtual ~Item() = default;

    /// @brief Pure virtual method implemented in subclasses
    /// @param board Reference to the board
    /// @param source_square Pointer to the source square (square on which the piece who has an item is).
    /// @param target_square Pointer to the square, who has the piece that is being targeted.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Returns true if the effect of the item was executed, otherwise false.
    virtual bool triggerEffect(Board& board, Square* source_square, Square* target_square, 
    const std::string& even_or_odd) = 0;

    /// @brief Getter for display_name_.
    /// @return Returns display_name_, as std::string.
    std::string getDisplayName() const {return display_name_;} 

    /// @brief Getter for id_.
    /// @return Returns id_, as std::string
    std::string getId() const {return id_;}

    /// @brief Pure virtual method implemented in subclasses
    /// @return Returns the unique ptr for potion or tool
    virtual std::unique_ptr<Item> clone() = 0;
};

#endif