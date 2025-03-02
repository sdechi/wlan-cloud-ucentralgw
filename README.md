# uCentralGW

## What is this?
The uCentralGW is an added service for the TIP controller that allows integration with the 
uCentral protocol. It supports a complete OpenAPI definition and uses the ucentral communication protocol. To use the uCentralGW,
you either need to [build it](#building) or use the [Docker version](#docker).

## Building
In order to build the uCentralGW, you will need to install its dependencies, which includes the following:
- cmake
- boost
- POCO 1.10.1 or later
- a C++17 compiler
- libyaml
- openssl
- libpq-dev (PortgreSQL development libraries)
- mysql-client (MySQL client)
- librdkafka
- cppkafka

The build is done in 2 parts. The first part is to build a local copy of the framework tailored to your environment. This 
framework is called [Poco](https://github.com/pocoproject/poco). The version used in this project has a couple of fixes
from the master copy needed for cmake. Please use the version of this [Poco fix](https://github.com/stephb9959/poco). Building 
Poco may take several minutes depending on the platform you are building on.

### Ubuntu
These instructions have proven to work on Ubuntu 20.4.
```
sudo apt install git cmake g++ libssl-dev libmariabd-dev-compat unixodbc-dev 
sudo apt install libpq-dev libaprutil1-dev apache2-dev libboost-all-dev libyaml-cpp-dev
sudo apt install librdkafka-dev lua5.3-dev

git clone https://github.com/stephb9959/poco
cd poco
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

git clone https://github.com/stephb9959/cppkafka
cd cppkafka
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/Telecominfraproject/wlan-cloud-ucentralgw
cd wlan-cloud-ucentralgw
mkdir cmake-build
cd cmake-build
cmake ..
make
```

### Fedora
The following instructions have proven to work on Fedora 33
```
sudo yum install cmake g++ openssl-devel unixODBC-devel mysql-devel mysql apr-util-devel boost boost-devel
sudo yum install yaml-cpp-devel lua-devel 
sudo dnf install postgresql.x86_64 librdkafka-devel
sudo dnf install postgresql-devel

git clone https://github.com/stephb9959/poco
cd poco
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

git clone https://github.com/stephb9959/cppkafka
cd cppkafka
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/Telecominfraproject/wlan-cloud-ucentralgw
cd wlan-cloud-ucentralgw
mkdir cmake-build
cd cmake-build
cmake ..
make

```
 
### OSX Build
The following instructions have proven to work on OSX Big Sur. You need to install [Homebrew](https://brew.sh/). You must also have installed [XCode for OS X](https://www.freecodecamp.org/news/how-to-download-and-install-xcode/).
```
brew install openssl
brew install cmake
brew install libpq
brew install mysql-client
brew install apr
brew install apr-util
brew install boost
brew install yaml-cpp
brew install postgresql
brew install unixodbc
brew install librdkafka

git clone https://github.com/stephb9959/poco
cd poco
mkdir cmake-build 
cd cmake-build
cmake ..
cmake --build . --config Release -j
sudo cmake --build . --target install

git clone https://github.com/stephb9959/cppkafka
cd cppkafka
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/Telecominfraproject/wlan-cloud-ucentralgw
cd wlan-cloud-ucentralgw
mkdir cmake-build
cd cmake-build
cmake ..
make -j
```

### Raspberry
The build on a rPI takes a while. You can shorten that build time and requirements by disabling all the larger database 
support. You can build with only SQLite support by not installing the packages for ODBC, PostgreSQL, and MySQL by 
adding -DSMALL_BUILD=1 on the cmake build line.

```
sudo apt install git cmake g++ libssl-dev libaprutil1-dev apache2-dev libboost-all-dev libyaml-cpp-dev
git clone https://github.com/stephb9959/poco
cd poco
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release
sudo cmake --build . --target install

cd ~
git clone https://github.com/Telecominfraproject/wlan-cloud-ucentralgw
cd wlan-cloud-ucentralgw
mkdir cmake-build
cd cmake-build
cmake -DSMALL_BUILD=1 ..
make
```

### After the build step is completed
Once your build is done. You can remove the Poco source as it is no longer needed. 

#### Expected directory layout
From the directory where your cloned source is, you will need to create the `certs`, `logs`, and `uploads` directories.
```shell
mkdir certs
mkdir certs/cas
mkdir logs
mkdir uploads
```

You should now have the following:

```
-- cert_scripts
  |
  +-- certs
  |   +--- cas
  +-- cmake
  +-- cmake-build
  +-- logs (dir)
  +-- src
  +-- test_scripts
  +-- openapi
  +-- uploads
  +-- ucentral.properties
```
### Certificates
Love'em of hate'em, we gotta use'em. So we tried to make this as easy as possible for you. 

#### The `certs` directory
For all deployments, you will need the following certs directory, populated with the proper files.

```asm
certs ---+--- root.pem
         +--- issuer.pem
         +--- websocket-cert.pem
         +--- websocket-key.pem
         +--- clientcas.pem
         +--- +cas
         |    +--- issuer.pem
         |    +--- root.pem
         +--- restapi-ca.pem
         +--- restapi-cert.pem
         +--- restapi-key.pem
```

#### DigiCert files
These are the files you should install on your gateway and devices. For your gateway, you will need to provide tge following files in the directory above
- `root.pem` is [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/root.pem).
- `issuer.pem` is [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/issuer.pem).
- `clientcas.pem` is [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/clientcas.pem).

#### Gateway certificates: TIP portion (* must be obtained from TIP)
The gateway requires its own DigiCert certificate. Once obtained, you need to identify the `key` and the `certificate` rename
them `websocket-key.pem` and `websocket-cert.pem`, and copy them in your `certs` directory. These files mus be obtained from TIP.

#### Gateway certificates: for REST API
The gateway requires a key/vertificate/ca for the REST interface. These files you need to obtain on your own of generate them. This is beyond the scope of this 
document. Once you have these files, you need to renamed them `restapi-key.pem`, `restapi-cert.pem`, and `restapi-ca.pem`. This will guarantee proper HTTPS
in your browner 

#### Configuration
The configuration for this service is kept in a properties file. This file is called `ucentral.properties` and you can 
see the latest version [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/ucentral.properties). The file will be loaded from
the directory set by the environment variable `UCENTRAL_CONFIG`. To use environment variables in the configuration,
you must use `$<varname>`. Only `path names` support the use of environment variables. The sample configuration requires very 
little changes if you keep the suggested directory structure. For the sample configuration to work, you need to define 2 
environment variables. 
```
export UCENTRAL_ROOT=`pwd`
export UCENTRAL_CONFIG=`pwd`
```
If you current working directory is the root of the project, this will set the variables properly. Otherwise, you can set the variables 
to point to wherever is necessary.

##### Important config entries
###### This is the logging directory
```
logging.channels.c2.path = $UCENTRAL_ROOT/logs/sample.log
```

###### This is the type of storage in use
```asm
storage.type = sqlite
```

###### Autoprovisioning settings
```asm
ucentral.autoprovisioning = true
ucentral.devicetypes.0 = AP:linksys_ea8300,edgecore_eap101,linksys_e8450-ubi
ucentral.devicetypes.1 = SWITCH:edgecore_ecs4100-12ph
ucentral.devicetypes.2 = IOT:esp32
```

###### This is the RESTAPI endpoint

```asm
ucentral.restapi.host.0.backlog = 100
ucentral.restapi.host.0.security = relaxed
ucentral.restapi.host.0.rootca = $UCENTRAL_ROOT/certs/restapi-ca.pem
ucentral.restapi.host.0.address = *
ucentral.restapi.host.0.port = 16001
ucentral.restapi.host.0.cert = $UCENTRAL_ROOT/certs/restapi-cert.pem
ucentral.restapi.host.0.key = $UCENTRAL_ROOT/certs/restapi-key.pem
ucentral.restapi.host.0.key.password = mypassword
```

##### This is the end point for the devices to connect with
This is the crucial section. I bet that 97.4% of all your problems will come from here, and it's boring. So put some good music on, 
give the kids the iPad, get a cup of coffee, and pay attention. Every field will be explained.

###### ucentral.websocket.host.0.backlog
This is the number of concurrent devices you are expecting to call all at once. Not the current number of devices. This is how many will connect in the same exact second. 
Take the total number of devices you have and divide by 100. That's a good rule of thumb. Never go above 500.

###### ucentral.websocket.host.0.rootca
This is the root file as supplied by Digicert. You can find it [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/root.pem) 

###### ucentral.websocket.host.0.issuer
This is the issuer file as supplied by Digicert. You can find it [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/issuer.pem)

###### ucentral.websocket.host.0.cert
This is a `pem` file that you will receive from Digicert for the gateway itself. This is the certificate for the gateway. 

###### ucentral.websocket.host.0.key
This is a `pem` file that you will receive from Digicert for the gateway itself. The is the private key for the gateway.

###### ucentral.websocket.host.0.clientcas
This is a `pem` file that contains both the issuer and the root CA certificates. You can find it You can find it [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/clientcas.pem)

###### ucentral.websocket.host.0.cas
This is a directory where you will copy your own `cert.pem`, the `root.pem`, and the `issuer.pem` files.

###### ucentral.websocket.host.0.address
Leve this a `*` in teh case you want to bind to all interfaces on your gateway host or select the address of a single interface.

###### ucentral.websocket.host.0.port
Leave to 15002 for now. 

###### ucentral.websocket.host.0.security
Leave this as strict for now for devices.

###### ucentral.websocket.host.0.key.password
If you key file uses a password, please enter it here.

###### ucentral.websocket.maxreactors
A single reactor can handle between 1000-2000 devices. Never leave this smaller than 5 or larger than 50.

#### Conclusion 
You will need to get the `cert.pem` and `key.pem` from Digicert. The rest is here.

```asm
ucentral.websocket.host.0.backlog = 500
ucentral.websocket.host.0.rootca = $UCENTRAL_ROOT/certs/root.pem
ucentral.websocket.host.0.issuer = $UCENTRAL_ROOT/certs/issuer.pem
ucentral.websocket.host.0.cert = $UCENTRAL_ROOT/certs/websocket-cert.pem
ucentral.websocket.host.0.key = $UCENTRAL_ROOT/certs/websocket-key.pem
ucentral.websocket.host.0.clientcas = $UCENTRAL_ROOT/certs/clientcas.pem
ucentral.websocket.host.0.cas = $UCENTRAL_ROOT/certs/cas
ucentral.websocket.host.0.address = *
ucentral.websocket.host.0.port = 15002
ucentral.websocket.host.0.security = strict
ucentral.websocket.host.0.key.password = mypassword
ucentral.websocket.maxreactors = 20
```

###### This is the end point for the devices when uploading files
```asm
ucentral.fileuploader.host.0.backlog = 100
ucentral.fileuploader.host.0.rootca = $UCENTRAL_ROOT/certs/restapi-ca.pem
ucentral.fileuploader.host.0.security = relaxed
ucentral.fileuploader.host.0.address = *
ucentral.fileuploader.host.0.name = 192.168.1.176
ucentral.fileuploader.host.0.port = 16003
ucentral.fileuploader.host.0.cert = $UCENTRAL_ROOT/certs/restapi-cert.pem
ucentral.fileuploader.host.0.key = $UCENTRAL_ROOT/certs/restapi-key.pem
ucentral.fileuploader.host.0.key.password = mypassword
ucentral.fileuploader.path = $UCENTRAL_ROOT/uploads
ucentral.fileuploader.maxsize = 10000
```

###### host.0.address entries
If you want to limit traffic to a specific interface, you should specify the IP address of that interface instead of 
the `*`. Using the `*` means all interfaces will be able to accept connections. You can add multiple interfaces 
by changing the `0` to another index. You need to repeat the whole configuration block for each index. Indexes must be sequential
start at `0`.

###### ucentral.fileuploader.host.0.name
This must point to the IP or FQDN of your uCentralGW.

#### Running the gateway
Tu run the gateway, you must run the executable `ucentralgw`. You can use several command line options to run as a daemon or specify the configuration file location. 

#### Device configuration
Once you have the gateway configured, you will need to have some devices coming to it. For now, you will need to get
the following in order to use the gateway:
- A DigiCert certificate that you will call `cert.pem`
- A DigiCert key that goes with that certificate. Please call this `key.pem`
- The Digicert root certificate that you will find [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/certificates/root.pem). You must copy `root.pem`
and rename it `cas.pem` on the device.
- All 3 files mus be present in `/etc/ucentral` on the device.

You will need to upgrade your device to the latest firmware. Once updated, you will need to copy the 3 files mentionned above in 
  the `/etc/ucentral` directory. You will need to modify the `/etc/config-shadow/ucentral` file with your hostname. At which point, 
  you should be able to restart the uCentral client with `/etc/init.d/ucentral restart`. Then the command `logread -f` should tell you
  if you device was able to connect to the gateway.

#### Server key entry
The gateway needs to encrypt information from time to time. In order to do so, it must have a crypto key. This key
can be any of the keys you are already using. You must keep that keep secret and always use it. In the configutation,
this is the entry

```asm
ucentral.service.key = $UCENTRAL_ROOT/certs/websocket-key.pem
```
 
#### Command line options
The current implementation supports the following. If you use the built-in configuration file, you do not need to use any command-line
options. However, you may decide to use the `--daemon` or `umask` options. 

```bash
./ucentralgw --help
usage: ucentralgw OPTIONS
A uCentral gateway implementation for TIP.

--daemon        Run application as a daemon.
--umask=mask    Set the daemon's umask (octal, e.g. 027).
--pidfile=path  Write the process ID of the application to given file.
--help          display help information on command line arguments
--file=file     specify the configuration file
--debug         to run in debug, set to true
--logs=dir      specify the log directory and file (i.e. dir/file.log)
```

##### file
This allows you to point to another file without specifying the UCENTRAL_CONFIG variable. The file name must end in `.properties`.
##### daemon
Run this as a UNIX service
##### pidfile
When running as a daemon, the pid of the running service will be set in the speficied file
##### debug
Run the service in debug mode.
##### logs
Speficy where logs should be kept. You must include an existing directory and a file name. For example `/var/ucentral/logs/log.0`.
##### umask
Seet the umask for the running service.


### Docker
So building this thing from scratch is not your thing? I can't blame you. It takes some patience and 
in the end, there's still more work. Here comes `docker` to the rescue. You can run a docker version following
these instructions. The following is the content of the `docker_run.sh` script you can find
[here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/docker_run.sh):

```bash
#!/bin/sh

HUBNAME=tip-tip-wlan-cloud-ucentral.jfrog.io
IMAGE_NAME=ucentralgw
DOCKER_NAME=$HUBNAME/$IMAGE_NAME

CONTAINER_NAME=ucentralgw

#stop previously running images
docker container stop $CONTAINER_NAME
docker container rm $CONTAINER_NAME --force

if [[ ! -d logs ]]
then
    mkdir logs
fi

if [[ ! -d certs ]]
then
  echo "certs directory does not exist. Please create and add the proper certificates."
  exit 1
fi

if [[ ! -f ucentral.properties ]]
then
  echo "Configuration file ucentral.properties is missing in the current directory"
  exit 2
fi

docker run -d -p 15002:15002 \
              -p 16001:16001 \
              -p 16003:16003 \
              --init \
              --volume="$PWD:/ucentral-data" \
              -e UCENTRAL_ROOT="/ucentral-data" \
              -e UCENTRAL_CONFIG="/ucentral-data" \
              --name="ucentralgw" $DOCKER_NAME

```

Create yourself a directory and copy that script which you can also get from [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/docker_run.sh).
You must have the basic configuration file copied in the directory. This file must be called `ucentral.properties`. You can bring your own or
copy it from [here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/ucentral.properties). Please look at [this](#certificates-with-docker) to have the right 
certificates. You need to make sure that the names match the content of the `ucentral.properties`
file. Once all this is done, you can simply run `docker_run.sh`.

#### Docker installation directory layout
Here is the layout expected for your Docker installation

```asm
Run-time root
    |
    ----- certs (same as above)
    +---- logs  (dir)
    +---- uploads  (dir)
    +---- ucentral.properties (file)
```

#### `ucentral.properties` for Docker
If you use the pre-made configuration file, and you follow the directory layout, the only line you must change 
is the following line:

```asm
ucentral.fileuploader.host.0.name = 192.168.1.176
```

This line should reflect the IP of your gateway or its FQDN. You must make sure that this name or IP is accessible
from your devices. This is used during file uploads from the devices.

#### Certificates with Docker
Please refer to the `certs` directory from the sections above.

#### Configuration with Docker
The configuration for this service is kept in a properties file. Currently, this configuration file must be kept in the 
current directory of uCentral or one level up. This file is called `ucentral.properties` and you can see the latest version
[here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/ucentral.properties). The file will be loaded from 
the directory set by the environment variable `UCENTRAL_CONFIG`. To use environment variables in the configuration,
you must use `$<varname>`. The path for the logs for the service must exist prior to starting the 
service. The path is defined under `logging.channels.c2.path`. Only `path names` support the use of 
environment variables. Here is a sample configuration:

## uCentral communication protocol
The communication protocol between the device and the controller is detailed in this [document](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/PROTOCOL.md).

## OpenAPI
The service supports an OpenAPI REST based interface for management. You can find the [definition here](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/openapi/ucentral/ucentral.yaml).
And here is [how to use it](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/OPENAPI.md)

## Using the API
In the `test_scripts` directory, you will find a series of scripts that will show you how to use the API 
with [curl](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/TEST_CURL.md) 
or [python](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/TEST_PYTHON.md). 
More scripts will be added in the future.

## Firewall Considerations
- The protocol uses TCP port 15002 between the devices and the gateway. This port must be opened.
- Devices use the TCP port 16003 to upload files. This port is configurable in the `ucentral.properties` file. Look for `ucentral.fileuploader.host.0.port`.
- The RESTAPI is accessed through TCP port 16001 by default. This port is configurable in the `ucentral.properties` file. Look for the entry `ucentral.restapi.host.0.port`.

## Kafka integration
So what about Kafka? Well, the gateway has basic integration with Kafka. It is turned off by default, to turn it on, in the configuration:

```asm
ucentral.kafka.enable = false
ucentral.kafka.brokerlist = 127.0.0.1:9092
ucentral.kafka.commit = false
ucentral.kafka.queue.buffering.max.ms = 50
```

#### `ucentral.kafka.enable`
Kind of obvious but hey, set `true` or `false`. Default is `false`

#### `ucentral.kafka.brokerlist`
This is a comma separator list of the brokers in your `kafka` deployment. 

#### Kafka topics
Toe read more about Kafka, follow the [document](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/main/KAFKA.md)

#### Securing `kafka`
This is beyond the scope of this document. As it stands today, the communication between the gateway and `kafka` is expected to be behind a firewall.
## Contributors
We love ya! We need more of ya! If you want to contribute, make sure you review 
the [coding style](https://github.com/Telecominfraproject/wlan-cloud-ucentralgw/blob/master/CODING_STYLE.md) document. 
Feel free to ask questions and post issues. 
