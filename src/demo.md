# Demo

The Straights game is implemented almost exactly to the specification the project PDF gave, with a few minor bugs.

## Bugs of Note

- The `deck` command skips the players' turn
- The value "10" doesn't show up correctly in a pile (it shows up as "1," while an ace shows up as "A")
- Erroneous command line arguments aren't checked
- Input buffer isn't cleared when an erroneous command is entered

## Starting the Game

Make the file, and the `straights` file can be run. It takes an optional command line argument, being the desired seed to shuffle the deck.

Start the game with the seed `12`

The game will prompt you to input whether each of the 4 players are human or computer. Make the first two human players (`h`), and the last two computer players (`c`).

## Playing the Game

You will take your turn first, since you have the Seven of Clubs in your hand. To play it, type `play 7S`.

The two Computer Players will then take their turns, playing `2D` and `6S`.

You can also discard cards if you have no legal plays, but not if you have any legal plays in hand. Try typing `discard 8S`, then type `play 8S`.

Next turn, you will have no plays. You can try the `play` command, but it won't work. Instead, discard a card in your hand.

Next turn, lets say you're fed up, and ragequit the game. Type in the command `ragequit`, and `Player1` will be replaced with an AI player named `Player1's Ghost`.

Now you could play out a round, perhaps experimenting with erroneous commands more, or you could quit out with the `quit` command. However, to make things quick we can fast forward to game to the end. Type `ragequit` again. The game will now be nothing but computer players who will play the rest of the game. Scrolling back up through the game history, 4 rounds will have been played, and the game stopped when `Player2's Ghost` accumulated more than 80 points.
