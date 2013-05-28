#include "UPnP.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <iostream>

using namespace std;

namespace modupnp {

	const char* UPnP::UPNP_BCAST_ADDR = "239.255.255.250";

	/**
	 * Entry point
	 */
	UPnP::discoverres_t UPnP::discover(const char *servicetype, long timeout)
	{
		int sock = socket(AF_INET, SOCK_DGRAM, 0);
		sockaddr_in addr;
		UPnP::discoverres_t result;

		string bcastMsg = "M-SEARCH * HTTP/1.1\r\n";
		bcastMsg += "ST: ";
		bcastMsg += servicetype; //upnp:rootdevice
		bcastMsg += "\r\n";
		bcastMsg += "MX: 3\r\n";
		bcastMsg += "MAN: \"ssdp:discover\"\r\n";
		bcastMsg += "HOST: 239.255.255.250:1900\r\n\r\n";

		if (sock < 0) {
			throw UPnPException();
		}

		// Enable broadcast
		int broadcastEnable = 1;
		setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

		// Init address
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(UPnP::UPNP_BCAST_ADDR);
		addr.sin_port = htons(UPnP::UPNP_BCAST_PORT);

		// send bcast
		sendto(sock, bcastMsg.c_str(), bcastMsg.size(), 0, (sockaddr*)(&addr), sizeof(addr));

		// Receive variables
		fd_set fds;
		timeval _timeout;
		char recvBuf[4096];
		sockaddr_in clientAddr;

		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		_timeout.tv_sec = timeout;
		_timeout.tv_usec = 0;

		while (select(sock + 1, &fds, NULL, NULL, &_timeout) > 0) {
			_timeout.tv_sec = 5;
			_timeout.tv_usec = 0;

			memset(&clientAddr, 0, sizeof(clientAddr));
			memset(&recvBuf, 0, sizeof(recvBuf));
			socklen_t addrLen = sizeof(clientAddr);
			recvfrom(sock, &recvBuf, sizeof(recvBuf), 0, (sockaddr*)&clientAddr, &addrLen);

			char addrstr[INET_ADDRSTRLEN];
			memset(addrstr, 0, sizeof(addrstr));
			inet_ntop(AF_INET, &(clientAddr.sin_addr), addrstr, sizeof(addrstr));

			string ip(addrstr);
			result[ip].push_back(string(recvBuf));
		}

		return result;
	}
}

