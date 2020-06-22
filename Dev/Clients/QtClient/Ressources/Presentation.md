# Shitty Wizard Installer

## What is this

This project is a clicker game.  
My main inspirations are ```Coockie Clicker``` and ```Spaceplan```.  
The theme of my game is a crazy bad software installer that requires you to ```"manually install"``` each bye.  
The end goal should be around 1 billion points. I aim for a game that can be won in arround 30minutes.  
The central feature of this project is the fact that the gameplay and the UI are separated across two process and they communicate through a Rest API.  
This setup is a bit cumbersome to work with but the end goal is to make this project ase a base for programation and optimization exercise.  
The idea came to me when I was playing Spaceplan and I was trying to determine what was the optimal build order to reach the end sequence.  
If I'm success full, someone with basic knowledge of programation should be able to make a small program that play the game automatically, trying to win as fast as possible (or in as few action as possible).  
The use of the Rest API makes it so it is possible to write such programe in most languages.  
When the game will be close to it's final iteration, I plan to make example of such programes in as many language as I can.  
This project, for now is fully opensource.  
The backend (core gameplay) is coded in C++ using RestCPP as a library for the Rest work.  
The front end (the User Interface) is coded in C++ using Qt.  
This is also a personal project to peak out of my Unity comfort zone.

## What have been done

* The base of the clicker mechanic
* The base of the upgrade system
* The game can be configured using a JSon
* The base of the Rest API and rest server shenanigan.
* The base of the Qt client (opening a window with a layout)
* The base of the client call to the gameplay.

## What is left to be done

* A proper balancing of the game
* Writing the intro textes.
* Storyboarding the intro animation.
* Implementing the storyboarded animation.
* Being able to launche the Server from the client.
* Binding the UI to the backend.
* Refreshing the UI more smartly than just displaying all the gameplay values (Hiding endgame upgrades etc).
* Storyboarding and implementing the main gameplay sequence.
* Storyboarding and implementing the end game sequence.
* Do pass of optimization and hunt for memory leaks.
* Writing down the whole API so it is possible to write middle men programs that complete the game.

## What I could do after

Ideally I would like this to be the base of some educational content. In the back of my mind I entertain the idea of searching for some gov funding so I could maintain this project and make it bloom.  
One cool side of the architecture is that it could run on a distant server really easely. therefore, it could be interesting to setup a server in such way that people could compete on how fast they can finish the game. I imagine leader board and all...  
If this work fine enough, It could be cool to make other game with the same ```middlemen``` approach (Tetris, 2048 etc...)

### Who am I  
I'm Anton. I'm a gameplay programmer in Paris that fancy himself as a potential indi dev.

#### [Full sources : github.com/Redoxee/AutoClicker](https://github.com/Redoxee/AutoClicker)
