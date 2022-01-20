#include <map>

#include "controller.h"
#include "model.h"
#include "player.h"
#include "view.h"
#include "debug.h"

const unsigned NUMBER_OF_PLAYERS = 4;
const std::string DIVIDER = "----------------------------------------";

StraightsController::StraightsController(View& view, StraightsModel& model) :
  view{view}, model{model}
{}

void StraightsController::initializePlayers(void) {
  for (unsigned i = 1; i <= NUMBER_OF_PLAYERS; i++) {
    view.displayMessage("Is Player " + std::to_string(i) +
      " a human (h) or a computer (c)?");
      std::string command;
    while(true) {
      command = view.promptCommand();
      if (command == "h") {;
        model.addPlayer(std::make_unique<HumanPlayer>(
          "Player" + std::to_string(i)));
        break;
      } else if (command == "c") {
        // Create computer player with SimpleStrategy
        auto strategy = std::make_unique<SimpleStrategy>(view, model);
        model.addPlayer(std::make_unique<ComputerPlayer>(
          "Computer" + std::to_string(i),
          std::move(strategy)
        ));
        break;
      } else {
        view.displayError("Invalid Option");
      }
    }
  }
}


void StraightsController::handlePlayer(HumanPlayer& p) {
  if (model.isEndOfRound()) {
    Debug::print("End of round reached");
    setLoopFlag(false);
    return;
  }
  view.displayMessage(DIVIDER);
  view.displayMessage("It's "+YELLOW+p.getName()+RESET+"'s turn to play");
  view.displayBoard(model.getClubsPile(), model.getDiamondsPile(),
    model.getHeartsPile(), model.getSpadesPile());
  const std::vector<Card *>& hand = p.getHand();
  view.displayHand(hand);
  if(hand.empty()) return;
  view.displayLegalPlays(model.getLegalPlays(p));
  while (true) {
    std::string command = view.promptCommand();
    if (command == "play") {
      if (play(p)) break;
    } else if (command == "discard") {
      if (discard(p)) break;
    } else if (command == "deck") {
      if (printDeck()) break;
    } else if (command == "quit") {
      if (quit()) break;
    } else if (command == "ragequit") {
      if (ragequit(p)) break;
    } else {
      view.displayError("Invalid Command"); 
    }
  }
}

bool StraightsController::play(HumanPlayer& p) {
  const std::string cardstr = view.promptCardSelection();
  Card* cardptr = model.getCard(cardstr);
  if (!cardptr) {
        view.displayError("Invalid Command");
        return false;
  }
  if (model.whoHasCard(*cardptr) != &p ||
      !model.isLegalPlay(*cardptr))
  {
    view.displayError("This is not a legal play");
    return false;
  }
  view.displayMessage(YELLOW+p.getName()+RESET+" plays "+cardstr);
  model.playCard(p, *cardptr);
  return true;
}

bool StraightsController::discard(HumanPlayer& p) {
  const std::string cardstr = view.promptCardSelection();
  Card* cardptr = model.getCard(cardstr);
  if (!cardptr && model.whoHasCard(*cardptr) != &p) {
    view.displayError("Invalid Command");
    return false;
  }
  if (!model.getLegalPlays(p).empty()) {
    view.displayError("You have a legal play. You may not discard.");
    return false;
  }
  view.displayMessage(YELLOW+p.getName()+RESET+" discards "+cardstr);
  p.discardCard(*cardptr);
  return true;
}

bool StraightsController::printDeck() {
  model.printDeck();
  return true;
}

bool StraightsController::quit() {
  // The model will stop looping
  Debug::print("Exiting...");
  setLoopFlag(false);
  quitFlag = true;
  return true;
}

bool StraightsController::ragequit(HumanPlayer& p) {
  view.displayMessage(YELLOW + p.getName() + RESET + " ragequits. "
    "A computer will now take over.");
  Player* newPlayer = model.replacePlayer(p, std::make_unique<ComputerPlayer>(
    p.getName() + "'s Ghost", std::make_unique<SimpleStrategy>(view, model)
  ));
  // Do another round
  Debug::print("We doin another round");
  newPlayer->accept(*this);
  Debug::print("We doin another round");
  return true;
}

void StraightsController::handlePlayer(ComputerPlayer& p) {
  if (model.isEndOfRound()) {
    Debug::print("End of round reached");
    setLoopFlag(false);
    return;
  }
  p.doTurn();
}

void SimpleStrategy::doTurn(ComputerPlayer &p) {
  const std::vector<Card*>& hand = p.getHand();
  if (hand.empty()) return;
  view.displayMessage(DIVIDER);
  const std::vector<Card*>& legalPlays = model.getLegalPlays(p);
  if (!legalPlays.empty()) {
    Card* cardToPlay = legalPlays[0];
    model.playCard(p, *cardToPlay);
    view.displayMessage(YELLOW+p.getName()+RESET+" plays "+ cardToPlay->getStringRep());
  } else {
    Card* cardToDiscard = hand[0];
    p.discardCard(*cardToDiscard);
    view.displayMessage(YELLOW+p.getName()+RESET+" discards "+ cardToDiscard->getStringRep());
  }
}

void StraightsController::startGameLoop(void) {
  initializePlayers();
  while (true) {
    // Round starts here
    model.shuffleDeck();
    model.dealHands();
    const Card* sevenOfSpades = model.getCard("7S");
    Player* const start = model.whoHasCard(*sevenOfSpades);
    view.displayMessage(UNDERLINE+"A new round begins."+RESET);
    setLoopFlag(true);
    // This command consists of one round.
    model.loopThroughPlayers(start, *this);
    // Used to quit the game
    if (quitFlag) return;
    // Check scores to see if game must be quit
    // otherwise start another round
    Debug::print("Printing player scores");
    view.displayMessage(DIVIDER);
    view.displayMessage(MAGENTA + BOLD + UNDERLINE + "Scores:" + RESET);
    model.forEachPlayer([this](Player &p) {
      view.displayScore(p.getName(), p.getDiscards(),
      p.getTotalScore() - p.getRoundScore(), p.getRoundScore());
    });
    view.displayMessage(DIVIDER);
    if (model.isEndOfGame()) {
      auto winners = model.getWinners();
      for (auto& p : winners) {
        view.displayWin(p->getName());
      }
      break;
    }
    model.resetRound();
  }
}

PlayerHandler::~PlayerHandler() {}

void PlayerHandler::setLoopFlag(const bool b) {
  loopFlag = b;
}

bool PlayerHandler::shouldLoop() const {
  return loopFlag;
}

TurnStrategy::TurnStrategy(View& view, StraightsModel &model) :
  view{view}, model{model}
{}

SimpleStrategy::SimpleStrategy(View& view, StraightsModel &model) :
  TurnStrategy(view, model)
{}
