#ifndef _H_MODEL
#define _H_MODEL

/*
The "model" portion of the MVC architecture. Handles
game state information.
*/

#include <vector>
#include <memory>
#include <map>
#include <deque>
#include <functional>

#include "deck.h"
#include "player.h"

class Player;
class StraightsController;
class PlayerHandler;

class StraightsModel {
  std::vector<std::unique_ptr<Player>> players;
  std::deque<Card*> clubsPile;
  std::deque<Card*> diamondsPile;
  std::deque<Card*> heartsPile;
  std::deque<Card*> spadesPile;
  const std::map<Suit, std::deque<Card*>&> pileMap = {
    {CLUBS, clubsPile}, {DIAMONDS, diamondsPile},
    {HEARTS, heartsPile}, {SPADES, spadesPile}
  };
  std::deque<Card*> &getPile(const Card &card) const;
  Deck deck;
public:
  // Game ends when a player reaches this score;
  static const int MAX_SCORE;
  // Seed will seed the Deck's RNG. If the seed isn't given,
  // or if the seed is DEFAULT_SEED, it's set to the current time by default.
  StraightsModel(unsigned seed = Deck::DEFAULT_SEED);
  void dealHands();
  // Loops through all players constantly, and applies the visitor to each
  // The loop breaks when the Loop Flag on v is set to false.
  void loopThroughPlayers(Player* start, PlayerHandler& v);
  // A for_each implementation to apply func to all players in the model;
  // Takes in a function with the signature void f(Player &)
  void forEachPlayer(std::function<void(Player &)> func);
  // Adds a player to the game.
  void addPlayer(std::unique_ptr<Player> p);
  // Replaces player p1 with a new player p2
  Player *replacePlayer(Player &p1, std::unique_ptr<Player> p2);
  // Searches all Player's hands (not discard piles)
  // Returns nullptr if player cannot be found
  Player* whoHasCard(const Card& card) const;
  // Returns nullptr if no card is found in the deck with the given
  // string representation
  Card *getCard(std::string rep) const;
  void playCard(Player &p, Card& card);
  bool isLegalPlay(const Card& card) const;
  const std::vector<Card*> getLegalPlays(const Player &p) const;
  void shuffleDeck();
  // Checks if players' hands are empty
  bool isEndOfRound() const;
  // Checks if any players score are above MAX_SCORE
  bool isEndOfGame() const;
  // Returns players with lowest score
  const std::vector<Player *> getWinners() const;
  // Clears each players' hands, discards, and roundscore (But keeps
  // their total score intact). Also resets deck, and clears the board
  void resetRound();
  // Purey for testing purposes. Prints the deck to stdout
  void printDeck();
  const std::deque<Card*> &getClubsPile() const;
  const std::deque<Card*> &getHeartsPile() const;
  const std::deque<Card*> &getDiamondsPile() const;
  const std::deque<Card*> &getSpadesPile() const;
};

// Exceptions
struct InvalidPlay: public std::exception {
  const char* what() {
    return "Play is not legal";
  }
};

#endif
