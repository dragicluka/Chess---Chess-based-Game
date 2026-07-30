### File Overview
- [Design Document - Description of Approach](Design_Document.md)
- [Milestone 1 - Structure, Config Parsing and Printing](Milestone_1.md)
- [Milestone 2 - Commands, Game Logic and Special Powers](Milestone_2.md)
- [Piece Overview (file listing all pieces and their special powers)](Pieces.md)
- Items Overview (file listing all items, their powers and usage) (this file)
- [Error Overview (file listing all errors that must be handled)](Errors.md)

# Items - Description and Usage

This file lists all potions and tools, describing their effects, required parameters, and behavior in different situations.

## Potions

Potions are consumable items with active effects that must be explicitly triggered using the `use` command. Once used, a potion is consumed.

> **Note:** If any parameter is invalid, the error message `E_INV_PARAM_USE` is printed.

| Potion Name        | Potion ID | Display Name | Additional Parameters | Description                                                                                            |
|--------------------|-----------|--------------|-----------------------|--------------------------------------------------------------------------------------------------------|
| Freeze             | `FREEZE`  | `*`          | `<SQUARE>`            | Freezes the piece on `<SQUARE>` for 1 turn. If the square is not occupied, the parameter is considered invalid. If a piece is already frozen remove the old freeze effect and apply the new one. |
| Teleport           | `TP`      | `→`          | `<SQUARE>`            | Teleports the activating piece to `<SQUARE>`. A square is invalid if it is offside (similar to football rules), meaning you may only teleport to ranks strictly before the rank of the farthest friendly piece toward the opponent’s side. The square is also invalid if it is occupied. |
| Even/Odd           | `EVENODD` | `½`          | `<SQUARE> <PARITY>`     | Forces an enemy piece on `<SQUARE>` to move only on either `even` or `odd` turns (depending on `<PARITY>`) for the remainder of the game. The command fails if the square is unoccupied, occupied by a friendly piece, or if `<PARITY>` is not a valid value. |
| Skywalker          | `LUKE`    | `↑`          | -                     | Pushes the piece directly in front of the activating piece (relative to the player’s forward direction) one square backward. If multiple pieces are aligned behind it, they are pushed as well. The command fails if there is no piece to push or if the last piece is already on the final rank. A push does not count as a move (so a pawn can still advance two squares if it has not moved yet). |

## Tools

Tools are items with passive effects that are automatically active while they are present in a piece’s inventory. Their effects persist without requiring explicit activation.

| Tool Name          | Tool ID   | Display Name | Description                                                                                            |
|--------------------|-----------|--------------|--------------------------------------------------------------------------------------------------------|
| Shield             | `SHIELD`  | `□`          | If an attacking piece attempts to capture a shielded piece, the shield is destroyed and the attacking piece does not move. The move is still written into the history. |
| Invisibility Cloak | `CLOAK`   | `⌂`          | The equipped piece becomes invisible to the opponent: its square appears empty during the opponent’s turn. However, if an opposing piece moves onto that square, the cloaked piece is still captured. Moves from the cloacked piece are still written to the history. |
| Queen Repellant    | `REPEL`   | `R`          | Any attempt by a queen to capture the equipped piece is considered invalid (`E_INVALID_MOVE`). |