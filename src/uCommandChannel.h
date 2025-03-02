//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#ifndef UCENTRALGW_UCOMMANDCHANNEL_H
#define UCENTRALGW_UCOMMANDCHANNEL_H

#include "uSubSystemServer.h"

#include "Poco/File.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"

namespace uCentral::CommandChannel {
	int Start();
	void Stop();

class Service : public uSubSystemServer {
  public:
	Service() noexcept;

	friend int Start();
	friend void Stop();

	static Service *instance() {
		if (instance_ == nullptr) {
			instance_ = new Service;
		}
		return instance_;
	}

	std::string  ProcessCommand(const std::string &Command);

  private:
	int Start() override;
	void Stop() override;

	static Service * instance_;
	std::unique_ptr<Poco::File> 				SocketFile_;
	std::unique_ptr<Poco::Net::SocketAddress>	UnixSocket_;
	std::unique_ptr<Poco::Net::ServerSocket>	Svs_;
	std::unique_ptr<Poco::Net::TCPServer>		Srv_;
};

}		//namespace

#endif // UCENTRALGW_UCOMMANDCHANNEL_H
