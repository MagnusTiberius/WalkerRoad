# WalkerRoad
A Client-Server Chat/Game Environment/Protocol.

Running the Demo:

1) Build the three executables. pinefarm_gui_c.exe, walkerroad_c and walkerroad_d.

2) Run the server first. i.e. walkerroad_d.

3) Run the client(s) after that. i.e. walkerroad_c and pinefarm_gui_c.exe.


The three programs are set up to run in one machine. The client(s) will connect to the server right away and run ad infinitum.

The demo is just used for that, demonstrating the concept. You can add handlers to both the Server and Client piece to define your own simulation.

For example, you can roll your own protocol using structured data, making your simulation of test cases very specific.

Continuous Integration:

https://ci.appveyor.com/project/MagnusTiberius/walkerroad

News:

2016 Feb 13 - Server (pinefarm_d) milestone reached. Chat engine can echo comments, while game engine can broadcast state of game object.
