
#include "../../Common/Window.h"

#include "../Engine/StateMachine.h"
#include "../Engine/StateTransition.h"
#include "../Engine/State.h"

#include "../Engine/PushdownState.h"
#include "../Engine/PushdownMachine.h"

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
// blarg
//wort wort wort
*/
int gameScore = 1000;

class OverScreen : public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState)override {
		g2->UpdateGame(dt);
		if (gameScore <= 0) {
			Debug::Print("Play again", Vector2(30, 10));
			Debug::Print("You lost", Vector2(30, 50));
		}
		else {
			Debug::Print("Play again", Vector2(30, 10));
			Debug::Print("You win", Vector2(30, 30));
			Debug::Print("Play again", Vector2(30, 10));
			Debug::Print("Your score:" + std::to_string(gameScore), Vector2(30, 50));
		}
		return PushdownResult::Top;
	}
	void OnAwake()override {
		std::cout << " 加载结束界面 !\n";
	}
protected:
	Game* g2 = new Game("2");

};

class PauseScreen : public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Debug::Print("Press K to return", Vector2(0, 5));
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::K)) {
			return PushdownResult::Pop;
		}
		g2->UpdateGame(dt);
		return PushdownResult::NoChange;
	}
	void OnAwake() override {
	};
protected:
	Game* g2 = new Game("2");
};

class GameScreen : public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {

		Debug::Print("Press P to pause", Vector2(1, 5));
		Debug::Print("Press F1 to return to main menu", Vector2(1, 10));

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
			*newState = new PauseScreen();
			return PushdownResult::Push;

		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
			return PushdownResult::Pop;

		}
		// function for game over （判断游戏是否结束）
/*		if (g1->GetFinal() || g1->GetScore() <= 0) {
			gameScore = g1->GetScore();
			*newState = new OverScreen();
			return PushdownResult::Over;
		}*/
		else {
			g1->UpdateGame(dt);
		}
		return PushdownResult::NoChange;

	};
	void OnAwake() override {
		std::cout << " Preparing to mine coins !\n";
	}
protected:
	Game* g1 = new Game("1");
};

class IntroScreen : public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Debug::Print("Click to Start", Vector2(42, 50));
		Debug::Print("Click to Exit", Vector2(43, 70));
		switch (g0->GetOpenOrExit()) {
		case 0:
			g0->UpdateIntroGame(dt);
			return PushdownResult::NoChange;
		case 1:
			*newState = new GameScreen();
			return PushdownResult::Push;
			break;
		case 2:
			Window::DestroyGameWindow();
			return PushdownResult::Pop;
			break;
			/*		case 5:
						addAi = true;
						*newState = new GameScreen();
						return PushdownResult::Push;
						break;*/
		}
	};
	void OnAwake() override {
		g0 = new Game("0");
	}
protected:
	Game* g0;
};

int main() {
	Window* w = Window::CreateGameWindow("CSC8508 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}


	srand((unsigned int)time(0));
	PushdownMachine machine(new IntroScreen());
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

//	Game* g = new Game();
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

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}
		machine.Update(dt);
		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));
	}

//	Window::DestroyGameWindow();
}