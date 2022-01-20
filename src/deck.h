#ifndef _DECK_H
#define _DECK_H

/*
Implementation for the deck of cards used.
*/

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <memory>
#include <random>
#include <stdexcept>

enum Suit {CLUBS = 1, DIAMONDS, HEARTS, SPADES};
enum Rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
                  NINE, TEN, JACK, QUEEN, KING};

class Player;

class Card {
  const Rank rank;
  const Suit suit;
  std::string rep;
  bool willBeValidCard(Rank rank, Suit suit);
public:
  // Throws an InvalidCard error if the rank/suit are invalid.
  Card(Rank rank, Suit suit);
  // Guaranteed to return either clubs, hearts, spades, or diamonds
  Suit getSuit() const;
  bool hasRank(Rank r) const;
  std::string getStringRep(void) const;
  int getScore() const;
  bool isAdjacentTo(Card &c) const;
  // Returns if the rank is one lower
  bool isFrontAdjacentTo(Card &c) const;
  // Returns if the rank is one higher
  bool isBackAdjacentTo(Card &c) const;
};

std::ostream& operator<<(std::ostream& os, Card &card);

class Deck {
  std::default_random_engine rng;
  std::vector<std::unique_ptr<Card>> cards;
  std::map<std::string, Card*> cardMap;
  void initializeStandardOrder(void);
  int dealtCardIndex = 0;
public:
  static unsigned DEFAULT_SEED;
  // Seed will seed the Deck's shuffle RNG. If the seed isn't given,
  // or if the seed is DEFAULT_SEED, it's set to the current time by default.
  Deck(unsigned seed = DEFAULT_SEED);
  // Returns a pointer to a Card given by the string representation.
  // Returns nullptr if the card cannot be found.
  Card *getCard(std::string rep) const;
  // Deals a card from the deck to player p.
  // Throws DeckIsEmpty error if the entire deck is already delt.
  void dealCard(Player& p);
  // Returns all delt cards to the deck
  void reset();
  // Shuffles the deck
  void shuffle();
  // Purely for testing purposes, to print the deck 
  void printDeck(void) const;
};

// Exceptions
struct InvalidCard: public std::exception {
  const char* what() {
    return "Arguments do not resolve to a valid card.";
  }
};
struct CardNotInHand: public std::exception {
  const char* what() {
    return "Card is not in the player's hand.";
  }
};

struct DeckIsEmpty: public std::exception {
  const char* what() {
    return "There are no more cards to deal";
  }
};

#endif
