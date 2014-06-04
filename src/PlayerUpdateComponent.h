#pragma once

#include <trutle/Trutle.h>
#include "net/Packet.h"

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

};
