#ifndef _H_PLAYER
#define _H_PLAYER

/*
Represents a player in the straights game.
Can be either a human player, or an AI ComputerPlayer
*/

#include <string>
#include <vector>
#include <memory>

class Card;
class PlayerHandler;
class StraightsModel;
class TurnStrategy;

class Player {
  const std::string name;
  std::vector<Card*> hand;
  std::vector<Card*> discards;
  int roundScore = 0;
  int totalScore = 0;
public:
  Player(std::string name);
  // Searches Player's hand (not discard pile)
  bool hasCard(const Card& card) const;
  void removeCard(const Card& card);
  void discardCard(Card& card);
  void giveCard(Card& card);
  int calculateScore();
  // Clears hands, discards, and roundScore
  void reset();
  int getRoundScore() const;
  int getTotalScore() const;
  std::string getName() const;
  const std::vector<Card*> &getHand() const;
  const std::vector<Card*> &getDiscards() const;
  // Implementation of Visitor Pattern
  virtual void accept(PlayerHandler &v) = 0;
  virtual ~Player() = default;
};

class HumanPlayer: public Player {
public:
  HumanPlayer(std::string name);
  void accept(PlayerHandler &v) override;
};

class ComputerPlayer: public Player {
  // By default, this is automatically init as a SimpleStrategy.
  std::unique_ptr<TurnStrategy> turnStrat;
public:
  // Requires a move of a unique_ptr<TurnStrategy> to transfer ownership
  ComputerPlayer(std::string name, std::unique_ptr<TurnStrategy> strat);
  void accept(PlayerHandler &v) override;
  void doTurn();
};

#endif
