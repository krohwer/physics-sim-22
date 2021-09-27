# physics-sim-22
Physics Simulator 2022

## Compiling the Program
Once we have executable code, we will want to convert our primary .py file into a .exe executable so that we can run it
without requiring the user to install Python and do some gymnastics to be able to run it.

First, navigate to the folder that contains your .py file and install pyinstaller, the module we will be using to 
perform this conversion:

```pip install pyinstaller```

Next, within the same folder, run the command:

```pyinstaller -D filename.py```

This will create the dist folder, which will contain all the files necessary as well as a .exe to run the program. The 
contents of this folder is what will be distributed to the users.

NOTE: Further research will be done on protecting the source code and improving efficiency using this method. Until 
then, more information about pyinstaller can be found here: https://pyinstaller.readthedocs.io/en/stable/index.html