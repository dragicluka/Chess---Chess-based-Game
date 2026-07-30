#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sstream>

class Utils
{
  public:
    Utils() = delete;
    Utils(const Utils& other) = delete;
    ~Utils() = delete;

    /// @brief Removes leading whitespace from the beginning of a string.
    /// @param string Reference to the string to be trimmed in place.
    static void trimStart(std::string& string);

    /// @brief Removes trailing whitespace from the end of a string.
    /// @param string Reference to the string to be trimmed in place.
    static void trimEnd(std::string& string);

    /// @brief Calls trimEnd and TrimStart
    /// @param string Reference to the string to be trimmed in place.
    static void trim(std::string& string);

    /// @brief Makes all characters in string lowercase.
    /// @param user_input Reference to the string, whose characters should become lowercase.
    static void toLowerCase(std::string& user_input);

    /// @brief Makes all characters in string uppercase.
    /// @param user_input Reference to the string, whose characters should become uppercase.
    static void toUpperCase(std::string& user_input);

    /// @brief Splits the string in substring based on the given delimeter.
    /// @param user_input Reference to the string that should be splitted
    /// @param delimeter Char that determines, where the string will be splitted.
    /// @param is_trimmed Boolean value that determines if the substrings should be trimmed or not.
    /// @return Returns vector of substrings, made out of user_input.
    static std::vector<std::string> splitString(std::string& user_input, char delimeter, bool is_trimmed);

    /// @brief Parses the id list of pieces that will be later used in loadGameConfigFile function
    /// @param piece_line Represents a reference to the part of file where pieces are listed.
    /// @return Returns the vector of strings, which will be later converted to pieces in loadGameConfigFile.
    static std::vector<std::string> parseIdList(std::string& piece_line);
};

#endif