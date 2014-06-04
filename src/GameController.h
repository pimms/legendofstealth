#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif


#include "net/Socket.h"

class GameScene;

class GameController : public Controller {
public:
	GameController(string adrs);
	~GameController();

	void LoadContent();
	void Update(DeltaTime &dt);

private:
	Socket *_tcpSocket;
	Socket *_udpSocket;
	string address;

	GameScene *_gameScene;


	void HandleIncoming();
	void HandlePacket(Packet *pkt);
};
