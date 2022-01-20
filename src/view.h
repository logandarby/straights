#ifndef _H_VIEW
#define _H_VIEW

/*
"View" portion of the MVC architecture.
Handles the user interface.
*/

#include <vector>
#include <deque>
#include <iostream>
#include <string>

class Card;

class View {
public:
  virtual void displayBoard(const std::deque<Card*>& clubs, const std::deque<Card*>& diamonds,
                            const std::deque<Card*>& hearts, const std::deque<Card*>& spades) = 0;
  virtual void displayHand(const std::vector<Card*>& hand) = 0;
  virtual void displayLegalPlays(const std::vector<Card*>& legalPlays) = 0;
  virtual void displayMessage(std::string msg) = 0;
  virtual void displayError(std::string err) = 0;
  virtual std::string promptCardSelection(std::string msg = "") = 0;
  virtual std::string promptCommand() = 0;
  virtual void displayScore(std::string playerName, const std::vector<Card*>& discards, int oldScore, int newScore) = 0;
  virtual void displayWin(std::string playerName) = 0;
};

class TextView: public View{
  std::ostream& out = std::cout;
  std::istream& in = std::cin;
  void printCardList(const std::vector<Card*>& cards);
  void printCardValues(const std::deque<Card*>& cards);
public:
  void displayBoard(const std::deque<Card*>& clubs, const std::deque<Card*>& diamonds,
                            const std::deque<Card*>& hearts, const std::deque<Card*>& spades) override;
  void displayHand(const std::vector<Card*>& hand) override;
  void displayLegalPlays(const std::vector<Card*>& legalPlays) override;
  void displayMessage(std::string msg) override;
  void displayError(std::string err) override;
  std::string promptCardSelection(std::string msg) override;
  std::string promptCommand() override;
  void displayScore(std::string playerName, const std::vector<Card*>& discards, int oldScore, int newScore) override;
  void displayWin(std::string playerName) override;
};

// Escape Codes to make output prettier
const std::string RESET = "\u001b[0m";
const std::string BOLD = "\u001b[1m";
const std::string UNDERLINE = "\u001b[4m";
const std::string BLACK = "\u001b[30m";
const std::string RED = "\u001b[31m";
const std::string GREEN = "\u001b[32m";
const std::string YELLOW = "\u001b[33m";
const std::string BLUE = "\u001b[34m";
const std::string MAGENTA = "\u001b[35m";
const std::string CYAN = "\u001b[36m";
const std::string WHITE = "\u001b[37m";


#endif
