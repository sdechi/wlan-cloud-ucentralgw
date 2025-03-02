//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#ifndef UCENTRALGW_RESTAPI_PROTOCOL_H
#define UCENTRALGW_RESTAPI_PROTOCOL_H

namespace uCentral::RESTAPI::Protocol {
	static const char * CAPABILITIES = "capabilities";
	static const char * LOGS = "logs";
	static const char * HEALTHCHECKS = "healthchecks";
	static const char * STATISTICS = "statistics";
	static const char * STATUS = "status";
	static const char * SERIALNUMBER = "serialNumber";
	static const char * PERFORM = "perform";
	static const char * CONFIGURE = "configure";
	static const char * UPGRADE = "upgrade";
	static const char * REBOOT = "reboot";
	static const char * FACTORY = "factory";
	static const char * LEDS = "leds";
	static const char * TRACE = "trace";
	static const char * REQUEST = "request";
	static const char * WIFISCAN = "wifiscan";
	static const char * EVENTQUEUE = "eventqueue";
	static const char * RTTY = "rtty";
	static const char * COMMAND = "command";
	static const char * STARTDATE = "startDate";
	static const char * ENDDATE = "endDate";
	static const char * OFFSET = "offset";
	static const char * LIMIT = "limit";
	static const char * LIFETIME = "lifetime";
	static const char * UUID = "UUID";
	static const char * DATA = "data";
	static const char * CONFIGURATION = "configuration";
	static const char * WHEN = "when";
	static const char * URI = "uri";
	static const char * LOGTYPE = "logType";
	static const char * VALUES = "values";
	static const char * TYPES = "types";
	static const char * PAYLOAD = "payload";
	static const char * KEEPREDIRECTOR = "keepRedirector";
	static const char * NETWORK = "network";
	static const char * INTERFACE = "interface";
	static const char * BANDS = "bands";
	static const char * CHANNELS = "channels";
	static const char * VERBOSE = "verbose";
	static const char * MESSAGE = "message";
	static const char * STATE = "state";
	static const char * HEALTHCHECK = "healthcheck";
	static const char * PCAP_FILE_TYPE = "pcap";
	static const char * DURATION = "duration";
	static const char * NUMBEROFPACKETS = "numberOfPackets";
	static const char * FILTER = "filter";
	static const char * SELECT = "select";
	static const char * SERIALONLY = "serialOnly";
	static const char * COUNTONLY = "countOnly";
	static const char * DEVICEWITHSTATUS = "deviceWithStatus";
	static const char * DEVICESWITHSTATUS = "devicesWithStatus";
	static const char * DEVICES = "devices";
	static const char * COUNT = "count";
	static const char * SERIALNUMBERS = "serialNumbers";
	static const char * CONFIGURATIONS = "configurations";
	static const char * NAME = "name";
	static const char * COMMANDS = "commands";
	static const char * COMMANDUUID = "commandUUID";
	static const char * FIRMWARES = "firmwares";
	static const char * TOPIC = "topic";
	static const char * REASON = "reason";
	static const char * FILEUUID = "uuid";
	static const char * USERID = "userId";
	static const char * PASSWORD = "password";
	static const char * TOKEN = "token";
	static const char * SETLOGLEVEL = "setloglevel";
	static const char * GETLOGLEVEL = "getloglevel";
	static const char * PARAMETERS = "parameters";
	static const char * VALUE = "value";
	static const char * LASTONLY = "lastOnly";
	static const char * NEWEST = "newest";
	static const char * ACTIVESCAN = "activeScan";
}

#endif // UCENTRALGW_RESTAPI_PROTOCOL_H
