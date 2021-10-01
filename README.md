# physics-sim-22
Physics Simulator 2022

## Opening This Code With VS
To open this code for editing and compiling, first open Visual Studio and clone the repo. 
In your Solution Explorer, you should see a file titled "physics-sim-22.sln" which is the solution file for this project. 
Double click this to open the solution, and then click the button that looks like 3 files, to show all files. (you may have to show all files within projects as well)
You can now open and edit any of these files, and select your build type (debug, release, etc) and your architecture.

## Contributing to This Project With VS
For each individual component that you wish to add, click File > New > Project... and in the dialog box select Empty Project.
On the next screen you will be given options to name the project.  Name it something appropriate for a directory (I.E. vector for the vector component) 
and make sure under "Solution" you select the option "Add to solution". This should automatically select physics-sim-22.
Click create to add your project, then you can create files by right clicking, selecting Add > New Item and choosing the appropriate format.

## Configuring and Compiling
So in general, all of our projects except the one that contains our main() and runs the whole program will be configured as Static Libraries. 
To do this, right click on the project and select "Properties". Make sure at the top of the window you have "All Configurations" and "All Platforms" selected. 
Then under Configuration Properties > General, change the Configuration Type to Static library (.lib)

In addition to this, in the Properties of the project containing main(), you'll need to add the project directory to the "Additional Include Directories" under C/C++ > General.
You can add this in the format $(SolutionDir)name_of_project_folder.
This will allow you to simply #include "project_name" in the main file instead of doing some shenanigans with including the directory in the include statement.
this *should* compile, but it gets funky sometimes, so let me know if you're having issues and we'll work it out. I was having some weird issues and had to register some stuff.