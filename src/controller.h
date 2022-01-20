#ifndef _H_CONTROLLER
#define _H_CONTROLLER

/*
 The controller portion of the MVC architecture. Handles interactions
 Between a View and the game Model.

 The Controller also acts as a Visitor, which performs handlePlayer on
 all Player elements within the model when StraightsModel::loopThroughPlayers
 is called.
*/

class StraightsModel;
class View;
class HumanPlayer;
class ComputerPlayer;
class Player;

class PlayerHandler {
  bool loopFlag = true;
protected:
  // Used in handlePlayer to stop StraightModel::loopThroughPlayers
  // from looping
  void setLoopFlag(bool b);
public:
  // In StraightsModel::loopThroughPlayers, this is used to check if
  // It should keep looping through players.
  bool shouldLoop() const;
  virtual void handlePlayer(HumanPlayer& p) = 0;
  virtual void handlePlayer(ComputerPlayer& p) = 0;
  virtual ~PlayerHandler();
};

class StraightsController: public PlayerHandler {
  View& view;
  StraightsModel& model;
  // Next four functions handle different commands the user can input
  // They return whether they were successful or not.
  bool play(HumanPlayer& p);
  bool discard(HumanPlayer& p);
  bool printDeck();
  bool quit();
  bool ragequit(HumanPlayer& p);
  void initializePlayers(void);
  bool quitFlag = false;
public:
  StraightsController(View& view, StraightsModel& model);
  void startGameLoop(void);
  void handlePlayer(HumanPlayer& p) override;
  void handlePlayer(ComputerPlayer& p) override;
};

// Strategies are used to control a ComputerPlayer's turn logic
class TurnStrategy {
protected:
  View& view;
  StraightsModel& model;
public:
  TurnStrategy(View& view, StraightsModel &model);
  virtual void doTurn(ComputerPlayer &p) = 0;
  virtual ~TurnStrategy() = default;
};

class SimpleStrategy: public TurnStrategy {
public:
  SimpleStrategy(View& view, StraightsModel &model);
  void doTurn(ComputerPlayer &p) override;
};
#endif
