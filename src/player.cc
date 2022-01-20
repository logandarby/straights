#include <algorithm>

#include "player.h"
#include "deck.h"
#include "controller.h"
#include "debug.h"

Player::Player(const std::string name) : name{name}
{}

bool Player::hasCard(const Card& card) const {
  return std::find(hand.begin(), hand.end(), &card) != hand.end();
}

void Player::removeCard(const Card& card) {
  auto i = find(hand.begin(), hand.end(), &card);
  if (i == hand.end()) {
    throw CardNotInHand{};
  }
  hand.erase(i);
}

void Player::discardCard(Card& card) {
  removeCard(card);
  discards.push_back(&card);
  roundScore += card.getScore();
  totalScore += card.getScore();
}

void Player::giveCard(Card& card) {
  hand.push_back(&card);
}

int Player::getRoundScore() const {
  return roundScore;
}

int Player::getTotalScore() const {
  return totalScore;
}


std::string Player::getName() const {
  return name;
}


const std::vector<Card*> &Player::getHand() const {
  return hand;
}

const std::vector<Card*> &Player::getDiscards() const {
  return discards;
}

void Player::reset() {
  hand.clear();
  discards.clear();
  roundScore = 0;
}

HumanPlayer::HumanPlayer(const std::string name) : Player{name}
{}

void HumanPlayer::accept(PlayerHandler &v) {
  v.handlePlayer(*this);
}

ComputerPlayer::ComputerPlayer(const std::string name,
                               std::unique_ptr<TurnStrategy> strat) : 
  Player{name}, turnStrat{std::move(strat)}
{}

void ComputerPlayer::accept(PlayerHandler &v) {
  v.handlePlayer(*this);
}

void ComputerPlayer::doTurn() {
  turnStrat->doTurn(*this);
}
