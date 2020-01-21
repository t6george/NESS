# NESS - NES Simulator
A cycle-accurate Nintendo Entertainment System Emulator and Tool-Assisted Speedrun Creator

For the past couple of months, I have been working on a really exciting project. Although the Ninendo Entertainment System was 
released in North America 14 years before I was born, I still enjoyed many of it's titles on NES emulators. Now, it is my turn
to write my own!

##The Name
I didn't want this to just be another NES emulator ;), so I called it a "simulator". More conveniently this spells out NESS, which
is the main character in my favorite RPG: Earthbound. Earthbound is in fact for the Super Nintendo Entertainment System, which is my favorite console
of all time, due to its vast library of genuinely awesome games.

That being said, I should probably get started on a Super Nintendo Emulator. However, the 8-bit NES proved to be quite a challenging project - 
I can only imagine the hardware quirks, volume of instructions, and graphical rendering that goes behind the 16-bit SNES. Before I move on to
this new adventure, there are a couple of aspects of NESS that I would like to improve:
* Code refactor - in a rush to complete NESS before the school term, there are some aspects of the code which could be encapsulated more effectively.
* Improved GUI - while I like what I have done so far, I want to add a file browser system and display some diagnostics such as the framerate.
Currently, I have put this project on hold to focus on the school term, but I look forward to resuming after finals!

##Gameplay
The gameplay controls are as follows:
Keyboard A - (A)
Keyboard S - (B)
Keyboard Z - (Start)
Keyboard X - (Select)
Arrow Keys - (D-Pad)

##Tool-Assisted Speedrunning
Speedrunning is a "sport" where gamers around the world compete to beat a game (from it's start screen to the end credits) in the 
minimal amount of time. Check out [this](https://www.youtube.com/watch?v=4CgC2g43smA) incredible video for the extents that hardcore speedrunners reach to shave off milliseconds
from the world records.

The premise behind Tool-Assisted Speedrunning is that you can use software to create a precise sequence of controller inputs which will allow
you to consistently exploit in-game glitches. While humans can become very good at speedrunning, there are usually instances
where the input into the game must be so precise, that only a Tool-Assisted Speedrunner could achieve it at a reasonable success
rate.

At the bottom of the screen, the NES Joypad's button presses are being indicated in real-time. This is to help with the tool-assisted
speedrunning experience. By screen-recording a speedrun recorded on this emulator, you can observe the frame-by-frame inputs that
the author is making.

##Credits
* [NesDev Wiki](https://nesdev.com/) - the NES development community is unparalleled. Thanks to all of the clever researchers and reverse-engineers
who made this possible!
* [LaiNES](https://github.com/AndreaOrru/LaiNES) and [ANESE](https://github.com/daniel5151/ANESE) - for much-needed inspiration and guidance in solving
some of the more intricate bugs
* My friends and family - they never ceased to believe in and supported me, even during that 8-hour long bug (which ended up being an incorrect
offset into the cartridge memory XD)
