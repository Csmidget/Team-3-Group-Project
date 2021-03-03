
#include "../../Common/Window.h"

#include "NetworkedGame.h"

#include "../Engine/GameServer.h"
#include "../Engine/GameClient.h"

using namespace NCL;
using namespace CSC8508;

#include <chrono>
#include <thread>


class TestPacketReceiver : public PacketReceiver {
public:
	TestPacketReceiver(string name) {
		this->name = name;
	}
	void ReceivePacket(int type, GamePacket* payload, int source) {
		if (type == String_Message) {
			StringPacket* realPacket = (StringPacket*)payload;

			string msg = realPacket->GetStringFromData();

			std::cout << name << " received message: " << msg << std::endl;
		}
	}
	string name;
};
void TestNetworkingClient() {
	NetworkBase::Initialise();
	TestPacketReceiver clientReceiver("Client1");
	int port = NetworkBase::GetDefaultPort();
	GameClient* client = new GameClient();
	client->RegisterPacketHandler(String_Message, &clientReceiver);
	bool canConnect = LocalHost::IsLocalHostMode() ? client->Connect(LocalHost::GetA(), LocalHost::GetB(), LocalHost::GetC(), LocalHost::GetD(), LocalHost::GetPort()) :
		client->Connect(0, 0, 0, 0, NetworkBase::GetDefaultPort());

	int i = 0;
	while (!Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		client->SendPacket(StringPacket("Client1 says hello! " + std::to_string(i)));
		i++;
		client->UpdateClient();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	NetworkBase::Destroy();

}

void TestNetworkingServer() {
	NetworkBase::Initialise();
	TestPacketReceiver serverReceiver("Server");
	//int port = NetworkBase::GetDefaultPort();
	GameServer* server = new GameServer(LocalHost::GetPort(), 2);
	server->RegisterPacketHandler(String_Message, &serverReceiver);

	while (!Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		server->SendGlobalPacket(StringPacket("Server says hello! "));
		server->UpdateServer();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	NetworkBase::Destroy();

}

void TestNetworking() {
	NetworkBase::Initialise();

	TestPacketReceiver serverReceiver("Server");
	TestPacketReceiver clientReceiver("Client1");

	TestPacketReceiver clientReceiver2("Client2");

	int port = NetworkBase::GetDefaultPort();

	GameServer* server = new GameServer(port, 2);
	GameClient* client = new GameClient();
	GameClient* client2 = new GameClient();

	server->RegisterPacketHandler(String_Message, &serverReceiver);
	client->RegisterPacketHandler(String_Message, &clientReceiver);
	client2->RegisterPacketHandler(String_Message, &clientReceiver2);

	bool canConnect = client->Connect(127, 0, 0, 1, port);
	bool canConnect2 = client2->Connect(127, 0, 0, 1, port);

	for (int i = 0; i < 100; ++i) {
		server->SendGlobalPacket(StringPacket("Server says hello! " + std::to_string(i)));
		client->SendPacket(StringPacket("Client1 says hello! " + std::to_string(i)));
		client2->SendPacket(StringPacket("Client2 says hello! " + std::to_string(i)));
		server->UpdateServer();
		client->UpdateClient();
		client2->UpdateClient();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	NetworkBase::Destroy();
}





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
int main() {
	NetworkBase::Initialise();
	Window* w = Window::CreateGameWindow("CSC8508 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}
	//TestNetworking();
	//TestNetworkingClient();
	//TestNetworkingServer();


	srand((unsigned int)time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	NetworkedGame* g = new NetworkedGame();
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

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g->UpdateGame(dt);
	}
	NetworkBase::Destroy();

	Window::DestroyGameWindow();
}