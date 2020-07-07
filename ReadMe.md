# Shitty Wizard Installer

## What is this

This project is a clicker game.  
My main inspirations are ```Coockie Clicker``` and ```Spaceplan```.  
The theme is a crazy bad software installer that requires you to ```"manually install"``` the program.  
The end goal is 1 billion points (named bytes). The first playthrough is around 2 hours.  
The central feature of this project is the fact that the gameplay and the UI are separated across two process that communicate through a Rest API.  
This setup is a bit cumbersome to work with but the end goal is to make this project ase a base for programation and optimization exercises.  
The idea came to me when I was playing Spaceplan and I was trying to determine what was the optimal build order to reach the end sequence.  
If I'm successfull, someone with basic knowledge of programation should be able to make a small program that play the game automatically, trying to win as fast as possible (or in as few action as possible).  
The use of the Rest API makes it possible to write such programe in most languages without too much hassle.  
The backend (core gameplay) is coded in C++ using RestCPP as a library for the Rest work.  
The front end (the User Interface) is coded in C++ using Qt.  
This is also a personal project to peak out of my Unity comfort zone.

## What is left to be done

* Rebalance the game.
* Do pass of optimization and hunt for memory leaks.
* Writing examples of middlemen clients.

## What I could do after

Ideally I would like this to be the base of some educational content. In the back of my mind I entertain the idea of searching for some gov funding so I could maintain this project and make it bloom.  
One cool side of the architecture is that it could run on a distant server really easely. Therefore, it could be interesting to setup a server in such way that people could competes on how fast they can finish the game. I imagine leader board and all...  
If this work fine enough, It could be cool to make other game with the same ```middlemen``` approach (Tetris, 2048 etc...)

### Who am I  
I'm Anton. I'm a gameplay programmer in Paris that fancy himself as a potential indi dev.

#### [Full sources : github.com/Redoxee/AutoClicker](https://github.com/Redoxee/AutoClicker)
