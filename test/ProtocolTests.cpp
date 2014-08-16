#include "Tests.h"
#include "client/Client.h"
#include "Protocol.h"


TEST(ProtocolTest, TestServerJoin)
{
	if (start_server() < 0) {
		FAIL();
	}

	client::ServerConnector connector("localhost");

	// Prematurely calling any getter should throw an exception
	ASSERT_ANY_THROW(connector.GetTCPSocket());
	ASSERT_ANY_THROW(connector.GetUDPSocket());
	ASSERT_ANY_THROW(connector.GetPlayerID());
	ASSERT_ANY_THROW(connector.GetTeam());

	// Connect to the server
	ASSERT_FALSE(connector.IsConnectedToServer());
	ASSERT_TRUE(connector.ConnectToServer());
	ASSERT_TRUE(connector.IsConnectedToServer());

	// The setters are now expected to give valid values
	Socket *tcp = NULL;
	Socket *udp = NULL;
	unsigned playerID;
	Team team;

	ASSERT_NO_THROW( tcp = connector.GetTCPSocket(); );
	ASSERT_NO_THROW( udp = connector.GetUDPSocket(); );
	ASSERT_NO_THROW( playerID = connector.GetPlayerID(); );
	ASSERT_NO_THROW( team = connector.GetTeam(); );

	// Assert valid values
	ASSERT_TRUE(tcp != NULL);
	ASSERT_TRUE(tcp->GetTransportLayerProtocol() == TCP);
	ASSERT_TRUE(tcp->IsConnectionOpen());

	ASSERT_TRUE(udp != NULL);
	ASSERT_TRUE(udp->GetTransportLayerProtocol() == UDP);

	ASSERT_TRUE(playerID != 0);
	ASSERT_TRUE(team == TEAM_SPY || team == TEAM_MERC);

	// Delete the sockets and kill the server
	delete tcp;
	delete udp;
	kill_server();
}
