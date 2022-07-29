# UrPlusPlus

The Royal Game of Ur with additional gameplay mechanics, fully developed in C++ using OOP principles and design patterns!

## Usage
Requires installation of SDL for graphical user interface. Makefile included.

Running the application:
```
./UrPlusPlus [-AI <AI level>] [-AI <AI level>] [--nogui]
```
Specify up to 2 AI with levels 1/2/3/4; each with increasing difficulty. Leaving out any AI will add a human player instead. The graphical interface can be removed using the --nogui flag.

Once the program is running, the program requires additional input to set up the board:

> Two integers specifying the height (h) and width (w) of the board, respectively

> A (h) x (w) block of characters specifying the tiles on a board. O represents a regular tile, B represents a black hole tile, L represents a lucky tile, T represents a tornado tile, X represents a Null tile (wall), * represents a rosette.

> For each player, two integers representing the starting row and column respectively of the path. Then a series of characters N, E, S, W to indicate the direction to move next in the path. (North, East, South, West respectively)

> An integer specifying the number of tokens (n) each player starts with

> For each player, A sequence of (n) tokens, each of the form (tokenType) (row) (column). Starting at the beginning is denoted with row = column = -1, and starting in the completed state is denoted with row = column = -2. tokenType is defined as follows: A is an assassin, B is a basic token, F is a flexible token, S is a Speedster, G is a supporter token.
Additionally: Speedster must take in an additional fourth parameter, the number of uses of the speedster ability.

> Following this, type __roll__ to roll the dice. To move a token, use __tokenName distance__.

Here is sample input to start a game:
```
3 8
*OOOXX*O
BOT*LOLT
*OOOXX*O
0 3
WWWSEEEEEEENW
2 3
WWWNEEEEEEESW
4
B -1 -1
A -1 -1
S -1 -1 1
A -1 -1
B -1 -1
A -1 -1
S -1 -1 1
A -1 -1
0
```

## Rules
Each player starts with an equal number of tokens off the board. The objective is to race all tokens across the board before your opponent. Tokens are moved one by one, according to the total of four two-sided dice with values 0/1. Paths will overlap, and unless otherwise stated, moving onto an opponent token will send the token back off the board. Some specifics are:
 - Only one token can occupy a space at one time.
 - After a roll, a player must make a move if possible.
 - To complete a path, a player must move a token exactly one past the last tile of the board. Rolling over will result in an invalid move.
 - Rolling a zero or having no invalid moves will pass the turn to the next player.

## Special Tiles
__Rosettes__: Upon landing on, grants another turn to the team of that token. Also protects the occupying token from capture.

__Black Hole__: Upon landing on, send token off the board to the start.

__Lucky Tile__: Upon landing on, there is a small chance the token is sent off the board to the start, and a small chance the token is sent off the board to the finish. This chance is equivalent to rolling the same value on all four dice. (1/16 for each event).

__Tornado Tile__: Upon landing on, there is a 50% chance the token is blown forward and a 50% chance the token is blown backwards. A “blown” token moves along its path until reaching either an empty tile or the start/end of a path. If the empty tile is itself a special tile, the tile ability is not activated.

## Special Tokens
__The Assassin__ - Gains another turn when an enemy token is captured

__The Flexible__ - Has access to an alternative dice roll when making its move

__The Speedster__ - Has a once-per-game ability to move one more than the regular dice roll

__The Supporter__ - A token that protects the first allied token within 3 tiles in the forward direction


## AI
There are four levels of AI:
 - __Level 1__: Makes random valid moves.
 - __Level 2__: Prioritizes finishing tokens and capturing enemy tokens.
 - __Level 3__: (along with developments in level 2) Also considers tile effects like avoiding black holes, preferring rosettes.
 - __Level 4__: Extensions to level 3:
   - Tornado tiles and Lucky tiles are dynamically scored based on the position of other tokens and the position of the tile itself
   - Strongly prefers occupying Rosettes that are on shared paths
   - Avoids moving pieces 1, 2, or 3 squares in front of an opponent
   - Effectively utilizes token abilities (Captures with assassin, protects tokens with supporter, etc)

Thanks for reading!
