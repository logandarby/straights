#include <algorithm>

#include "model.h"
#include "controller.h"
#include "deck.h"
#include "debug.h"

const int INIT_CARDS_IN_HAND = 13;
const int StraightsModel::MAX_SCORE = 80;

StraightsModel::StraightsModel(const unsigned seed) : deck{seed}
{}

void StraightsModel::dealHands() {
  for (auto& p : players) {
    for (int i = 0; i < INIT_CARDS_IN_HAND; ++i) {
      deck.dealCard(*p);
    }
  }
  return;
}

void StraightsModel::addPlayer(std::unique_ptr<Player> p) {
  players.push_back(std::move(p));
}

Player* StraightsModel::replacePlayer(Player &p1, std::unique_ptr<Player> p2) {;
  unsigned i = 0;
  bool found = false;
  for (; i < players.size(); i++) {
    if (players[i].get() == &p1) {
      players[i].swap(p2);
      // Swap their hands as well
      for (Card* card : p1.getHand()) {
        players[i]->giveCard(*card);
      }
      // Also swap their discards
      for (Card* card : p1.getDiscards()) {
        players[i]->giveCard(*card);
        players[i]->discardCard(*card);
      }
      found = true;
      break;
    }
  }
  if (found) return players[i].get();
  return nullptr;
}

void StraightsModel::loopThroughPlayers(Player* start, PlayerHandler& v) {;
  // Find start
  auto p = std::find_if(players.begin(), players.end(),
    [start](std::unique_ptr<Player>& pp) {
      return pp.get() == start;
    });
  // Loop untill PlayerHandler's LoopFlag is set to false;
  while (true) {
    (*p)->accept(v);
    if (!v.shouldLoop()) return;
    ++p;
    if (p == players.end()) {
      p = players.begin();
    }
  }

  // // Find Start
  // unsigned startIndex = 0;
  // const unsigned endIndex = players.size();
  // for (unsigned i = 0; i < endIndex; i++) {
  //   if (players[i].get() == start) {
  //     startIndex = i;
  //     break;
  //   }
  // }
  // // Loop untill Playerhandler's LoopFlag is set to false
  // for (unsigned i = startIndex; i < endIndex; ) {
  //   players[i]->accept(v);
  //   if (!v.shouldLoop()) return;
  //   i++;
  //   if (i == endIndex) {
  //     i = 0;
  //   }
  // }
}

void StraightsModel::forEachPlayer(std::function<void(Player &)> func) {
  for (auto& p : players) {
    func(*p);
  }
}

Player* StraightsModel::whoHasCard(const Card& card) const {
  auto it = players.begin();
  for (; it != players.end(); ++it) {
    if ((*it)->hasCard(card)) break;
  }
  if (it == players.end()) return nullptr;
  return it->get();
}

void StraightsModel::playCard(Player &p, Card &card) {
  p.removeCard(card);
  auto& pile = getPile(card);
  if (pile.empty()) {
    Debug::print("Pile is empty.");
    if (!card.hasRank(SEVEN)) throw InvalidPlay{};
    pile.push_back(&card);
  } else if (card.isFrontAdjacentTo(*pile.front())) {
    Debug::print("Is front adjacent");
    pile.push_front(&card);
  } else if (card.isBackAdjacentTo(*pile.back())) {
    Debug::print("Is back adjacent");
    pile.push_back(&card);
  } else {
    throw InvalidPlay{};
  }
}

bool StraightsModel::isLegalPlay(const Card& card) const {
  auto& pile = getPile(card);
  if (!pile.empty()) {
    return card.isAdjacentTo(*pile.front()) ||
          card.isAdjacentTo(*pile.back());
  }
  return card.hasRank(SEVEN);
}

const std::vector<Card*> StraightsModel::getLegalPlays(const Player &p) const {
  std::vector<Card*> legalPlays;
  const std::vector<Card*> &hand = p.getHand();
  for (auto& card : hand) {
    if(isLegalPlay(*card)) {
      legalPlays.push_back(card);
    }
  }
  return legalPlays;
}


Card *StraightsModel::getCard(const std::string rep) const {
  return deck.getCard(rep);
}

bool StraightsModel::isEndOfRound() const {
  for (auto& p : players) {
    if (!p->getHand().empty()) return false;
  }
  return true;
}

bool StraightsModel::isEndOfGame() const {
  for (auto& p : players) {
    if (p->getTotalScore() >= MAX_SCORE) {
      return true;
    }
  }
  return false;
}

const std::vector<Player *> StraightsModel::getWinners() const {
  int lowestScore = MAX_SCORE;
  // Determine lowest score
  for (auto& p : players) {
    if (p->getTotalScore() < lowestScore) {
      lowestScore = p->getTotalScore();
    }
  }
  // Determine any players who've reached the lowest score (to include ties)
  std::vector<Player *> winners;
  for (auto& p : players) {
    if (p->getTotalScore() == lowestScore) {
      winners.push_back(p.get());
    }
  }
  return winners;
}

void StraightsModel::resetRound() {
  for (auto& p : players) {
    p->reset();
  }
  deck.reset();
  clubsPile.clear();
  diamondsPile.clear();
  heartsPile.clear();
  spadesPile.clear();
}


void StraightsModel::shuffleDeck() {
  deck.shuffle();
}

void StraightsModel::printDeck() {
  deck.printDeck();
}

const std::deque<Card*> &StraightsModel::getClubsPile() const {
  return clubsPile;
}

const std::deque<Card*> &StraightsModel::getHeartsPile() const {
  return heartsPile;
}

const std::deque<Card*> &StraightsModel::getDiamondsPile() const {
  return diamondsPile;
}

const std::deque<Card*> &StraightsModel::getSpadesPile() const {
  return spadesPile;
}

std::deque<Card*> &StraightsModel::getPile(const Card &card) const {
  return pileMap.at(card.getSuit());
}
