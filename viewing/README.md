## Graphics - Viewing pipeline simulation

### Compiling and Running
-	create a directory named `models` and save all the `.raw` files in that.
- 	create a scene file named `myscene.scn`.
- 	run `make` to compile the code.
- 	run `./assign2` to run the simulation.

### Usage
	[Key : function]
-	`0` : move to WCS (world coordinate system)
-	`1` : move to VCS (view coordinate system)
- 	`2` : move to CCS (clip coordinate system)
- 	`3` : move to NDCS (normalized device coordinate system)
-	`4` : move to DCS (device coordinate system)

- 	`C` : clip the scene
-	`V` : unclip (normal scene without clipping)

-	`Z` : zoom in (keep pressing to zoom in continuously)
-	`X` : zoom out

-	`UP`, `DOWN` : rotate about Y-axis
-	`LEFT`, `RIGHT` : rotate about X-axis
-	`PAGE UP`, `PAGE DOWN` : rotate about Z-axis

### Screenshots

-	World Coordinate System (WCS)  
	<img src="screenshots/WCS-1.png" width="400">
	<img src="screenshots/WCS-2.png" width="400">
	<img src="screenshots/WCS-3.png" width="400">
	<img src="screenshots/WCS-3-clipped.png" width="400">
	
- 	View Coordinate System (VCS)  
	<img src="screenshots/VCS-1.png" width="400">
	<img src="screenshots/VCS-2.png" width="400">
	
-	Clip Coordinate System (CCS)  
	<img src="screenshots/CCS-1.png" width="400">
	<img src="screenshots/CCS-1-clipped.png" width="400">
	<img src="screenshots/CCS-2.png" width="400">
	
-	Normalized Device Coordinate System (NDCS)  
	<img src="screenshots/NDCS-1.png" width="400">
	<img src="screenshots/NDCS-2.png" width="400">
	
- 	Device Coordinate System (DCS)  
	<img src="screenshots/DCS-1.png" width="400">
	<img src="screenshots/DCS-1-clipped.png" width="400">
	<img src="screenshots/DCS-2.png" width="400">
	<img src="screenshots/DCS-2-clipped.png" width="400">
