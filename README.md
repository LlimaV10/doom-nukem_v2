# doom-nukem Readme

# Compilation:
make

# usage:
doom-nukem new|edit|game map_name

# 2D editor:
![image0](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/0.jpg)
Press:
 - arrows to moving on map
 - scroll to scale map
 - Tab to change drawing|selecting mode
 - Left click on dots to draw walls points (in drawing mode)
 - Right click on wall in closed sector to select wall (in selecting mode)
 - Shift to create portal (when 2 same walls in 2 different sectors selected)

# 3D editor:
![image3](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/3.jpg)
Press:
 - W, A, S, D - moving player
 - Arrows or mouse movement to rotate player
 - Tab to change mouse mode
 - Left mouse button on the wall to put selected texture on wall
 - Right mouse button on the portal to put selected texture as glass
 - '/' to put selected texture on floor
 - '.' to put selected texture on ceil
 - ',' to replace ceil with skybox
 - M to replace wall with skybox
 - P to place selected texture as picture on wall
 - V to change fly_mode (0 - no flymode, 1 - jetpack, 2 - noclip)
 - Numpad '+' and '-' to change lunar gravity on map
 - Numpad '*' to set selected texture as skybox
 - Numpad '/' to set selected texture as story image in the begining of the level
 - C to pick texture from wall

Hold:
 - [ when looking on picture on wall and use mouse movement to place it whatever you whant, scroll to change scale
 - ] when looking on sprite and use scroll to change scale, left mouse click to replace sprite with selected one
 
# Some special pictures to control light and create animation:
![image6](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/6.jpg)
 
# Creating new map
doom-nukem new map_name

Create a closed sector by clicking on the dots:
![image1](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/1.jpg)
Click on the robot icon (player) and put him inside of the sector. Then press play icon:
![image2](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/2.jpg)
You are now in 3d editor:
![image3](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/3.jpg)
Press Escape to go back in 2d editor. Create another sector with the same wall as the previous one.
Press Tab to change mode from drawing to selecting and right click on the wall. Then click on the ax to create portal
between this 2 sectors:
![image4](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/4.jpg)
Run the 3D mode again and change some floor|ceil height and incline. Textures on walls and floor|ceils.
Put some sprites, pictures, glass, animations...
Save map and enjoy!
![image5](https://raw.githubusercontent.com/LlimaV10/doom-nukem_v2/master/README_images/5.jpg)

 Use: "doom-nukem edit map_name" to edit map.
 
 # Game mode
 Use: "doom-nukem game map_name" to play map in game mode.
 
Use:
 - W, A, S, D to moving player
 - Arrows and mouse movement to rotate
 - F to interact with the panels and to lift items
 - Ctrl to squat
 - Shift to run faster (infinite player energy)
 - Left mouse click to attack
 - Tab to open inventory
 
	-- Click on picked up item to select it in inventory
 
	-- Click green check mark or press E on keyboard to use selected item (Medkits, ammunition)
 
	-- Click red cross or press Q to drop selected item
 
 - Caps Lock to open map (if you have a watch in inventory)
 - 1, 2, 3 to change weapons in hands

Visit https://github.com/LlimaV10/doom-nukem_binaries to play our game levels on any PC OS.

# Doom-nukem Wiki describing the main formulas and mathematical functions will be around April. Please Star this repository if you want it. To give me more motivation to do it.
