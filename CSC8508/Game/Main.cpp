
#include "../../Common/Window.h"

#include "Game.h"

using namespace NCL;
using namespace CSC8508;

/*
The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead.
This time, we've added some extra functionality to the window class - we can
hide or show the
*/

int main() {
	Window* w = Window::CreateGameWindow("Fall Bros.", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	srand((unsigned int)time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
	Game* game = new Game();
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {

		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (!game->UpdateGame(dt))
			break;

		w->SetTitle("Fall Bros. | frame time:" + std::to_string(1000.0f * dt));
	}

	Window::DestroyGameWindow();
}