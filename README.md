# Pman
Pman is a process management tool for starting/stopping, automatic restart, and status monitoring.  
Pman intends to run on a UNIX-based OS, and currently it has been tested on Mac OSX and Ubuntu.  
Pman is inspired by [Supervisor](https://github.com/Supervisor/supervisor) in several ways.

## Usage
### Configuration file
Pman reads the configuration file (default: /etc/pman.conf) at startup.  
The configuration file has a section that describes the settings of the daemon and a section that describes the setting of the program to be executed.
Please see [sample conf file](https://github.com/matsune/pman/blob/master/pman.conf).

### Start server
```shell
$ pman
# specific conf file path
$ pman -c pman.conf
```

### Run client
```shell
$ pman <command> [<args>]

command:
   status <program>      show status
   status all            show status of all programs
   start <program>       start program
   start all             start all programs which is not running
   stop <program>        stop program
   stop all              stop all programs which is running
   restart <program>     stop and start program
   restart all           restart all programs
   kill                  kill daemon process
```

## gRPC
Pman server adopts [gRPC](https://grpc.io) as a protocol for IPC.  
You can call pman's API from client program written in programming language supporting gRPC.  
Please check [pman.proto](https://github.com/matsune/pman/blob/master/protos/pman.proto) file.
