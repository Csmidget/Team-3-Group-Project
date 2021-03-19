#include "LobbyState.h"
#include "Game.h"
#include "../../Common/Window.h"
#include "PlayState.h"
using namespace NCL;
using namespace CSC8508;


LobbyState::LobbyState(Game* game, bool client) {
	this->game = game;
	gameStarted = false;
	game->EnableNetworking(client);
}

LobbyState::~LobbyState() {
	game->DisableNetworking();
}

PushdownState::PushdownResult LobbyState::OnUpdate(float dt, PushdownState** newState) {

	//If this is true, then we have returned after starting the game, which means we just want to return to the menu.
	if (gameStarted)
		return PushdownResult::Pop;

	//Something needs to be checked here to know when to start the game. For now we just wait for K to be pressed.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::K)) {
		gameStarted = true;
		*newState = new PlayState(game);
		game->InitNetworkPlayers();
		return PushdownResult::Push;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void LobbyState::OnAwake() {
	game->InitWorld("CharlesTest.json");
}

