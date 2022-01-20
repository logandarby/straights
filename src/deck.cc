#include <map>
#include <algorithm>
#include <chrono>

#include "deck.h"
#include "player.h"
#include "debug.h"

// Amount of times that std::suffle shuffles the deck
const int SHUFFLE_AMOUNT = 100;
unsigned Deck::DEFAULT_SEED = 0;

const std::map<Suit, std::string> suitMap = {
  {CLUBS, "C"}, {DIAMONDS, "D"}, {HEARTS, "H"}, {SPADES, "S"}
};
const std::map<Rank, std::string> rankMap = {
  {ACE, "A"}, {TWO, "2"}, {THREE, "3"}, {FOUR, "4"}, {FIVE, "5"},
  {SIX, "6"}, {SEVEN, "7"}, {EIGHT, "8"}, {NINE, "9"}, {TEN, "10"},
  {JACK, "J"}, {QUEEN, "Q"}, {KING, "K"}
};

Card::Card(const Rank rank, const Suit suit) :
  rank{rank}, suit{suit}
{
  if (!willBeValidCard(rank, suit)) {
    throw InvalidCard{};
  } 
  rep = rankMap.at(rank) + suitMap.at(suit);
}

Suit Card::getSuit() const {
  return suit;
}

bool Card::hasRank(Rank r) const {
  return rank == r;
}

bool Card::willBeValidCard(const Rank rank, const Suit suit) {
  try {
    rankMap.at(rank);
    suitMap.at(suit);
    return true;
  } catch (std::out_of_range const&) {
    return false;
  }
}

std::string Card::getStringRep() const {
  return rep;
}

int Card::getScore() const {
  return rank;
}

bool Card::isAdjacentTo(Card &c) const {
  return (c.suit == suit) &&
         (rank == c.rank + 1 || rank == c.rank - 1);
}

bool Card::isFrontAdjacentTo(Card &c) const {
  return (c.suit == suit) && (rank == c.rank - 1);
}

bool Card::isBackAdjacentTo(Card &c) const {
  return (c.suit == suit) && (rank == c.rank + 1);
}


std::ostream& operator<<(std::ostream& os, Card &card) {
  os << card.getStringRep();
  return os;
}

Deck::Deck(const unsigned seed) {
  unsigned newSeed = seed;
  if (seed == 0) {
    newSeed = std::chrono::system_clock::now().time_since_epoch().count();
  }
  Debug::print("Deck seed: " + std::to_string(newSeed));
  rng = std::default_random_engine{newSeed};
  initializeStandardOrder();
}

void Deck::shuffle() {
  for (int i = 0; i < SHUFFLE_AMOUNT; i++) {
    std::shuffle(cards.begin(), cards.end(), rng);
  }
}

void Deck::dealCard(Player& p) {
  try {
    Card* cardPtr = cards.at(dealtCardIndex).get();
    p.giveCard(*cardPtr);
    dealtCardIndex++;
  } catch (std::out_of_range& e) {
    throw DeckIsEmpty{};
  }
}

void Deck::reset() {
  dealtCardIndex = 0;
}

Card *Deck::getCard(const std::string rep) const {
  try {
    auto cardPtr = cardMap.at(rep);
    return cardPtr;
  } catch (std::out_of_range& e) {
    return nullptr;
  }
}

void Deck::initializeStandardOrder() {
  for (int suit = CLUBS; suit <= SPADES; suit++) {
    for (int rank = ACE; rank <= KING; rank++) {
      std::unique_ptr<Card> cardPtr = std::make_unique<Card>(
          static_cast<Rank>(rank), static_cast<Suit>(suit));
      cardMap[cardPtr->getStringRep()] = cardPtr.get();
      cards.push_back(std::move(cardPtr));
    }
  }
}

void Deck::printDeck(void) const {
  int i = 1;
  for (auto& card : cards) {
    std::cout << card->getStringRep() << " ";
    if (i % 13 == 0) std::cout << std::endl;
    i++;
  }
}

