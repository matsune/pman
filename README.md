# Pman
Pman is a process management tool for starting/stopping, automatic restart, and status monitoring.  
Pman intends to run on a UNIX-based OS, and currently it has been tested on Mac OSX and Ubuntu.  
Pman is inspired by [Supervisor](https://github.com/Supervisor/supervisor) in several ways.

## Usage
### Configuration file
Pman reads the configuration file (default: /etc/pman.conf) at startup.  
The configuration file has 2 sections; a section that describes the settings of the daemon and a section that describes the setting of the program to be executed.
Pman has `echo` command that shows default configurations.

```shell
$ sudo sh -c './bin/pman echo > /etc/pman.conf'
$ cat /etc/pman.conf
[pman]
pidfile=/tmp/pman.pid 	; pman daemon's pidfile
logfile=/tmp/pman.log 	; pman daemon's logfile
port=127.0.0.1:50010  	; gRPC server port
directory=/tmp        	; default is current dir

[program:sample]
command=/bin/ls               	; program command
stdout=/tmp/sample_stdout.log 	; program stdout logfile (default: /tmp/${program name}_stdout.log)
stderr=/tmp/sample_stderr.log 	; program stderr logfile (default: /tmp/${program name}_stderr.log)
autorestart=true              	; automatically restart if exited unexpectedly
autostart=true                	; start program on daemon's startup
```

### Start server
```shell
$ pman
# specific conf file path
$ pman -c ./pman.conf
```

### Run client
```shell
$ pman <command>

command:
  status <program>  	show status
  start <program>   	start program
  stop <program>    	stop program
  restart <program> 	stop and start program
  kill              	kill daemon process
  echo              	echo default conf
```

You can use "all" to operate all registered programs.
```shell
$ pman stop all
[sample] STOPPING
[sample2] STOPPING
[sample3] STOPPING

$ pman start all
[sample] RUNNING 	pid: 8978	uptime: 00:00:00
[sample2] RUNNING 	pid: 8979	uptime: 00:00:00
[sample3] RUNNING 	pid: 8980	uptime: 00:00:00

$ pman status
[sample] RUNNING 	pid: 8978	uptime: 00:00:02
[sample2] RUNNING 	pid: 8979	uptime: 00:00:02
[sample3] RUNNING 	pid: 8980	uptime: 00:00:02
```


## gRPC
Pman server adopts [gRPC](https://grpc.io) as a protocol for IPC.  
You can call pman's API from client program written in programming language supporting gRPC.  
Please check [pman.proto](https://github.com/matsune/pman/blob/master/protos/pman.proto) file.
