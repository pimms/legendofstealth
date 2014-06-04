#pragma once

#ifdef _WIN32
	#include <Trutle.h>
#else
	#include <trutle/Trutle.h>
#endif

#include "net/Packet.h"


// forward declaration
class Socket;


/* Player Update Component
 *
 * Component which sends and retrieves player updates. Can be 
 * attached to both local and remote players.
 */
class PlayerUpdateComponent : public Component {
public:
	~PlayerUpdateComponent();
	void Update(const DeltaTime &dt);

	void SetUDPSocket(Socket *updSocket);

	void SendUpdatePacket();
	void HandleUpdatePacket(const PacketPlayerUpdate *packet);

private:
	Socket *_udpSocket;

	float _timer;

};
