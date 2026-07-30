#ifndef POTION_HPP
#define POTION_HPP
#include "Item.hpp"

class Piece;

class Potion : public Item{
  public:
    Potion(const std::string& id, const std::string& display_name) : Item(id, display_name) {}
    Potion(const Potion&) = delete;
    virtual ~Potion() = default;

    /// @brief Pure virtual method, implemented in  every subclass. Used in use command to determine, if the valid
    /// number of parameters for chosen potion has been entered
    /// @return Returns the number of parameters in form of std_size_t
    virtual std::size_t getRequiredParameterCount() const = 0;
};

class FreezePotion : public Potion
{
  public:
    FreezePotion(const std::string& id, const std::string& display_name) : Potion(id, display_name) {}
    FreezePotion(const FreezePotion&) = delete;
    virtual ~FreezePotion() = default;

    /// @brief Freezes the piece on target_square for 1 turn.
    /// @param board Reference to the board, not used here.
    /// @param source_square Reference to the square on which piece is firstly positioned
    /// @param target_square Reference to the sqaure on which piece that will be frozen is.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Returns only true, after the freeze was applied.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, 
    const std::string& even_or_odd) override;

    /// @brief Returns the number of parameters this potion requires
    /// @return The required number of parameters.
    std::size_t getRequiredParameterCount() const override {return 1;}

    /// @brief Creates a deep copy of Freeze Potion.
    /// @return A unique_ptr owning a new Freeze Potion with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<FreezePotion>(id_, display_name_);}
};

class TeleportPotion : public Potion
{
  public:
    TeleportPotion(const std::string& id, const std::string& display_name) : Potion(id, display_name) {}
    TeleportPotion(const TeleportPotion&) = delete;
    virtual ~TeleportPotion() = default;

    /// @brief Teleports the piece from source_square to target_square.
    /// @param board Reference to the board, not used here.
    /// @param source_square Reference to the square on which piece is firstly positioned
    /// @param target_square Reference to the sqaure on which piece will be teleported.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Returns true after the piece was removed from the source_square and moved to target_square.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, 
    const std::string& even_or_odd) override;

    /// @brief Returns the number of parameters this potion requires
    /// @return The required number of parameters.
    std::size_t getRequiredParameterCount() const override {return 1;}

    /// @brief Creates a deep copy of Teleport Potion.
    /// @return A unique_ptr owning a new Teleport Potion with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<TeleportPotion>(id_, display_name_);}
};

class EvenOddPotion : public Potion
{
  public:
    EvenOddPotion(const std::string& id, const std::string& display_name) : Potion(id, display_name) {}
    EvenOddPotion(const EvenOddPotion&) = delete;
    virtual ~EvenOddPotion() = default;

    /// @brief Forces an enemy piece on target_square to move only on either even or odd turns 
    /// for the remainder of the game.
    /// @param board Reference to the board, not used here.
    /// @param source_square Reference to the square on which active piece is positioned
    /// @param target_square Reference to the sqaure on which the enemy piece is
    /// @param even_or_odd Reference to the string even or odd
    /// @return Returns true after the potion was applied to opponent piece.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, 
    const std::string& even_or_odd) override;

    /// @brief Returns the number of parameters this potion requires
    /// @return The required number of parameters.
    std::size_t getRequiredParameterCount() const override {return 2;}

    /// @brief Creates a deep copy of EvenOdd Potion.
    /// @return A unique_ptr owning a new EvenOdd Potion with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<EvenOddPotion>(id_, display_name_);}
};

class SkyWalkerPotion : public Potion
{
  public:
    SkyWalkerPotion(const std::string& id, const std::string& display_name) : Potion(id, display_name) {}
    SkyWalkerPotion(const SkyWalkerPotion&) = delete;
    virtual ~SkyWalkerPotion() = default;

    /// @brief Pushes the piece on source_square directly in front of the activating piece one square backward.
    /// If multiple squares are alligned behind it, then they are pushed as well.
    /// @param board Reference to the board
    /// @param source_square Reference to the square on which piece is firstly positioned
    /// @param target_square Reference to the target_square, not used here.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Returns false if the piece could not be moved, otherwise true.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, 
    const std::string& even_or_odd) override;

    /// @brief Returns the number of parameters this potion requires
    /// @return The required number of parameters.
    std::size_t getRequiredParameterCount() const override {return 0;}

    /// @brief Creates a deep copy of Skywalker Potion.
    /// @return A unique_ptr owning a new Skywalker Potion with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<SkyWalkerPotion>(id_, display_name_);}
};

/*class MirrorPotion : public Potion
{
  public:
    MirrorPotion(const std::string& id, const std::string& display_name) : Potion(id, display_name) {}
    MirrorPotion(const MirrorPotion&) = delete;
    virtual ~MirrorPotion() = default;

    /// @brief Pushes the piece on source_square directly in front of the activating piece one square backward.
    /// If multiple squares are alligned behind it, then they are pushed as well.
    /// @param board Reference to the board
    /// @param source_square Reference to the square on which piece is firstly positioned
    /// @param target_square Reference to the target_square, not used here.
    /// @param even_or_odd Reference to the string even or odd, only relevent for evenodd potion.
    /// @return Returns false if the piece could not be moved, otherwise true.
    bool triggerEffect(Board& board, Square* source_square, Square* target_square, 
    const std::string& even_or_odd) override;

    /// @brief Returns the number of parameters this potion requires
    /// @return The required number of parameters.
    std::size_t getRequiredParameterCount() const override {return 1;}

    /// @brief Creates a deep copy of Skywalker Potion.
    /// @return A unique_ptr owning a new Skywalker Potion with the same id and display name.
    std::unique_ptr<Item> clone() override {return std::make_unique<MirrorPotion>(id_, display_name_);}
};*/

#endif