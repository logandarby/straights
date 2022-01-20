# Project Specification - Straights

## Game

- Lowest score wins. Game stops when a player reaches 80 points
  - Game score is score of all rounds combined
  - Wait till end of round to count score (don't stop in middle when 80 is reached)
- Deck is standard 52, no jokers. Ace is 1, King is 13
- 4 Players, each get 13 cards
- 7S goes first in the center of table
- Legal plays:
  - 7 of any suit starts a new pile
  - a card with same suit and adjacent rank (one above or below) on a pile
  - You can add cards to any side of the suit pile
  - NOTE: this means that all piles will be in numerical order
- If you have no legal plays, you must discard a card
  - If you have a legal play, you CANNOT discard
- Round ends when all card are played or discarded
  - Score is rank of all discarded cards

## Computer Details

- Command line arg: Seed for shuffling deck (pseudo-random shuffling to test)
  - `shuffle.cc` is an example used to shuffle `std::shuffle`

- Starts by inviting players 1 - 4

```
Is Player <x> a human (h) or a computer (c)?
>
```

- Cards are represented like AC, KS, 5D, etc.
- Deck must initially be in this order
  - AC - KC, AD - KD, AH - KH, AS - KS
- At the beginning of each round, shuffle the deck once
  - Beginning of next round, deck is restored to the state pre-game and shuffled again (to preserve pseudo-randomness)

- Game order is cyclical: P1 -> P2 -> P3 -> P4 -> P1 -> ... etc.
  - But remember: starts with P with 7S
- Starts off with printing:
  - `A new round begins. It's Player<x>'s turn to play` 

For human players, each turn we print:

```
Cards on the table:
Clubs:<list of clubs>
Diamonds:<lsit of diamonds>
Hearts:<list of hearts>
Spades:<list of spades>
Your hand:<cards in your hand>
Legal plays:<list of legal plays>
```

- each list of cards is ordered numerically
- every other list is the order in which they appear
- Lists are seperated, starts with space (except for `deck` command), seperated by spaces, no final space after last card.
- If no cards, then empty string

#### Commands

`play <card>`
  - Plays the card
  - Assume `<card>` has valid syntax and is in hand
  - If play is legal, print: `Player<x> plays <card>`
  - Otherwise, `This is not a legal play` and do not proceed to next player
- `discard <card>`
  - Only valid if no legal plays
  - Add card from hand into discard pile
  - Assume valid syntax and card is in hand
  - Print `Player<x> discards <card>`
  - Otherwise `You have a legal play. You may not discard.`
- `deck` 
  - Used only for testing purposes
  - Prints the contents of the deck in order, 13 card per line. 
- `quit`
  - Terminate the program
- `ragequit`
  - The player leaves and is replaced by a computer

While you're not required to have robust handling of commands, you probably should handle invalid commands/card values

#### Computer Players

- We only print if they play or discard a card.
- Strategy will be very simple. They make the first legal move in their hand. If they have no plays, they discard the first card in their hand.

#### Scoring

For each player, print

```
Player<x>'s discards:<list of discards>
Player<x>'s score: <old score> + <score gained> = <new score>
```

Print in the order they were discarded. The player (or players, if theres a tie) who win are printed:

```
Player<x> wins!
```

## Questions

- What sort of class design or design pattern should you use to structure your game classes to that changing the user interface from text-based to graphical, or changing the game rules, would have as little impact on the code as possible? Explain how your classes fir this framework
- Consider thet different types of computer players might also have different play  strategies, and that strategies might change as the game progresses i.e., dynamically during the play of the game. How would that affect your class structures?
- How would your design change, if at all, if the two Jokers in a deck were added to the game as *wildcards* i.e., the player in possesion of a Joker would choose it to take the place of any card in the game except the 7S?
