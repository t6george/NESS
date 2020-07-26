<p align="center">
  <img width="120" height="80" src="https://user-images.githubusercontent.com/31244240/72781082-763a2a00-3bee-11ea-902f-9963f2a764fb.png">
</p>

# NES++ - NES Emulator + Automated Speedrunner

For the past couple of months, I have been working on a really exciting project. Although the [Ninendo Entertainment System](https://en.wikipedia.org/wiki/Nintendo_Entertainment_System) was 
released in North America 14 years before I was born, I still enjoyed the multitude of games that I played on various NES emulators. It was about time I wrote my own!

## Usage and Gameplay

| [![](https://user-images.githubusercontent.com/31244240/72782990-3f1a4780-3bf3-11ea-9ca1-6cca205c2a43.gif)](https://en.wikipedia.org/wiki/Super_Mario_Bros.)  | [![](https://user-images.githubusercontent.com/31244240/72783129-adf7a080-3bf3-11ea-8ed0-6ec779172a35.gif)](https://en.wikipedia.org/wiki/Ice_Climber) |
|:---:|:---:|
| [Super Mario Bros.](https://en.wikipedia.org/wiki/Super_Mario_Bros.) | [Ice Climber](https://en.wikipedia.org/wiki/Ice_Climber) |
| [![](https://user-images.githubusercontent.com/31244240/72783177-c8ca1500-3bf3-11ea-9a86-0d6a557b04e9.gif)](https://en.wikipedia.org/wiki/Donkey_Kong_3)  | [![](https://user-images.githubusercontent.com/31244240/72783600-d59b3880-3bf4-11ea-9cbc-721b2ea76ee5.gif)](https://en.wikipedia.org/wiki/Excitebike) |
| [Donkey Kong 3](https://en.wikipedia.org/wiki/Donkey_Kong_3) | [Excitebike](https://en.wikipedia.org/wiki/Excitebike) |

To build this project, just run
```make -j200```
(just in case you are on a server). This will create a ```nes``` executable.

Due to the temporary lack of a GUI File System, you will have to pass the parameters via the command line.
* To simply play a game:
``` ./ness play <path_to_binary_game_file>```
* To create a tool-assisted speedrun:
``` ./ness record <path_to_binary_game_file> <path_to_tas_file>```
* To view your tool-assisted speedrun:
``` ./ness replay <path_to_binary_game_file> <path_to_tas_file>```

The in-game controls are:
```
Keyboard A - (A)
Keyboard S - (B)
Keyboard Z - (Start)
Keyboard X - (Select)
Arrow Keys - (D-Pad)
```
## The Name
I didn't want this to just be another NES emulator ;) so I just titled it after my favorite programming language (and the one I used to build this project), C++. Also, NES++ more than just an emulator - it is a competitive gaming environment for retro enthusiasts!

My journey in emulation began in Summer 2018, when I built a [Chip8 Interpreter](https://github.com/t6george/Chip8Emulator), but my interests in retro tech began in my pre-teen years. At this point, I may as well get started on a Super Nintendo Emulator. However, the 8-bit NES proved to be quite a challenging project - 
I can only imagine the hardware quirks, volume of instructions, and graphical rendering that goes behind the 16-bit SNES. Before I embark on
this new adventure, there are a couple of aspects of NESS that I would like to improve:
* Code refactor - in a rush to complete NESS before the school term, there are some aspects of the code which could be encapsulated more effectively.
* Improved GUI - while I like what I have done so far, I want to add a file browser system and display some diagnostics such as the framerate.

Currently, I have put this project on hold to focus on the school term, but I look forward to resuming development after finals!

## Tool-Assisted Speedrunning
Speedrunning is a "sport" where gamers around the world compete to beat a game (from it's start screen to the end credits) in the minimal amount of time. Check out [this](https://www.youtube.com/watch?v=4CgC2g43smA) incredible video to witness the extents that hardcore speedrunners reach to shave off milliseconds from the world records.

The premise behind Tool-Assisted Speedrunning is that you can use software to create a precise sequence of controller inputs which will allow
you to consistently exploit in-game glitches. While humans can become very good at speedrunning, there are usually instances
where the input into the game must be so precise, that only a Tool-Assisted Speedrunner could achieve it at a reasonable success
rate.

At the bottom of the screen, the NES Joypad's button presses are being indicated in real-time. This is to help with the tool-assisted speedrunning experience. By screen-recording a speedrun recorded on this emulator, you can observe the frame-by-frame inputs that the author is making. Hopefully, this can make speedrunning more accessible to newcomers who have not developed 60Hz reflexes yet.

## Credits
* [NesDev Wiki](https://nesdev.com/) - the NES development community is unparalleled. Thanks to all of the clever researchers and reverse-engineers
who made this possible!
* [LaiNES](https://github.com/AndreaOrru/LaiNES) and [ANESE](https://github.com/daniel5151/ANESE) - for much-needed inspiration and guidance in solving
some of the more intricate bugs
* My friends and family - they never ceased to believe in and support me, even during that 8 hour long bug (which ended up being an incorrect
offset into the cartridge memory XD)
