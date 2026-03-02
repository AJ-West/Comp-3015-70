# Arrow dodge game for showing off lighting and texturing techniques

## Project info

### Github:
https://github.com/AJ-West/Comp-3015-30-

### Video:
https://www.youtube.com/watch?v=f5KkX3ISI_E

### Visual studio version: 
Microsoft Visual Studio Community 2022 (64-bit) - Current
Version 17.8.3

### Operating system
Windows 11
Version 25H2 (OS Build 26200.7840)

## How the project works
I am not covering all files used in the project but rather just the main files that handle the bulk of the program.


Scenerunner.h:
When created it makes the window and callback for input handling using GLFW. During runtime it handles running the main loop and taking the user input passing the input onto where it is required.

scenebasic_uniform.h/.cpp:
Handles the main scene most importantly the objects within the scene. This is where you set up the shaders for rendering as well as the variables the shaders require e.g. light position and colours. Updating the objects during runtime is also done here in the update function with the update camera function only being used when called from scenerunner after the user has inputted a movement key or the p key (movement: WASD, pause: P).

As well as the loading the shaders with the correct variables it also handles the passes of the basic_uniform shader. This shader uses the HDR lighting technique meaning it requires 2 passes. Pass 1 handles the lights of the scene and model textures creating what would be rendered. This is then projected onto a 2d plane covering the viewport in pass 2. In pass 2 of the shaders is when the luminance of the scene is rendered after being calculated between the passes.

Camera.h:
This is used for updating the viewport. While the actual view is set in scenebasic_uniform the camera position, up and forward are calculated here and passed to set the view port. They are updated here after a user input of a movement function. When not paused it also clamps the camera to within the game area.

global.h/.cpp:
This is used to store variable that are used throughout such as the camera. It also stores thye struct for arrows with their update logic and the list of the arrows used in object pooling. The arrows are set to in use/not in use when required instead of spawning/despawning new arrows each time.

crossbow.h:
Handles the logic of the crossbows including their rotation, direction and spawning of arrows. These functions are called from scenebasic_uniform to update the relevant crossbow.

basic_uniform.vert/.frag:
Shaders for models in the scene. Includes BlingPhong, Basic texture sampling, mixed texture sampling, fog lighting, toonshading, normalmap and HDR.

skybox.vert/.frag
Shaders for the skybox. Outputs to HDRColour so that the skybox can be used with HDR.

## Technique screenshots from shader

### BlingPhong
Code:


### Basic texture sampling
Code:


### Fog lighting
Code:


### Toonshading
Code:


### Texture mixing
Code:


### Normalmap
Code:


In scene without toonshading as wasn't clear in video:


### Skybox
Code:


### HDR
Code:

