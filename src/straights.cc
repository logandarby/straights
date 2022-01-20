#include <iostream>
#include <string>
#include <map>

#include "view.h"
#include "model.h"
#include "deck.h"
#include "controller.h"
#include "debug.h"

using namespace std;

int main(int argc, char* argv[]) {
  Debug::print("Debug enabled");
  unsigned seed = Deck::DEFAULT_SEED;
  if (argc == 2) {
    Debug::print("Setting seed");
    seed = std::stoi(std::string{argv[1]});
  }
  // If the seed is DEFAULT_SEED, it uses a default seed
  StraightsModel model{seed};
  TextView view;
  StraightsController controller{view, model};
  controller.startGameLoop();
}
