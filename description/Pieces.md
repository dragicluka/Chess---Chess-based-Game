### File Overview
- [Design Document - Description of Approach](Design_Document.md)
- [Milestone 1 - Structure, Config Parsing and Printing](Milestone_1.md)
- [Milestone 2 - Commands, Game Logic and Special Powers](Milestone_2.md)
- Piece Overview (file listing all pieces and their special powers) (this file)
- [Items Overview (file listing all items, their powers and usage)](Items.md)
- [Error Overview (file listing all errors that must be handled)](Errors.md)

# Pieces - Movement and Special Powers

This file describes all game pieces, including their standard movement rules, attributes (type, ID, value, short name), and any special abilities, along with the syntax and error handling for using those abilities.

### How to read Piece Descriptions?
Each Piece has some key elements, which are explained here:
- **Piece Type**: Specifies the class of the piece (i.e. pawn, rook, knight, bishop, queen, or king).
                  Used when printing information or promoting a pawn.
- **Piece ID**: Unique piece identification. Will be used when reading the config, or interacting with some commands.
- **Short Name**: The characters printed for the piece. Is comprised of the standard class short name and an added letter.
- **Value**: The value of the piece.

## Pawn

**Piece Type**: `P`

**Piece ID**: `P`

**Short Name** (`PAWN_SHORT_NAME`): `♟P` for white pawns, `♟p` for black pawns.

**Value**: `1`

<details>
<summary><h3>Basic Movement</h3></summary>

A pawn always moves forward. For white pawns, this means moving to a higher rank; for black pawns, to a lower rank.

When not capturing, a pawn moves straight forward on the same file:
- If the pawn has not moved yet, it may advance either one or two squares.
- Otherwise, it may advance exactly one square.

When capturing, a pawn moves diagonally forward exactly one square.

**En passant** is a special capture that is only available immediately after an opposing pawn moves forward two squares in a single move. In this case, a pawn may capture that pawn as if it had moved only one square forward.

All other pawn moves are considered invalid and result in printing the error message `E_INVALID_MOVE`.

**Promotion** occurs immediately when a pawn reaches the opponent’s back rank. The pawn must be promoted to one of the following pieces: rook, knight, bishop, or queen. Only standard chess pieces without special abilities are allowed.
The piece to promote to must be specified as part of the move command (see [Milestone 2](Milestone_2.md#commands) > Command: move).

```
       Move               Capture                        En Passant           
+----+----+----+     +----+----+----+       +----+----+----+     +----+----+----+
|    |  ° |    |     |  ° |    |  ° |       |    |    |    |     |    |    |    | 
+----+----+----+     +----+----+----+       +----+----+----+     +----+----+----+
|    | ♟P |    |     |    | ♟P |    |       |  ° |    |    |  -> | ♟P |    |    | 
+----+----+----+     +----+----+----+       +----+----+----+     +----+----+----+
|    |    |    |     |    |    |    |       | ♟p | ♟P |    |     |    |    |    | 
+----+----+----+     +----+----+----+       +----+----+----+     +----+----+----+
```

> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Golden Pawn</h3></summary>

 **Piece ID**: `PGLD`

 **Short Name**: `<PAWN_SHORT_NAME>g`

 **Special Power Type**: Passive

When a golden pawn reaches the opponent’s back rank, it does not promote. Instead, the current player immediately wins the game.
The promotion syntax must still be used to reach the back rank.

</details>

<details>
<summary><h3>Impatient Pawn</h3></summary>

 **Piece ID**: `PIPT`

 **Short Name**: `<PAWN_SHORT_NAME>i`

 **Special Power Type**: Active

 **Mana Cost**: Variable

The impatient pawn can promote before reaching the opponent’s back rank by using the `special` command.

**Syntax**: `special <SQUARE> <PIECE_TYPE>`

`<PIECE_TYPE>` must be one of `R`, `N`, `B` or `Q` and specifies the piece type that the pawn promotes to.

The mana cost is calculated as:

```
Mana Cost = rank_difference * piece_value
```

Where:
- `rank_difference` is the absolute distance between the pawn’s current rank and the opponent’s back rank
- `piece_value` is the value of the selected piece type

In addition to the general `special` command errors, the following must be handled:

| Rank | Error Description                                                    | Message Key                    |
|------|----------------------------------------------------------------------|--------------------------------|
| 12 | The piece type is not one of `R`, `N`, `B`, or `Q`  | `E_INV_PARAM_PIECE_TYPE`           |


</details>

<details>
<summary><h3>Stubborn Pawn</h3></summary>

 **Piece ID**: `PSTB`

 **Short Name**: `<PAWN_SHORT_NAME>+`

 **Special Power Type**: Active

 **Mana Cost**: 5

The stubborn pawn can capture directly forward using the `special` command.

If such a capture ends on the opponent’s back rank, the pawn does **not** promote and instead remains a pawn. This exception applies only to captures performed via the `special` command, normal moves still result in promotion.


 **Syntax**: `special <SQUARE>`

In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                    | Message Key                    |
 |------|----------------------------------------------------------------------|--------------------------------|
 | 18 | The target square (directly in front of `<SQUARE>`) does not exist or does not contain an opponent piece | `E_INVALID_MOVE` |
</details>

<details>
<summary><h3> Nervous Pawn</h3></summary>

 **Piece ID**: `PNRV`

 **Short Name**: `<PAWN_SHORT_NAME>-`

 **Special Power Type**: Active

 **Mana Cost**: 1

The nervous pawn can move one square backward using the `special` command.

**Syntax**: `special <SQUARE>`

In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                    | Message Key                    |
 |------|----------------------------------------------------------------------|--------------------------------|
| 18  | The target square (directly behind `<SQUARE>`) is not empty      | `E_INVALID_MOVE` |
</details>


<details>
<summary><h3> Explosive Pawn</h3></summary>

 **Piece ID**: `PEXP`

 **Short Name**: `<PAWN_SHORT_NAME>!`

 **Special Power Type**: Active

 **Mana Cost**: 3

When the explosive pawn captures a piece using its special ability, it additionally captures all **non-pawn** pieces in the 8 surrounding squares. This includes both enemy and friendly pieces.

After the explosion, the explosive pawn itself is also removed from the board (is a form of capture). All items held by captured pieces are discarded.
The pawn is removed even if it has a shield equipped.

```
    Before Capture         After Capture (axb2)
  --+----+----+----+         +----+----+----+
  3 | ♟p | ♞n | ♟p |         | ♟p |    | ♟p |
  --+----+----+----+         +----+----+----+
  2 | ♛q | ♟p |    |         |    |    |    |
  --+----+----+----+         +----+----+----+
  1 | ♟P!|    | ♜R |         |    |    |    |
  --+----+----+----+         +----+----+----+
    |  A |  B |  C |         |  A |  B |  C |
```

If one of the surrounding pieces is a king, the game ends as if the king had been captured normally.
It is therefore possible for the active player to lose the game with this move.
If both kings are captured by the explosion the game ends in a draw.

In case a piece with the shield tool (see [Items](Items.md#tools)) equipped is captured, the pawn does not move, but still explodes.
If a shielded piece is in the blast radius of the explosive pawn, the shield is destroyed regardless of it being a pawn or not.
Note that the attempted capture occurs and is resolved before the explosion. Therefore, if the explosive pawn attempts to capture a shielded piece, the initial capture fails, but since the shield is now destroyed, the subsequent explosion may still capture the piece.

**Syntax**: `special <SQUARE> <TARGET_SQUARE>`

In addition to the general `special` command errors, the following must be handled:

| Rank | Error Description                                                    | Message Key                    |
|------|----------------------------------------------------------------------|--------------------------------|
| 11 | The target square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
| 18 | The target square does not correspond to a valid pawn capture (diagonal and contains opponent piece)        | `E_INVALID_MOVE`               |


</details>


## Rook

**Piece Type**: `R`

**Piece ID**: `R`

**Short Name** (`ROOK_SHORT_NAME`): `♜R` for white pieces, `♜r` for black pieces 

**Value**: `5`

<details>
<summary><h3>Basic Movement</h3></summary>

The rook may move any number of squares along its current file or rank, provided all intermediate squares are empty.

All other moves are considered invalid and result in `E_INVALID_MOVE`.

```
+----+----+----+----+----+
|    |    |  ° |    |    | 
+----+----+----+----+----+
|    |    |  ° |    |    | 
+----+----+----+----+----+
|  ° |  ° | ♜R |  ° |  ° | 
+----+----+----+----+----+
|    |    |  ° |    |    | 
+----+----+----+----+----+
|    |    |  ° |    |    | 
+----+----+----+----+----+
```

> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Invincible Rook</h3></summary>

 **Piece ID**: `RINV`

 **Short Name**: `<ROOK_SHORT_NAME>i`

 **Special Power Type**: Active

 **Mana Cost**: `<TURN_COUNT>`

The invincible rook becomes immobile and cannot be captured for `<TURN_COUNT>` turns. It is no longer considered invincible once the owning player has completed the specified number of turns, excluding the turn in which the rook became invincible.

**Syntax**: `special <SQUARE> <TURN_COUNT>`

In addition to the general `special` command errors, the following must be handled:

| Rank | Error Description                                                    | Message Key                    |
|------|----------------------------------------------------------------------|--------------------------------|
| 13 | The turn count is not a positive number greater than 0               | `E_INV_PARAM_TURN_COUNT`       |

</details>

<details>
<summary><h3>Painter Rook</h3></summary>

 **Piece ID**: `RPNT`

 **Short Name**: `<ROOK_SHORT_NAME>p`

 **Special Power Type**: Active

 **Mana Cost**: Variable

The painter rook can either:
- move normally using the `move` command, or
- use `special` to move to `<TARGET_SQUARE>` while painting all squares along its path including the starting square.

The rook has an inherent color, which determines the color applied to painted squares:
- If the rook starts on a regular square, its inherent color is the color of that square.
- If the rook starts on a special square, its inherent color is determined by the piece’s color.

 **Syntax**: `special <SQUARE> <TARGET_SQUARE>`

The mana cost is equal to the distance between `<SQUARE>` and `<TARGET_SQUARE>` (i.e., the number of squares moved).

> **Notes:**
>  - The special ability can also be used to perform a capture.
>  - A special square can be painted over, and loses its special ability (becoming a normal square with the new color, and is printed as such).
 
In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                    | Message Key                    |
 |------|----------------------------------------------------------------------|--------------------------------|
 | 11 | The target square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
 | 18 | The target square is not reachable by a valid rook move                 | `E_INVALID_MOVE`               |

</details>

## Knight

**Piece Type**: `N`

**Piece ID**: `N`

**Short Name** (`KNIGHT_SHORT_NAME`): `♞N` for white pieces, `♞n` for black pieces 

**Value**: `3`

<details>
<summary><h3>Basic Movement</h3></summary>

The knight moves in an L-shape: two squares in one direction followed by one square perpendicular to that direction.

Intermediate squares do **not** need to be empty, as the knight can jump over other pieces.

```
+----+----+----+----+----+
|    |  ° |    |  ° |    | 
+----+----+----+----+----+
|  ° |    |    |    |  ° | 
+----+----+----+----+----+
|    |    | ♞N |    |    | 
+----+----+----+----+----+
|  ° |    |    |    |  ° | 
+----+----+----+----+----+
|    |  ° |    |  ° |    | 
+----+----+----+----+----+
```

> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Jumpy Knight</h3></summary>

 **Piece ID**: `NJMP`

 **Short Name**: `<KNIGHT_SHORT_NAME>j`

 **Special Power Type**: Passive

The jumpy knight moves in an extended L-shape: three squares in one direction followed by two squares perpendicular to that direction.

As with the normal knight, intermediate squares do **not** need to be empty.

```
--+----+----+----+----+----+----+----+----+
8 |    |    |    |    |    |    |    |    | 
--+----+----+----+----+----+----+----+----+
7 |    |  ° |    |    |    |  ° |    |    | 
--+----+----+----+----+----+----+----+----+
6 |  ° |    |    |    |    |    |  ° |    | 
--+----+----+----+----+----+----+----+----+
5 |    |    |    |    |    |    |    |    | 
--+----+----+----+----+----+----+----+----+
4 |    |    |    | ♞Nj|    |    |    |    | 
--+----+----+----+----+----+----+----+----+
3 |    |    |    |    |    |    |    |    | 
--+----+----+----+----+----+----+----+----+
2 |  ° |    |    |    |    |    |  ° |    | 
--+----+----+----+----+----+----+----+----+
1 |    |  ° |    |    |    |  ° |    |    | 
--+----+----+----+----+----+----+----+----+
  |  A |  B |  C |  D |  E |  F |  G |  H | 
```


> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Ice Knight</h3></summary>

 **Piece ID**: `NICE`

 **Short Name**: `<KNIGHT_SHORT_NAME>i`

 **Special Power Type**: Passive

Whenever the ice knight moves, it freezes all pieces it jumps over for 1 turn. This includes both friendly and enemy pieces.
If a piece is already frozen, the old freeze is removed and the new one is applied, hence resetting the round counter.

For determining the affected squares, the movement is interpreted stepwise:
- First, the knight moves two squares in one direction.
- Then, it moves one square perpendicular to that direction.

All squares traversed during this movement (excluding the starting and destination squares) are affected by the freezing effect.

The following diagrams illustrate the affected squares:

```
+----+----+----+----+----+     +----+----+----+----+----+
|    |  ° |  ° |    |    |     |    |    |  ° |  ° |    | 
+----+----+----+----+----+     +----+----+----+----+----+
|  . |    |  ° |    |    |     |    |    |  ° |    |  * | 
+----+----+----+----+----+     +----+----+----+----+----+
|  . |  . | ♞Ni|  * |  * |     |  . |  . | ♞Ni|  * |  * | 
+----+----+----+----+----+     +----+----+----+----+----+
|    |    |  + |    |  * |     |  . |    |  + |    |    | 
+----+----+----+----+----+     +----+----+----+----+----+
|    |    |  + |  + |    |     |    |  + |  + |    |    | 
+----+----+----+----+----+     +----+----+----+----+----+
```

> **Legend**:
>  - `°` &mdash; Movement (2 up, 1 left / right)
>  - `+` &mdash; Movement (2 down, 1 left / right)
>  - `.` &mdash; Movement (2 left, 1 up / down)
>  - `*` &mdash; Movement (2 right, 1 up / down)

</details>

## Bishop

**Piece Type**: `B`

**Piece ID**: `B`

**Short Name** (`BISHOP_SHORT_NAME`): `♝B` for white pieces, `♝b` for black pieces 

**Value**: `3`

<details>
<summary><h3>Basic Movement</h3></summary>

Each bishop has an inherent color determined by the square it occupies at the start of the game.  
If the bishop starts on a special square, its inherent color is determined by the color of the piece itself.

The bishop may move any number of squares in a straight line while remaining on squares matching its inherent color, provided all intermediate squares are empty.

Under standard conditions, this corresponds to diagonal movement. However, since square colors may change during the game, valid movement may also appear horizontal or vertical.

Special squares are considered both black and white. Therefore, a bishop may always move over or land on a special square, regardless of its inherent color.

All other moves are considered invalid and result in `E_INVALID_MOVE`.

```
+----+----+----+----+----+
|  ° |    |    |    |  ° | 
+----+----+----+----+----+
|    |  ° |    |  ° |    | 
+----+----+----+----+----+
|    |    | ♝B |    |    | 
+----+----+----+----+----+
|    |  ° |    |  ° |    | 
+----+----+----+----+----+
|  ° |    |    |    |  ° | 
+----+----+----+----+----+
```

> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Color Blind Bishop</h3></summary>

 **Piece ID**: `BCLR`

 **Short Name**: `<BISHOP_SHORT_NAME>c`

 **Special Power Type**: Active

 **Mana Cost**: 3

The Color Blind Bishop may move to or capture on any adjacent square using its special ability, ignoring color restrictions.

If the destination square’s color differs from the bishop’s inherent color, the bishop’s inherent color is updated to match the destination square. Special squares do not affect the bishop’s inherent color.

 **Syntax**: `special <SQUARE> <TARGET_SQUARE>`
 
In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                    | Message Key                    |
 |------|----------------------------------------------------------------------|--------------------------------|
 | 11 | The target square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
 | 18 | The target square is not adjacent to `<SQUARE>` or contains a friendly piece | `E_INVALID_MOVE`               |

</details>

<details>
<summary><h3>Preacher Bishop</h3></summary>

 **Piece ID**: `BPRC`

 **Short Name**: `<BISHOP_SHORT_NAME>p`

 **Special Power Type**: Active

 **Mana Cost**: Variable &mdash; Three times the value of the targeted piece type

The Preacher Bishop can convert an opponent piece to the current player’s color.

The converted piece retains all of its properties, including status effects (e.g., frozen) and inherent color.

> **Notes:**
> - This effect can only be reversed by another Preacher Bishop.
> - Preacher Bishops are immune to conversion.
> - Only opponent pieces that could be captured by a valid bishop move may be targeted.


 **Syntax**: `special <SQUARE> <TARGET_SQUARE>`
 
In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                    | Message Key                    |
 |------|----------------------------------------------------------------------|--------------------------------|
 | 11 | The target square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
 | 14 | The target square is occupied by another Preacher Bishop             | `E_UNWAVERING_FAITH`           |
 | 18 | The target square is empty or not reachable by a valid bishop move   | `E_INVALID_MOVE`               |

</details>

## Queen

**Piece Type**: `Q`

**Piece ID**: `Q`

**Short Name** (`QUEEN_SHORT_NAME`): `♛Q` for white pieces, `♛q` for black pieces 

**Value**: `9`

<details>
<summary><h3>Basic Movement</h3></summary>

The queen may move any number of squares along its current file, rank, or along a diagonal, provided all intermediate squares are empty.

All other moves are considered invalid and result in `E_INVALID_MOVE`.

```
+----+----+----+----+----+
|  ° |    |  ° |    |  ° | 
+----+----+----+----+----+
|    |  ° |  ° |  ° |    | 
+----+----+----+----+----+
|  ° |  ° | ♛Q |  ° |  ° | 
+----+----+----+----+----+
|    |  ° |  ° |  ° |    | 
+----+----+----+----+----+
|  ° |    |  ° |    |  ° | 
+----+----+----+----+----+
```

> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Flipper Queen</h3></summary>

 **Piece ID**: `QFLP`

 **Short Name**: `<QUEEN_SHORT_NAME>f`

 **Special Power Type**: Active

 **Mana Cost**: Variable &mdash; Count of squares from the bounce point to and including `<TARGET_SQUARE>`

The flipper queen moves like a normal queen when using the `move` command.

When using `special`, the queen may perform a diagonal move that includes exactly one bounce off a board edge (top, bottom, left, or right). After the bounce, the queen continues moving diagonally.

This allows the queen to reach squares that are not directly accessible via standard movement.

```
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+ 
8 |    |    | *° |    | .° |    |    |    |         8 |    | *°.|    |    |    |    |    |    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
7 |    | *° |    |    |    | .° |    |    |         7 | *! |    |  °.|    |    |    |    |    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
6 | *! |    |    |    |    |    | .° |    |         6 |    |  * |    |  °.|    |    |    |    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
5 |    |  * |    |    |    |    |    | .! |         5 |    |    |  * |    |  °.|    |    |    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
4 | #° |    |  * |    |    |    |  . |    |         4 |    |    |    |  * |    |  °.|    |    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
3 |    | #° |    |  * |    |  . |    | +° |         3 |    |    |    |    |  * |    |  °.|    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
2 |    |    | #° |    | ♛Qf|    | +° |    |         2 |    |    |    |    |    |  * |    |  !.| 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+
1 |    |    |    | #! |    | +! |    |    |         1 |    |    |    |    |    |    | ♛Qf|    | 
--+----+----+----+----+----+----+----+----+         --+----+----+----+----+----+----+----+----+ 
  |  A |  B |  C |  D |  E |  F |  G |  H |           |  A |  B |  C |  D |  E |  F |  G |  H | 
```

> **Legend**:
>  - `°` &mdash; Square representing a valid flipper move i.e. moves after a bounce (`<TARGET_SQUARE>`).
>  - `!` &mdash; Bounce Squares (`<BOUNCE_SQUARE>`).
>  - `*` &mdash; Potential flipper movement pattern: upper left diagonal.
>  - `.` &mdash; Potential flipper movement pattern: upper right diagonal.
>  - `#` &mdash; Potential flipper movement pattern: lower left diagonal.
>  - `+` &mdash; Potential flipper movement pattern: lower right diagonal.

 **Syntax**: `special <SQUARE> <BOUNCE_SQUARE> <TARGET_SQUARE>`

- `<BOUNCE_SQUARE>` must lie on the board edge and represent the point at which the queen changes direction. It is not counted toward the mana cost.
- The combined path (before and after the bounce) must form a valid continuous diagonal trajectory.
- All intermediate squares along the path must be valid according to movement rules.

In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                          | Message Key                    |
 |------|----------------------------------------------------------------------------|--------------------------------|
 | 11 | The target square or bounce square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
 | 18 | `<BOUNCE_SQUARE>` and `<TARGET_SQUARE>` do not define a valid bounced diagonal path | `E_INVALID_MOVE`               |
</details>

<details>
<summary><h3>Jumpy Queen</h3></summary>

 **Piece ID**: `QJMP`

 **Short Name**: `<QUEEN_SHORT_NAME>j`

 **Special Power Type**: Active

 **Mana Cost**: 2

When using the `special` command, the queen may perform a single knight move.

 **Syntax**: `special <SQUARE> <TARGET_SQUARE>`
 
In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                    | Message Key                    |
 |------|----------------------------------------------------------------------|--------------------------------|
 | 11 | The target square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
 | 18 | The target square is not reachable by a valid knight move            | `E_INVALID_MOVE`               |
</details>

<details>
<summary><h3>Hungry Queen</h3></summary>

 **Piece ID**: `QHNGR`

 **Short Name**: `<QUEEN_SHORT_NAME>h`

 **Special Power Type**: Passive

The hungry queen may capture friendly pieces in addition to enemy pieces. The only exception is the friendly king, which cannot be captured.
Attempting to capture the friendly king results in the error message `E_INVALID_MOVE`.
</details>

## King

**Piece Type**: `K`

**Piece ID**: `K`

**Short Name** (`KING_SHORT_NAME`): `♚K` for white pieces, `♚k` for black pieces 

**Value**: `0`

<details>
<summary><h3>Basic Movement</h3></summary>

The king may move to any neighboring square, capturing enemy pieces if present.

The king also has a special move: **castling**. The move is allowed if all of the following conditions are met:

1. The king has not moved since the start of the game.
2. A rook on the same rank has not moved since the start of the game.
3. All squares between the king and rook are empty.
4. If the rook is directly adjacent to the king, the square immediately beyond the rook (away from the king) is empty.
5. The king is not in check, and the square the king moves through is not under attack.

If valid, the king moves exactly **two squares** toward the rook, and the rook moves to the square the king skipped. The move is denoted by the king’s target square.

```
                                            Castling
+----+----+----+----+----+        +----+----+----+----+    +----+----+----+----+
|    |    |    |    |    |        |    |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+        +----+----+----+----+    +----+----+----+----+
|    |  ° |  ° |  ° |    |        | ♚K |    |  ° | ♜R |  ->|    | ♜R | ♚K |    | 
+----+----+----+----+----+        +----+----+----+----+    +----+----+----+----+
|    |  ° | ♚K |  ° |    |        |    |    |    |    |    |    |    |    |    | 
+----+----+----+----+----+        +----+----+----+----+    +----+----+----+----+
|    |  ° |  ° |  ° |    |         
+----+----+----+----+----+        
|    |    |    |    |    |  
+----+----+----+----+----+
```

> **Legend**:
>  - `°` &mdash; Square representing a valid move.

</details>

<details>
<summary><h3>Frightened King</h3></summary>

 **Piece ID**: `KFRT`

 **Short Name**: `<KING_SHORT_NAME>f`

 **Special Power Type**: Passive

If the Frightened King is in **check** at the start of a turn, it **cannot capture any pieces** during that turn.  

However, if the player chooses to move the king while in check, the king may move a **second time** (see [Milestone 1](Milestone_1.md#important-rules) > Important Rules > Moving Multiple Times).

</details>

<details>
<summary><h3>Archer King</h3></summary>

 **Piece ID**: `KARC`

 **Short Name**: `<KING_SHORT_NAME>a`

 **Special Power Type**: Active

 **Mana Cost**: Variable &mdash; chessboard distance between `<SQUARE>` and `<TARGET_SQUARE>`

The Archer King can shoot a **frozen arrow** at an opponent piece, freezing it for 1 turn.
- The arrow flies over all other pieces.
- The arrow can only be shot **forward**, along the same file toward the opponent's back rank.
- If the piece is already frozen, the freeze effect is reset.

**Syntax**: `special <SQUARE> <TARGET_SQUARE>`

In addition to the general `special` command errors, the following must be handled:

 | Rank | Error Description                                                                                                     | Message Key      |
 |------|-----------------------------------------------------------------------------------------------------------------------|------------------|
 | 11 |  The target square could not be parsed                                | `E_INV_PARAM_SPECIAL_SQUARE`           |
 | 15 | The target square is not directly in front of the king (same file, higher/lower rank depending on color) | `E_INVALID_ARCHER_TARGET` |
 | 16 | The target square does not contain a piece belonging to the opponent  | `E_OPPONENT_PIECE_NOT_FOUND`    |

</details>

