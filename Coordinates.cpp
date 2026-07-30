#include "Coordinates.hpp"

Coordinates::Coordinates(std::size_t file, std::size_t rank) : file_(file), rank_(rank) {}

Coordinates::Coordinates(std::string& coordinates) 
{
  file_ = std::toupper(coordinates[0]) - 'A';
  rank_ = std::toupper(coordinates[1]) - '1';
}

std::size_t Coordinates::getFile() const {return file_;}

std::size_t Coordinates::getRank() const {return rank_;}