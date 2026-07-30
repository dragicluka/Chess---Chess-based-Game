#include "Utils.hpp"

// OOP1 Tutors
// from Utils.cpp file in A1 assignment
// begin
void Utils::trimStart(std::string& string)
{
  std::size_t start = string.find_first_not_of(' ');
  string = start == std::string::npos ? "" : string.substr(start);
}

void Utils::trimEnd(std::string& string)
{
  std::size_t end = string.find_last_not_of(' ');
  string = end == std::string::npos ? "" : string.substr(0, end + 1);
}

void Utils::trim(std::string& string)
{
  trimStart(string);
  trimEnd(string);
}
// end



void Utils::toLowerCase(std::string& user_input)
{
  for(auto& character : user_input)
  {
    character = tolower(character);
  }
}



void Utils::toUpperCase(std::string& user_input)
{
  for(auto& character : user_input)
  {
    character = toupper(character);
  }
}



std::vector<std::string> Utils::splitString(std::string& user_input, char delimeter, bool is_trimmed)
{
  std::istringstream input_stream(user_input);

  std::vector<std::string> input_tokens;

  std::string token;

  while(getline(input_stream, token, delimeter))
  {
    if(is_trimmed)
      trim(token);
    
    if(!token.empty())
      input_tokens.push_back(token);
  }

  return input_tokens;
}



std::vector<std::string> Utils::parseIdList(std::string& piece_line)
{
  std::vector<std::string> pieces;

  bool is_trimmed = true;
  std::vector<std::string> tokens = Utils::splitString(piece_line, ',', is_trimmed);

  for(auto& token : tokens)
  {
    Utils::trim(token);

    std::size_t x_position = token.find('x');

    if(x_position != std::string::npos)
    {
      std::size_t piece_count = std::stoi(token.substr(0, x_position));
      std::string piece_id = token.substr(x_position + 1);
      for(std::size_t index = 0; index < piece_count; index++)
      {
        pieces.push_back(piece_id);
      }
    }
    
    else
    {
      pieces.push_back(token);
    }
  }

  return pieces;
}