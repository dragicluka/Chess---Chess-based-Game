### File Overview
- [Design Document - Description of Approach](Design_Document.md)
- [Milestone 1 - Structure, Config Parsing and Printing](Milestone_1.md)
- [Milestone 2 - Commands, Game Logic and Special Powers](Milestone_2.md)
- [Piece Overview (file listing all pieces and their special powers)](Pieces.md)
- [Items Overview (file listing all items, their powers and usage)](Items.md)
- Error Overview (file listing all errors that must be handled) (this file)

# Errors and Error Ranking

This is a list of all errors that must be handled during execution. All of these errors are also explained in the text where they are relevant; this list is just meant as an additional helpful resource.

## Terminating Errors

After one of these errors is detected, the program should print the error message, free all allocated resources and terminate with the specified return value. If multiple errors occur at the same time, only the one listed highest in the table should be used.

| Return Value | Description                                                                          | Error Message                          |
|--------------|--------------------------------------------------------------------------------------|----------------------------------------|
| 0            | Game was ended with a command or game has ended successfully                         | -                       |
| 1            | Memory could not be allocated                                                        | `Error: Not enough memory!\n`          |
| 2            | Wrong number of command line arguments                                              | `Error: Wrong number of arguments!\n` |
| 3            | Game or message config file (`<FILE_PATH>`) cannot be opened for reading or does not start with correct magic number | `Error: Invalid file (<FILE_PATH>)!\n` | 



## Command Errors

These errors must be handled when validating commands entered by the user. For each invalid command only print one error message. If several errors occur at the same time, only the one that is ranked earliest (lowest Rank number) should be printed.

| Rank | Message Key | Relevant for Command |
|------|-------------|-----------------|
| 1    | `E_UNKNOWN_COMMAND`              | all commands |
| 2    | `E_INVALID_PARAM_COUNT`          | all commands |
| 3    | `E_SPECIAL_USE_UNAVAILABLE`      | special, use |
| 4    | `E_INV_PARAM_PLAYER`             | prison |
| 5    | `E_INV_PARAM_SQUARE`             | starting the game, special, move ambiguity, use |
| 6    | `E_INV_PARAM_PIECE`              | info |
| 7    | `E_PLAYER_PIECE_NOT_FOUND`       | special, use |
| 8    | `E_NO_SPECIAL_POWER`             | special |
| 9    | `E_INVALID_PARAM_COUNT_SPECIAL`  | special |
| 10   | `E_PIECE_FROZEN`                 | special |
| 11   | `E_INV_PARAM_SPECIAL_SQUARE`     | special |
| 12   | `E_INV_PARAM_PIECE_TYPE`         | special |
| 13   | `E_INV_PARAM_TURN_COUNT`         | special |
| 14   | `E_UNWAVERING_FAITH`             | special |
| 15   | `E_INVALID_ARCHER_TARGET`        | special |
| 16   | `E_OPPONENT_PIECE_NOT_FOUND`     | special |
| 17   | `E_INV_PARAM_MOVE`               | move |
| 18   | `E_INVALID_MOVE`                 | move, special |
| 19   | `E_INSUFFICIENT_MANA`            | special |
| 20   | `E_NO_POTION_FOUND`              | use |
| 21   | `E_INVALID_PARAM_COUNT_USE`      | use |
| 22   | `E_INV_PARAM_USE`                | use |
| 23   | `E_INVALID_PASS`                 | pass |
| 24   | `E_INV_PARAM_YES_NO`             | draw |
| 25   | `E_INVALID_PATH`                 | game end  |