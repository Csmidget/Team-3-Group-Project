#include "NetworkManager.h"
#include "../../Common/Window.h"

NetworkManager::NetworkManager()
{
	if (OFFLINE_MODE) return;
	
	NetworkBase::Initialise();

	isClient ? TestClient() : TestServer();



}

NetworkManager::~NetworkManager()
{
	NetworkBase::Destroy();
}

void NetworkManager::Update(float dt)
{
	if (OFFLINE_MODE) return;


}

void NetworkManager::TestClient()
{
	string clientName = "Client1";
	
	TestPacketReceiver clientReceiver(clientName);
	GameClient* client = new GameClient();
	client->RegisterPacketHandler(String_Message, &clientReceiver);
	bool canConnect = client->Connect(80, 5, 123, 22, NetworkBase::GetDefaultPort());

	int i = 0;
	while (!Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		StringPacket strPacket(clientName + " says hello! " + to_string(i));
		client->SendPacket(strPacket);
		i++;
		client->UpdateClient();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void NetworkManager::TestServer()
{
	TestPacketReceiver serverReceiver("Server");
	GameServer* server = new GameServer(NetworkBase::GetDefaultPort(), 2);
	server->RegisterPacketHandler(String_Message, &serverReceiver);

	while (!Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		StringPacket strPacket("Server says hello! ");
		server->SendGlobalPacket(strPacket);
		server->UpdateServer();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
}
