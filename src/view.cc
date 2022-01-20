#include "view.h"
#include "deck.h"

void TextView::displayBoard(const std::deque<Card*> &clubs,
  const std::deque<Card*> &diamonds, const std::deque<Card*> &hearts,
  const std::deque<Card*> &spades) 
{
  out << BOLD << "Cards on the table:" << RESET << std::endl;
  out << "Clubs: ";
  printCardValues(clubs);
  out << std::endl;
  out << "Diamonds: ";
  printCardValues(diamonds);
  out << std::endl;
  out << "Hearts: ";
  printCardValues(hearts);
  out << std::endl;
  out << "Spades: ";
  printCardValues(spades);
  out << std::endl;
}

void TextView::displayHand(const std::vector<Card*>& hand) {
 out << BOLD << "Your hand: " << RESET;
 printCardList(hand);
 out << std::endl;
}

void TextView::displayLegalPlays(const std::vector<Card*>& legalPlays) {
  out << "Legal plays: ";
  printCardList(legalPlays);
  out << std::endl;
}

void TextView::displayMessage(const std::string msg) {
  out << msg << std::endl;
}

void TextView::displayError(const std::string err) {
  out << RED << BOLD << "Error: " << RESET;
  out << err << std::endl;
}

std::string TextView::promptCardSelection(const std::string msg = "") {
  displayMessage(msg);
  std::string card;
  in >> card;
  return card;
}

std::string TextView::promptCommand() {
  out << "> ";
  std::string command;
  in >> command;
  return command;
}

void TextView::displayScore(std::string playerName, const std::vector<Card*>& discards,
                            const int oldScore, const int newScore)
{                          
  out << YELLOW << playerName << RESET << "'s discards: ";
  printCardList(discards);
  out << std::endl;
  out << YELLOW << playerName << RESET  << "'s score: " << oldScore
      << " + " << newScore << " = " << newScore + oldScore << std::endl;
}

void TextView::displayWin(const std::string playerName) {
  out << playerName << " wins!" << std::endl;
}

void TextView::printCardList(const std::vector<Card*>& cards) {
  auto card = cards.begin();
  if (card == cards.end()) return;
  for (; card < cards.end() - 1; card++) {
    out << **card << " ";
  }
  out << **card;
}


void TextView::printCardValues(const std::deque<Card*>& cards) {
  auto card = cards.begin();
  if (card == cards.end()) return;
  for (; card < cards.end() - 1; card++) {
    out << (*card)->getStringRep()[0] << " ";
  }
  out << (*card)->getStringRep()[0];
}

