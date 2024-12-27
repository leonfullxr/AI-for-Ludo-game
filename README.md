# Parchís Mario Bros Style 2022/2023 Project
<p style="text-align: center;">
  <img src="https://github.com/leonfullxr/AI-for-Ludo-game/blob/main/data/textures/product-mario-party.jpg" alt="portrait image" style="margin-right: 10px;">
  <img src="https://github.com/leonfullxr/AI-for-Ludo-game/blob/main/doc/img/ludo_game.png" width="40%" alt="main menu image">
</p>

## Introduction
To adapt the popular Parchís game to the requirements of the subject, the random behavior of rolling a die is replaced by choosing a die from the available ones. The set of available dice will, a priori, consist of the 6 values of a regular die. Each time one of the dice is used, that value is expended, and the player must choose a different die in the next turn. When all values of the die have been spent, it regenerates completely. Eventually, special values like 10 or 20 will be added to the classic die values to be used at specific moments.

In addition, instead of each player choosing a color, allowing for 2-4 players to play, in this case, there will always be 2 players playing with two alternating colors. Whenever it's a player's turn, with the chosen die, they can move a piece of any of their two colors. Even though each player controls two colors, these colors can attack each other. For example, even if a player is playing with yellow and red, when moving a yellow piece to an unsafe square where there was previously a red one, the yellow piece captures the red piece, even if they belong to the same player.

The goal of PARCHÍS is to get ALL pieces of one of our colors into their destination square. The player who manages to get ALL pieces of ANY of their colors into the destination square wins the game, regardless of where the rest of their pieces are.

## Prerequisites
The software uses the [SFML library](https://www.sfml-dev.org/index.php), so you must install it according to your Linux version:
- On Ubuntu: `sudo apt install libsfml-dev`
- On MacOS: `brew install sfml`
- Ubuntu/Debian: `sudo apt install libsfml-dev`
- Arch: `sudo pacman -S sfml`
- Fedora/CentOS/Suse: `sudo dnf -y install SFML-devel` or `sudo yum -y install SFML-devel`

If none of these options work for you, there are also other [installation options](https://www.sfml-dev.org/tutorials/2.5/start-linux.php).

## Game Rules
The game rules are the same as those of classic Parchís, with the following improvements/new items derived from Mario Bros:

### Rules
1. Two players play with two colors each. Player 1 (yellow and red) vs Player 2 (blue and green). The first player to get ALL pieces of ANY of their two colors to the goal wins. In this version of the game, the board will have only 3 pieces for each color.

2. The order of play is Yellow → Blue → Red → Green.

3. In each turn, the player chooses a die of the corresponding color and moves any piece of either of their two colors. The chosen die is spent and cannot be used again until all dice are spent. Initially available dice are numbers 1 to 6, except for the number 3.

4. To move a piece out of the house, a 5 must be rolled.

5. If a 6 is rolled, roll again with the same color.
6. When a 6 is rolled and all pieces of that color are out of the house, move 7 squares instead of 6.
7. There cannot be more than two pieces on the same square, except in the home or goal. If there are already two, a third piece cannot move to that square with its roll.
8. Two pieces of the same color on the same square form a barrier. A piece of another color cannot pass this barrier until it is broken. A piece of the same color can jump over the barrier.

9. If a 6 is rolled, and there is a barrier of the corresponding color, it is mandatory to break that barrier. A piece not from a barrier cannot be moved.

10. When a piece reaches an unsafe square with a piece of another color, it captures that piece. The captured piece returns home. The player who captures the piece scores 20 with any piece of their color. This is managed in an additional turn where the player only has the +20 movement available.

11. Two colors of the same player can capture each other. For example, a yellow piece can capture a red one if the situation arises, even if both belong to Player 1.

12. In safe squares (marked with a circle), two pieces of different colors can coexist. A piece on these squares cannot be captured. Even if there are two pieces of different colors on a safe square, they do not act as a barrier, meaning any other piece can jump over that square.

13. To reach the goal, the exact number of squares remaining must be rolled. If more are rolled, the piece bounces back and starts counting the excess squares backward.

14. The total number of bounces a color can make throughout a game is limited to 30. If this number is exceeded, the player loses the game. This is an artificial rule designed to prevent infinite games.

15. When a piece reaches the goal, score 10 with any other piece of the same color. This is managed in an additional turn where the player only has the +10 movement available.

16. In each turn, it is mandatory to choose a die that has not been used. If the chosen die's value does not allow any piece to move, it can be spent, and the turn can be passed without making any moves.

17. To expedite games, all pieces will not start in the home at the beginning of the game but organized as seen in Figure 2.

18. There is a total of 10 special dice that can be used, in addition to the classic dice from 1 to 6. At the start of the game, they will be distributed across the board as collectible objects, in the arrangement shown in Figure 2. When a player lands on a square with one of these objects, they will acquire the associated special die, and the object will disappear from the board for the rest of the game. The special die can be used at any time and is for single use. They are independent of the dice from 1 to 6 in the sense that the regular dice are used and renewed as specified in rule 3 independently of the special dice a player may have. Each special die has a different effect. Some dice give the piece a special power, others allow long-distance movements, and others allow setting traps or attacking enemy pieces based on their position. The 10 special dice, which will be described starting in the following rules, are: banana, mushroom, horn, red shell, blue shell, bullet, lightning, boo, star, and megamushroom.

19. Each player can have a maximum of 2 special dice in their hand. If they land on another square that provides a die while already having 2, that die will not be picked up and will remain on the board.

20. As a consequence of some of the special dice, pieces may also acquire a special state. Possible special states for pieces are: normal piece, flattened piece, mini piece, ghost piece, invincible piece, and megapiece. The following rules describe how these states can be acquired and how each type of piece interacts with the others.

21. The banana is a special die that places a trap on the board. It is applied to a piece. On the square where the piece is, a banana trap is placed.
22. When a piece (of any color) is making a move, and there is a banana trap along the path, the piece will stay on the trap square, and the trap will disappear. The player who made the move loses the turn, even if they rolled a 6. Additionally, the piece will enter the flattened state for one turn.
23. A flattened piece cannot move or capture (this includes the turn it falls into the trap). If a normal die is attempted to be applied to this piece, the die will be spent, and the turn will advance without the piece moving.
24. Invincible, ghost, and megapieces are immune to the banana trap, but they do not eliminate them from the board if they pass over or land on them.

25. The mushroom is a special die that allows a piece to advance more than usual. When used, the chosen piece will advance a minimum of 8 squares, regardless of the piece's current state. Additionally, if the destination square is occupied, the piece will continue advancing to an empty square. If the piece had any special state, it will retain it after using the mushroom. If the goal is surpassed with this object, the piece will not bounce back and will stay at the goal.

26. The red shell is a special die that allows eliminating pieces of a different color from the board. When applied to a piece, it will automatically send the nearest piece in front of it back home, ignoring barriers and/or safe squares. If there are several at the same distance, it will eliminate all of them.

27. The blue shell is a special die that allows eliminating pieces of a different color from the board. When applied to a piece, it will automatically send the piece closest to the goal (that has not reached it yet) back home, ignoring barriers and/or safe squares. If there are several at the same distance, it will eliminate all of them.

28. The horn is a special die that can be used in two different ways. Firstly, if about to receive the impact of a red or blue shell when having this die, the horn blocks the hit, and the die disappears. Secondly, if decided to apply the die to a piece, an explosion occurs, sending all pieces within two squares or less of the selected piece back home.
29. Invincible, ghost, megapieces, and pieces of the same color are not affected by the horn's explosion.
30. Invincible and megapieces are not affected by the effects of both red and blue shells. Ghost pieces are ignored when shells are used, and the target piece becomes the next closest to the goal, respectively.

31. The bullet is a special die that allows a piece to advance more than usual. When used, the chosen piece will advance a minimum of 40 squares, regardless of the piece's current state. Additionally, if the destination square is occupied, the piece will continue advancing to an empty square. The piece loses any special condition it had and returns to being normal. If the goal is surpassed with this object, the piece will not bounce back; it will stay at the goal.
32. Both the bullet and the mushroom can pass through barriers and megabarriers (the latter are described later).

33. The lightning is a special die that harms almost all pieces, except those of the selected color. When used, all pieces, except invincible, ghost, and megapieces, become mini-pieces for 5 turns, acquiring the weaknesses indicated in the following rules. Megapieces return to normal after using lightning. Additionally, using lightning causes the opposing player to lose the last acquired object.
34. A mini-piece will move at half speed (integer division) when a normal die is used. For example, if a mini-piece moves by rolling a one on the die, it will not advance any squares.
35. Barriers formed by mini-pieces are passable by any other piece. Additionally, mini-pieces cannot capture other pieces. They will coexist with the other piece in the destination square, regardless of color.

36. The boo is a special die that grants powers to the chosen piece. When used, the piece enters the ghost state for 5 turns. Additionally, it steals the last die acquired by the opposing player, if they had any, and becomes part of the current player's dice.
37. A ghost piece can coexist with any other piece on safe and unsafe squares. It cannot capture or be captured. It is also unaffected by the actions of invincible and megapieces. Additionally, it can pass through barriers and megabarriers.

38. The star is a special die that grants special powers to the chosen piece. When used, the piece enters the invincible state for 3 turns.

39. An invincible piece cannot be captured. If a piece of another color attempts to overtake or capture an invincible piece, it will be eliminated and sent back home. Additionally, when an invincible piece makes a move, it eliminates all (except for the exceptions in the following rules) pieces of a different color it encounters along its path, including barriers and pieces on safe squares, although no score of 20 is counted for any of the eliminated pieces.
40. Invincible pieces score an additional 2 units for each move made with a normal die.
41. Ghost and megapieces are not affected by the effects of the invincible piece when moving.

42. The megamushroom is a special die that grants powers to the chosen piece. When used, the piece enters the megapiece state for 3 turns.
43. A megapiece occupies 2 squares: the square it originated from when it was normal and the next one. It cannot grow if the current or next square has pieces of the same color, invincible pieces, or other megapieces. Otherwise, upon growing, it eliminates all pieces occupying both squares, except for ghosts, with which it can coexist.
44. A megapiece cannot move to adjacent pairs of squares where there is a piece of the same color, invincible pieces, or other megapieces. Otherwise, when moving, it will eat all the pieces in those two squares, except for ghosts, with which it can coexist. No score of 20 is counted for the pieces eaten in this state.
45. A megapiece alone acts as a megabarrier. It does not allow any other piece of a different color to pass through, not even invincible pieces or other megapieces. Only ghost pieces can pass through megabarriers.
46. In the turn the megapiece returns to being small, it stays in the farthest square of the two it occupied. The megapiece reaches the goal when one of the two squares it occupies is the goal. At that moment, it automatically becomes small.

47. The turns that the special states of the pieces last are counted during the turn of the player to whom the pieces belong. For example, a yellow invincible piece will last in this state for the next 3 moves of the yellow-red player. If, in one of those turns, a 6 is rolled or a +20 or +10 movement is made by eating or reaching the goal, that extra move also decreases the invincibility counter. The same applies to any other special state.

## Practice Objectives
The practice aims to design and implement a deliberative agent capable of exhibiting intelligent behavior within the PARCHÍS game explained above.
To have a deliberative agent, the implementation of the MINIMAX or ALPHA-BETA PRUNING algorithm will be necessary to provide intelligent deliberative behavior to an artificial player for this game, allowing it to compete and win against its adversaries.
