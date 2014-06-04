#include "GameController.h"
#include "GameScene.h"
#include "net/Packet.h"



/*
================
GameController public methods
================
*/
GameController::GameController(string adrs)
{
	address = adrs;
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

	_tcpSocket = new Socket(TCP, address, TCP_SERVER_PORT);
	_udpSocket = new Socket(UDP, address, UDP_SERVER_PORT, 0);

	PacketJoinRequest jreq;
	jreq.type = PACKET_JOIN_REQUEST;
	jreq.port = _udpSocket->GetListenPortUDP();
	_tcpSocket->SendPacket(&jreq);
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
	if (!_gameScene->HandlePacket(pkt)) {
		Log::Debug("Unhandled packet :" + PacketTypeStr(pkt->type));
	}

	delete pkt;
}
