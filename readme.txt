/* Start Header -------------------------------------------------------
File Name: readme.txt
Purpose: readme file
Language: C++
Platform: visual studio 2019, windows 10
Project: william.patrick_CS350_3
Author: William Patrick
End Header --------------------------------------------------------*/

Interface: the only interface currently is the GUI in the program.
- Camera controls (wasd + left_ctrl + space for movement along axes, arrow keys + right_ctrl + np_0 for rotations)
- Imgui buttons to swap between looking at the file object to inspect individual BV results
- Imgui buttons to swap to viewing power plant files to inspect BV heirarchies

How to start: An executable can be found in the 'Executable' Folder, Able to rebuild through Visual Studio project

Machines tested on: remote (Windows 10 Home, AMD Radeon RX 6600, Radeon Graphics Drivers AMD Software: Adrenalin Edition 22.11.2 11-30-2022)

Other info: 
- This is a compilation project of 5 projects, each were done under time pressure of classes.
- 4sphere model takes some time to load.
- Power plant is white due to poor handling of light vectors in shader
- BV heirarchy occasionally flickers with sphere tree when changed
- BV heirarchy displays ROYGBIV for depths, Red for the root

Possible improvements:
- Cleanup the scene file
- Remove copy-paste code for all rendered objects, separate into classes
- Light vectors are broken at the moment
- Spotlight performs strangely when there are multiple spotlights
- BVH bottom up construction does not appear to limit itself at 7 layers
- objects that span multiple (3 or more) octree spaces get cut off.