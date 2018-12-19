Prerequisite installation:

git:    if on windows, download cmder and choose "full" with git for windows: 
        http://cmder.net/

        otherwise:
        https://git-scm.com/downloads

node: https://nodejs.org/en/download/

npm: (should have downloaded with node, if not: https://www.npmjs.com/get-npm)



To run the program:

double click startDigipotProgramming.bat

changing the numbers will automatically set and save those values to the level, you can press tab or enter to go to the next level

you can find current and legacy values for units in the server/app/local folder

you can find and manually change the default values for each channel in the server/app/local folder

you must select the unit number before you can select a channel

make sure the digipot cables are connected to the channel before you select the channel

make sure you switch the digipot cables to a new channel before switching channels (if you forget to, you can refresh data)

if you are seeing weird numbers, check to make sure everything is connected, try reconnecting to the digipot (with the button)

right click startDigipotProgramming.bat and select Create Shortcut, move the shortcut to the desktop to start the program from the desktop (don't move any of the files around)



To find and/or manually change the default values for each channel as well as the log of previous system's final values:

go to: server/app/local



To set up on a new computer:

copy the setupDigipotProgramming.bat file onto the new computer using a flash drive and double click

copy the "local" folder found in server/app directory to the same place on the new computer (server/app directory)

you can delete the setupDigipotProgramming.bat file from outside of the digipotValueSetter folder 
(leave the one inside just in case to copy to a new computer)



To update the program (only needed if there are changes made to the program):

double click update.bat