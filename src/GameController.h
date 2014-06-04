#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif


#include "net/Socket.h"


class GameController : public Controller {
public:
	GameController();
	~GameController();

	void LoadContent();
	void Update(DeltaTime &dt);

private:
	Socket *_tcpSocket;
	Socket *_udpSocket;

	void HandleIncoming();
	void HandlePacket(Packet *pkt);
};
