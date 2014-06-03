#pragma once

#include <trutle/Trutle.h>
#include "net/NetworkController.h"
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
};
