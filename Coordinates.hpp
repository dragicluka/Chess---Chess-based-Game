#ifndef COORDINATES_HPP
#define COORDINATES_HPP
#include <iostream>
#include <string>

class  Coordinates
{
    private:
    std::size_t file_;
    std::size_t rank_;
    public:
    Coordinates(std::string& coordinates);
    Coordinates(std::size_t file, std::size_t rank);
    ~Coordinates() = default;

    /// @brief Getter for a file
    /// @return Returns the file value, as std::size_t
    std::size_t getFile() const;

    /// @brief Getter for a rank
    /// @return Returns the rank value, as std::size_t
    std::size_t getRank() const;

    /// @brief Setter for a file
    /// @param file File value to be set.
    void setFile(std::size_t file){file_ = file;};

    /// @brief Setter for a rank
    /// @param rank Rank value to be set
    void setRank(std::size_t rank){rank_ = rank;};
};

#endif