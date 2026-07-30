#ifndef TOOL_HPP
#define TOOL_HPP
#include "Item.hpp"

class Square;
class Board;

class Tool : public Item
{
  public:
    Tool(const std::string& id, const std::string& display_name) : Item(id, display_name) {}
    Tool (const Tool&) = delete;
    virtual ~Tool() = default;
};


class ShieldTool : public Tool
{
  public:
    ShieldTool(const std::string& id, const std::string& display_name) : Tool(id, display_name) {}
    ShieldTool(const ShieldTool&) = delete;
    virtual ~ShieldTool() = default;

    /// @brief Removes the item from piece's inventory.
    /// @param board Reference to the board, not used here.
    /// @param source_square Reference to the square on which piece is firstly positioned
    /// @param target_square Reference to the sqaure on which piece could potentially end up, unused
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Returns true, when piece is removed.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, const std::string& even_or_odd) 
    override;

    /// @brief Creates a deep copy of Shield Tool.
    /// @return A unique_ptr owning a new Shield Tool with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<ShieldTool>(id_, display_name_);}
};


class InvisibilityCloakTool : public Tool
{
  public:
    InvisibilityCloakTool(const std::string& id, const std::string& display_name) : Tool(id, display_name) {}
    InvisibilityCloakTool(const InvisibilityCloakTool&) = delete;
    virtual ~InvisibilityCloakTool() = default;

    /// @brief No trigger effect; this tool has no actively triggered behavior.
    /// @param board Reference to the board, unused.
    /// @param source_square Pointer to the source sqaure, unused.
    /// @param target_square Pointer to the target square, unused.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Always false.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, const std::string& even_or_odd) 
    override {(void) board; (void) source_square; (void) target_square; (void) even_or_odd; return false;}

    /// @brief Creates a deep copy of Invisibility Cloak Tool.
    /// @return A unique_ptr owning a new Invisibility Cloak Tool with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<InvisibilityCloakTool>(id_, display_name_);}
};

class QueenRepellantTool : public Tool
{
  public:
    QueenRepellantTool(const std::string& id, const std::string& display_name) : Tool(id, display_name) {}
    QueenRepellantTool(const QueenRepellantTool&) = delete;
    virtual ~QueenRepellantTool() = default;

    /// @brief No trigger effect; this tool has no actively triggered behavior.
    /// @param board Reference to the board, unused.
    /// @param source_square Pointer to the source sqaure, unused.
    /// @param target_square Pointer to the target square, unused.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Always false.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, const std::string& even_or_odd) 
    override {(void) board; (void) source_square; (void) target_square; (void) even_or_odd; return false;}

    /// @brief Creates a deep copy of Queen Repellant Tool.
    /// @return A unique_ptr owning a new Queen Repellant Tool with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<QueenRepellantTool>(id_, display_name_);}
};


#endif