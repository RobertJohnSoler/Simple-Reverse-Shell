# Simple-Reverse-Shell

## Overview
This repo contains code for a simple reverse shell malware written in C. Anything in this repo is strictly for educational purposes only. <strong> Using this code for illegal purposes is punishable by law. </strong>

## How it works
1. Make sure your attacking machine and victim machine are able to communicate with each other via sockets (see [Prerequisites and Preliminaries](#section-1)).
2. Choose the reverse shell you want to use, change its `server_ip` to the IP address of your attacking machine, compile it, and have your victim machine download and run it.
3. Run the Python server code.
4. See the magic happen.

## Reverse Shell Kinds
If you check the files in this repo, you will notice that there are actually two reverse shell codes present:
* `rev_shell.c` - A very basic proof of concept for a reverse shell. When run on the victim machine, your attacking machine will have remote access to its files. However, you still need to run this on the command line.
* `actual_rev_shell.c` - The closest I could get to an actual reverse shell malware. It's stealthier and more optimized. Instead of running it on the command line, you simply open it from file explorer, and it will start running in the background without telling the user anything.

## Important Commands
* To compile `rev_shell.c`: `g++ rev_shell.c -o <output_filename>.exe -lws2_32`
* To compile `actual_rev_shell.c` : `g++ actual_rev_shell.c -o <output_filename>.exe -lws2_32 -mwindows`

<br/>

# <a name="section-1"></a> Prerequsites and Preliminaries
1. Make sure Python 3 is installed in your machine.
2. Make sure the environment for C/C++ development is already set up on your machine. The C/C++ code in this repo is build for Windows machines, so if that's the OS you are using, follow the guide [here](https://code.visualstudio.com/docs/cpp/config-mingw?fbclid=IwY2xjawG6AfdleHRuA2FlbQIxMAABHSO4WPA2xtDaTKrFsBsA-wPPEC2UcH2cfyFbi2WN0b8scKeCweYNZqBKvw_aem_VmqdFcg02qeJubMOo6dONQ).
3. Open a terminal and run the Python server. You should see this on your terminal:
```
PS C:\Path\To\Simple-Reverse-Shell> python server.py

Socket created.
Socket binded to 8080

Socket is listening...
```
4. Then, on a separate terminal, compile `socket_client.c` using the command `g++ socket_client.c -o sc.exe -lws2_32` and run it. Your second terminal should look like this:
```
PS C:\Path\To\Simple-Reverse-Shell> g++ socket_client.c -o sc.exe -lws2_32
PS C:\Path\To\Simple-Reverse-Shell> ./sc.exe
Sending hello message from the C client to the server... 
Hello message sent
Client socket closed.
```
After running that code, go back to the terminal running your Python server. It should now looke like this:
```
PS C:\Path\To\Simple-Reverse-Shell> python server.py

Socket created.
Socket binded to 8080

Socket is listening...
Got connection from reverse shell in  ('127.0.0.1', 54203)
(Reverse Shell) Hello from my C client!
>
```
5. Press Ctrl+C to terminate your Python server.

Of course, the examples shown above are only if you are running both the socket client and socket server in the same machine, hence the IP 127.0.0.1. If you want to try socket communication over two different machines, (like running the server in one laptop and running the client in another), just change the `server_ip` variable in `socket_client.c` to the IP address of the computer you are hosting your sever in.
```
const char* server_ip = "127.0.0.1"; // change this to your server machine's IP address
```
If you get the results shown above, then your machines are able to comminicate with each other via sockets. Now you may start playing with the reverse shell codes.