//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#include "RESTAPI_server.h"

#include "Poco/URI.h"

#include "RESTAPI_BlackList.h"
#include "RESTAPI_callback.h"
#include "RESTAPI_command.h"
#include "RESTAPI_commands.h"
#include "RESTAPI_default_configuration.h"
#include "RESTAPI_default_configurations.h"
#include "RESTAPI_device_commandHandler.h"
#include "RESTAPI_device_handler.h"
#include "RESTAPI_devices_handler.h"
#include "RESTAPI_file.h"
#include "RESTAPI_oauth2Handler.h"
#include "RESTAPI_system_command.h"

#include "RESTAPI_unknownRequestHandler.h"
#include "uUtils.h"

namespace uCentral::RESTAPI {

    Service *Service::instance_ = nullptr;

    int Start() {
        return Service::instance()->Start();
    }

    void Stop() {
        Service::instance()->Stop();
    }

    Service::Service() noexcept: uSubSystemServer("RESTAPIServer", "RESTAPIServer", "ucentral.restapi")
    {
    }

    int Service::Start() {
        Logger_.information("Starting.");

        for(const auto & Svr: ConfigServersList_) {
			Logger_.information(Poco::format("Starting: %s:%s Keyfile:%s CertFile: %s", Svr.Address(), std::to_string(Svr.Port()),
											 Svr.KeyFile(),Svr.CertFile()));

            auto Sock{Svr.CreateSecureSocket(Logger_)};

//			Sock.setReceiveTimeout(Poco::Timespan(10,0));

			Svr.LogCert(Logger_);
			if(!Svr.RootCA().empty())
				Svr.LogCas(Logger_);

            auto Params = new Poco::Net::HTTPServerParams;
            Params->setMaxThreads(50);
            Params->setMaxQueued(200);
			Params->setKeepAlive(true);
//			uint64_t T = 45000;
//			Params->setKeepAliveTimeout(T);
//			Params->setMaxKeepAliveRequests(200);
//			Params->setTimeout();

            auto NewServer = std::make_unique<Poco::Net::HTTPServer>(new RequestHandlerFactory, Pool_, Sock, Params);
            NewServer->start();
            RESTServers_.push_back(std::move(NewServer));
        }

        return 0;
    }

    Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest & Request) {

        Logger_.debug(Poco::format("REQUEST(%s): %s %s", uCentral::Utils::FormatIPv6(Request.clientAddress().toString()), Request.getMethod(), Request.getURI()));

        Poco::URI uri(Request.getURI());
        const auto & Path = uri.getPath();
        RESTAPIHandler::BindingMap bindings;

        if (RESTAPIHandler::ParseBindings(Path, "/api/v1/oauth2/{token}", bindings)) {
            return new RESTAPI_oauth2Handler(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/oauth2", bindings)) {
            return new RESTAPI_oauth2Handler(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/devices", bindings)) {
            return new RESTAPI_devices_handler(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/device/{serialNumber}/{command}", bindings)) {
            return new RESTAPI_device_commandHandler(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/device/{serialNumber}", bindings)) {
            return new RESTAPI_device_handler(bindings, Logger_);
        }  else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/default_configurations", bindings)) {
            return new RESTAPI_default_configurations(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/default_configuration/{name}", bindings)) {
            return new RESTAPI_default_configuration(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/command/{commandUUID}", bindings)) {
            return new RESTAPI_command(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/commands", bindings)) {
            return new RESTAPI_commands(bindings, Logger_);
        } else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/file/{uuid}", bindings)) {
            return new RESTAPI_file(bindings, Logger_);
		} else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/system", bindings)) {
			return new RESTAPI_system_command(bindings, Logger_);
		} else if (RESTAPIHandler::ParseBindings(Path, "/api/v1/blacklist", bindings)) {
			return new RESTAPI_BlackList(bindings, Logger_);
		} else if(RESTAPIHandler::ParseBindings(Path, "/api/v1/callbackChannel", bindings)) {
			return new RESTAPI_callback(bindings, Logger_);
		}

		Logger_.error(Poco::format("INVALID-API-ENDPOINT: %s",Path));
        return new RESTAPI_UnknownRequestHandler(bindings,Logger_);
    }

    void Service::Stop() {
        Logger_.information("Stopping ");
        for( const auto & svr : RESTServers_ )
            svr->stop();
    }

}  // namespace