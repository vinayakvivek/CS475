# Toy story character rendering

## Compiling
```bash
	mkdir build
	cd build
	cmake ..
	make
```
- if build succeeds, an executable `toys` will be created in `build/`

## Key bindings
  [Key : function]
- `B`: selects Buzz model (selected by default)
- `H`: selects Hamm model
- If you have selected **Buzz**, parts of Buzz can be selected by the following: [key - part]
    - `0` - Hip
    - `1` - Torso
    - `2` - Neck
    - `3` - Left Upper Arm
    - `4` - Left Lower Arm
    - `5` - Left Hand
    - `6` - Right Upper Arm
    - `7` - Right Lower Arm
    - `8` - Right Hand
    - `SHIFT + 0` - Left Thigh
    - `SHIFT + 1` - Left Leg
    - `SHIFT + 2` - Right Thigh
    - `SHIFT + 3` - Right Leg
    - `SHIFT + 4` - Left Foot
    - `SHIFT + 5` - Right Foot
- If you have selected **Hamm**, parts of Buzz can be selected by the following: [key - part]
    - `0` - Torso
    - `1` - Head
    - `3` - Front Left Leg
    - `4` - Front Right Leg
    - `5` - Back Left Leg
    - `6` - Back Right Leg
