
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
void TestNetworkingClient(Window* w) {
	NetworkBase::Initialise();
	TestPacketReceiver clientReceiver("JakeClient");
	int port = NetworkBase::GetDefaultPort();
	GameClient* client = new GameClient();
	client->RegisterPacketHandler(String_Message, &clientReceiver);
	bool canConnect = client->Connect(80,5,123,22, port);


	
	int i = 0;

	bool keyPressed;

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {

		char inputs[5] = "0000";
		keyPressed = false;

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::W))
		{
			inputs[0] = '1';
			keyPressed = true;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::A))
		{
			inputs[1] = '1';
			keyPressed = true;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::S))
		{
			inputs[2] = '1';
			keyPressed = true;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::D))
		{
			inputs[3] = '1';
			keyPressed = true;
		}


		//inputs[4] = '/0';


		if (keyPressed)
		{
					client->SendPacket(StringPacket("JakeClient inputs " + std::string(inputs) + std::to_string(i)));
		i++;
		}

		client->UpdateClient();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	NetworkBase::Destroy();

}

void TestNetworkingServer() {
	NetworkBase::Initialise();
	TestPacketReceiver serverReceiver("Server");
	int port = NetworkBase::GetDefaultPort();
	GameServer* server = new GameServer(port, 2);
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

	//GameServer* server = new GameServer(port, 1);
	GameClient* client = new GameClient();


	//server->RegisterPacketHandler(String_Message, &serverReceiver);
	client->RegisterPacketHandler(String_Message, &clientReceiver);

	bool canConnect = client->Connect(127, 0, 0, 1, port);

	if (canConnect)
	{
		for (int i = 0; i < 100; ++i) {
			client->SendPacket(StringPacket("Jake says hello! " + std::to_string(i)));
			client->UpdateClient();
			//client2->UpdateClient();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
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
	Window* w1 = Window::CreateGameWindow("CSC8508 Server!", 1280, 720);
	//Window* w2 = Window::CreateGameWindow("CSC8508 Client!", 1280, 720);

	if (!w1->HasInitialised()) {
		return -1;
	}
	//TestNetworking();
	//TestNetworkingClient();


	//NetworkBase::Initialise();
	
	srand((unsigned int)time(0));
	w1->ShowOSPointer(false);
	w1->LockMouseToWindow(true);

	NetworkedGame* g = new NetworkedGame();

	w1->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	TestNetworkingClient(w1);
	while (w1->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {

		float dt = w1->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w1->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w1->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w1->SetWindowPosition(0, 0);
		}

		w1->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g->UpdateGame(dt);

	}

	//NetworkBase::Destroy();
	Window::DestroyGameWindow();
}