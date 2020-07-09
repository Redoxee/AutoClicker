# Disclaimer

## This is not real

The Shitty Wizard Installer is the creation of AntonMakesGames inc.
By continuing you are not agreeing with a license agreement.
If you are still reading this, you are aknowledging that the team responsible for this document is not aware of the efficacity nor the utility of such document.
The AntonMakesGames (there after AMG) team do not believe this wizard can harm you or your computer, provided you got it from an official source.
Some wizard could harm you. This one will not.  
This wizard is not real. To the extent that some wizards may be real.
The Shitty Wizartd Installer (there after SW Installer) will not install anything on your computer, it will only pretend to do so.  
Do not panic, and do not close the black window that just opened. If you do so, you should take full resposability for SW Installer not behaving properly.
You should expect something that looks like an installation but really isn't.

When the installation is finished, or if you find bugs, you can send me an email at  
AntonMakesGames@gmail.com  
consider including the GameLog.txt as it will help me improve the SW Installer.

If this is your first time installing I expect you not to read past this point.  
Please, stop.

## In depth liability

This project is a clicker.  
the main inspirations are ```Coockie Clicker``` and ```Spaceplan```.  
The theme is a bad software installer that requires you to ```"manually install"``` each bytes.  
AMG expect you to complete the installation in under three hours.  
Try to optimize your activity count, try to minimize your interaction. This could increase your final score.
Please note that this is a two process installation setup, meaning that this installer is actually two programs executing concurently and communicating through a REST api.
This setup is a bit cumbersome, sure, but think about this: what if you didn't install the thing? What if instead, you wrote a program to do it in your place.
The SW Installer won't hold you back (It can't).
The use of the Rest API makes it possible for you to write a middle men software.  

Send your requests to  
[http://localhost:1234?[command]](http://localhost:1234?[command])  

commands :  

* click
* buy = { *index of the upgrade you whant to buy* }  
* get_state  ( *get the state of the installation as a json* )
* set_frame_length = { *time between each automatic installations* }
* set_update_pause = true/false ( *no more automatic installations* )
* set_terminated ( *please don't* )

Sources and examples of automatic software for the SW Installer can be found somewere around here :  
[sources : github.com/Redoxee/AutoClicker](https://github.com/Redoxee/AutoClicker)
