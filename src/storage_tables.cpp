//
//	License type: BSD 3-Clause License
//	License copy: https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/LICENSE
//
//	Created by Stephane Bourque on 2021-03-04.
//	Arilia Wireless Inc.
//

#include "uStorageService.h"

namespace uCentral::Storage {

	int Service::Create_Tables() {

		Create_Statistics();
		Create_Devices();
		Create_Capabilities();
		Create_HealthChecks();
		Create_Authentication();
		Create_DeviceLogs();
		Create_DefaultConfigs();
		Create_CommandList();
		Create_BlackList();
		Create_FileUploads();
		Create_FirmwareUpgrades();
		Create_LifetimeStats();

		return 0;
	}

	int Service::Create_Statistics() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if ( dbType_== pgsql || dbType_ == sqlite) {
				Sess << "CREATE TABLE IF NOT EXISTS Statistics ("
						"SerialNumber VARCHAR(30), "
						"UUID INTEGER, "
						"Data TEXT, "
						"Recorded BIGINT)",
					Poco::Data::Keywords::now;
				Sess << "CREATE INDEX IF NOT EXISTS StatsSerial ON Statistics (SerialNumber ASC, Recorded ASC)",
					Poco::Data::Keywords::now;
			} else if (dbType_ == mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS Statistics ("
						"SerialNumber VARCHAR(30), "
						"UUID INTEGER, "
						"Data TEXT, "
						"Recorded BIGINT, "
						"INDEX StatSerial (SerialNumber ASC, Recorded ASC))",
					Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_Devices() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS Devices ("
						"SerialNumber  	VARCHAR(30) UNIQUE PRIMARY KEY, "
						"DeviceType    	VARCHAR(32), "
						"MACAddress    	VARCHAR(30), "
						"Manufacturer  	VARCHAR(64), "
						"Configuration 	TEXT, "
						"Notes         	TEXT, "
						"Owner  		VARCHAR(64), "
						"Location  		TEXT, "
						"Firmware 		VARCHAR(128),"
						"Compatible		VARCHAR(128),"
						"FWUpdatePolicy	VARCHAR(128),"
						"UUID          	BIGINT,      "
						"CreationTimestamp BIGINT,   "
						"LastConfigurationChange BIGINT, "
						"LastConfigurationDownload BIGINT, "
						"LastFWUpdate	BIGINT		"
						",INDEX DeviceOwner (Owner ASC),"
						"INDEX LocationIndex (Location ASC))", Poco::Data::Keywords::now;
			} else if(dbType_==sqlite || dbType_==pgsql) {
				Sess << "CREATE TABLE IF NOT EXISTS Devices ("
						"SerialNumber  	VARCHAR(30) UNIQUE PRIMARY KEY, "
						"DeviceType    	VARCHAR(32), "
						"MACAddress    	VARCHAR(30), "
						"Manufacturer  	VARCHAR(64), "
						"Configuration 	TEXT, "
						"Notes         	TEXT, "
						"Owner  		VARCHAR(64), "
						"Location  		TEXT, "
						"Firmware 		VARCHAR(128),"
						"Compatible		VARCHAR(128),"
						"FWUpdatePolicy	VARCHAR(128),"
						"UUID          	BIGINT,      "
						"CreationTimestamp BIGINT,   "
						"LastConfigurationChange BIGINT, "
						"LastConfigurationDownload BIGINT, "
						"LastFWUpdate	BIGINT		"
						")", Poco::Data::Keywords::now;
				Sess << "CREATE INDEX IF NOT EXISTS DeviceOwner ON Devices (Owner ASC)", Poco::Data::Keywords::now;
				Sess << "CREATE INDEX IF NOT EXISTS DeviceLocation ON Devices (Location ASC)", Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_Capabilities() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==pgsql || dbType_==sqlite || dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS Capabilities ("
						"SerialNumber VARCHAR(30) PRIMARY KEY, "
						"Capabilities TEXT, "
						"FirstUpdate BIGINT, "
						"LastUpdate BIGINT"
						")",
					Poco::Data::Keywords::now;
			}

			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_HealthChecks() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS HealthChecks ("
						"SerialNumber VARCHAR(30), "
						"UUID          BIGINT, "
						"Data TEXT, "
						"Sanity BIGINT , "
						"Recorded BIGINT, "
						"INDEX HealthSerial (SerialNumber ASC, Recorded ASC)"
						")", Poco::Data::Keywords::now;
			} else if(dbType_==sqlite) {
				Sess << "CREATE TABLE IF NOT EXISTS HealthChecks ("
						"SerialNumber VARCHAR(30), "
						"UUID          BIGINT, "
						"Data TEXT, "
						"Sanity BIGINT , "
						"Recorded BIGINT) ", Poco::Data::Keywords::now;
				Sess << "CREATE INDEX IF NOT EXISTS HealthSerial ON HealthChecks (SerialNumber ASC, Recorded ASC)", Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_Authentication() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==pgsql || dbType_==sqlite || dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS Authentication ("
						"Identity			VARCHAR(128) PRIMARY KEY, "
						"Password			VARCHAR(128), "
						"AccessType			INT, "
						"Created			BIGINT, "
						"Modified			BIGINT, "
						"Expires 			BIGINT, "
						"CreatedBy			VARCHAR(128), "
						"ACLRead 			INT, "
						"ACLReadWrite 		INT, "
						"ACLReadWriteCreate INT, "
						"ACLDelete 			INT, "
						"ACLPortal 			INT "
						") ", Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_DeviceLogs() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS DeviceLogs ("
						"SerialNumber   VARCHAR(30), "
						"Log            TEXT, "
						"Data           TEXT, "
						"Severity       BIGINT, "
						"Recorded       BIGINT, "
						"LogType        BIGINT, "
						"UUID	        BIGINT, "
						"INDEX LogSerial (SerialNumber ASC, Recorded ASC)"
						")", Poco::Data::Keywords::now;
			} else if(dbType_==pgsql || dbType_==sqlite) {
				Sess << "CREATE TABLE IF NOT EXISTS DeviceLogs ("
						"SerialNumber   VARCHAR(30), "
						"Log            TEXT, "
						"Data           TEXT, "
						"Severity       BIGINT, "
						"Recorded       BIGINT, "
						"LogType        BIGINT, "
						"UUID	        BIGINT  "
						")", Poco::Data::Keywords::now;
				Sess << "CREATE INDEX IF NOT EXISTS LogSerial ON DeviceLogs (SerialNumber ASC, Recorded ASC)", Poco::Data::Keywords::now;
			}

			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_DefaultConfigs() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==pgsql || dbType_==sqlite || dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS DefaultConfigs ("
						"Name VARCHAR(30) PRIMARY KEY, "
						"Configuration TEXT, "
						"Models TEXT, "
						"Description TEXT, "
						"Created BIGINT , "
						"LastModified BIGINT)", Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_CommandList() {
		try {
			Poco::Data::Session Sess = Pool_->get();
			if(dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS CommandList ("
						"UUID           VARCHAR(64) PRIMARY KEY, "
						"SerialNumber   VARCHAR(30), "
						"Command        VARCHAR(32), "
						"Status         VARCHAR(64), "
						"SubmittedBy    VARCHAR(64), "
						"Results        TEXT, "
						"Details        TEXT, "
						"ErrorText      TEXT, "
						"Submitted      BIGINT, "
						"Executed       BIGINT, "
						"Completed      BIGINT, "
						"RunAt          BIGINT, "
						"ErrorCode      BIGINT, "
						"Custom         BIGINT, "
						"WaitingForFile BIGINT, "
						"AttachDate     BIGINT,"
						"AttachSize     BIGINT,"
						"AttachType     VARCHAR(64),"
						"INDEX CommandListIndex (SerialNumber ASC, Submitted ASC)"
						")", Poco::Data::Keywords::now;
			} else if (dbType_==pgsql || dbType_==sqlite) {
				Sess << "CREATE TABLE IF NOT EXISTS CommandList ("
						"UUID           VARCHAR(64) PRIMARY KEY, "
						"SerialNumber   VARCHAR(30), "
						"Command        VARCHAR(32), "
						"Status         VARCHAR(64), "
						"SubmittedBy    VARCHAR(64), "
						"Results        TEXT, "
						"Details        TEXT, "
						"ErrorText      TEXT, "
						"Submitted      BIGINT, "
						"Executed       BIGINT, "
						"Completed      BIGINT, "
						"RunAt          BIGINT, "
						"ErrorCode      BIGINT, "
						"Custom         BIGINT, "
						"WaitingForFile BIGINT, "
						"AttachDate     BIGINT,"
						"AttachSize     BIGINT,"
						"AttachType     VARCHAR(64)"
						")", Poco::Data::Keywords::now;
				Sess << "CREATE INDEX IF NOT EXISTS CommandListIndex ON CommandList (SerialNumber ASC, Submitted ASC)", Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_BlackList() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==mysql || dbType_==pgsql || dbType_==sqlite) {
				Sess << "CREATE TABLE IF NOT EXISTS BlackList ("
						"SerialNumber	VARCHAR(30) PRIMARY KEY, "
						"Reason			TEXT, "
						"Created		BIGINT, "
						"Author			VARCHAR(64)"
						")", Poco::Data::Keywords::now;
			}
			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_FileUploads() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==sqlite) {
				Sess << "CREATE TABLE IF NOT EXISTS FileUploads ("
						"UUID			VARCHAR(64) PRIMARY KEY, "
						"Type			VARCHAR(32), "
						"Created 		BIGINT, "
						"FileContent	BLOB"
						") ", Poco::Data::Keywords::now;
			} else if(dbType_==mysql) {
				Sess << "CREATE TABLE IF NOT EXISTS FileUploads ("
						"UUID			VARCHAR(64) PRIMARY KEY, "
						"Type			VARCHAR(32), "
						"Created 		BIGINT, "
						"FileContent	LONGBLOB"
						") ", Poco::Data::Keywords::now;
			} else if(dbType_==pgsql) {
				Sess << "CREATE TABLE IF NOT EXISTS FileUploads ("
						"UUID			VARCHAR(64) PRIMARY KEY, "
						"Type			VARCHAR(32), "
						"Created 		BIGINT, "
						"FileContent	BYTEA"
						") ", Poco::Data::Keywords::now;
			}

			return 0;
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_FirmwareUpgrades() {
		try {
			Poco::Data::Session Sess = Pool_->get();

			if(dbType_==sqlite || dbType_==mysql || dbType_==pgsql) {
				Sess << "CREATE TABLE IF NOT EXISTS PendingFirmwareUpgrades ("
						"SerialNumber   VARCHAR(30) PRIMARY KEY, "
						"UUID			VARCHAR(64), 	"
						"NewFirmware 	VARCHAR(128), 	"
						"OldFirmware 	VARCHAR(128), 	"
						"URI			TEXT,			"
						"ScheduleAt 	BIGINT, 		"
						"Created 		BIGINT, 		"
						"UpdateDone		BIGINT			"
						") ",
					Poco::Data::Keywords::now;
				return 0;
			}

		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

	int Service::Create_LifetimeStats() {
		try {
			Poco::Data::Session Sess = Pool_->get();
			if(dbType_==sqlite || dbType_==mysql || dbType_==pgsql) {
				Sess << "CREATE TABLE IF NOT EXISTS LifetimeStats ("
						"SerialNumber   VARCHAR(30) PRIMARY KEY, "
						"Statistics		TEXT, "
						"Created 		BIGINT, 		"
						"Updated		BIGINT			"
						") ",
					Poco::Data::Keywords::now;
				return 0;
			}
		} catch(const Poco::Exception &E) {
			Logger_.log(E);
		}
		return -1;
	}

}