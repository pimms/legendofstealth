#include "GameController.h"
#include "GameScene.h"
#include "net/Packet.h"



/*
================
GameController public methods
================
*/
GameController::GameController()
{
	
}

GameController::~GameController()
{
	if (_tcpSocket)
		delete _tcpSocket;
	if (_udpSocket)
		delete _udpSocket;
}

void GameController::LoadContent()
{
	Controller::LoadContent();
	SetScene(new GameScene);

	_tcpSocket = new Socket(TCP, "localhost", TCP_SERVER_PORT);
	_udpSocket = new Socket(UDP, "localhost", UDP_SERVER_PORT, 0);

	PacketJoinRequest *jreq = new PacketJoinRequest;
	jreq->type = PACKET_JOIN_REQUEST;
	jreq->port = _udpSocket->GetListenPortUDP();
	_tcpSocket->SendPacket(jreq);
	delete jreq;
}

void GameController::Update(DeltaTime &dt)
{
	if (!_tcpSocket->IsConnectionOpen()) {
		Log::Warning("Server disconnected");
		GetApp()->Quit();
		return;
	}

	Controller::Update(dt);

	HandleIncoming();
}



/*
================
GameController Private
================
*/
void GameController::HandleIncoming()
{
	while (_tcpSocket->HasActivity()) {
		Packet *pkt = _tcpSocket->GetPacket();
		if (pkt) 
			HandlePacket(pkt);
		else break;
	}

	while (_udpSocket->HasActivity()) {
		Packet *pkt = _udpSocket->GetPacket();
		if (pkt)
			HandlePacket(pkt);
		else break;
	}
}

void GameController::HandlePacket(Packet *pkt)
{
	// Great stuff
	delete pkt;
}
