#ifndef ITEMFACTORY_HPP
#define ITEMFACTORY_HPP

#include "Item.hpp"
#include <memory>

class ItemFactory
{
  public:
    ItemFactory() = default;
    ItemFactory(const ItemFactory&) = delete;
    ItemFactory& operator=(const ItemFactory&) = delete;
    ~ItemFactory() = default;

    /// @brief This function allocates items.
    /// @param id Reference to the item id, that is to be created.
    /// @return Returns unique ptr for corresponding item.
    static std::unique_ptr<Item> makeItem(std::string& id);
};

#endif