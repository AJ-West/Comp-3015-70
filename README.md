# Arrow dodge game for showing off lighting and texturing techniques

To compile the project it should just compile assuming you have opengl and Gld.c set up as suggested in the brief. However in testing sometimes I had to add glad.c again even though the file path hadn't changed but I am not sure why. If there is an issue with glad.c remove the current one from the project then add it back to the same place and it should work. I have this issue when first using the project template before changing anything and this was my fix.

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
<img width="586" height="302" alt="Screenshot 2026-03-02 211646" src="https://github.com/user-attachments/assets/e1d46e96-79db-4f06-9c95-4006202af791" />

### Basic texture sampling
Code:
<img width="318" height="56" alt="Screenshot 2026-03-02 211654" src="https://github.com/user-attachments/assets/98d60cc9-d637-452f-b01c-978c1c8fdca7" />

### Fog lighting
Code:
<img width="504" height="145" alt="Screenshot 2026-03-02 211705" src="https://github.com/user-attachments/assets/b1b8c371-366f-45d6-b1e6-936af95a1d5b" />

<img width="458" height="115" alt="Screenshot 2026-03-02 211713" src="https://github.com/user-attachments/assets/9c52c107-ef5c-4958-9fcb-fcf527ea4db5" />

### Toonshading
Code:
<img width="278" height="62" alt="Screenshot 2026-03-02 211728" src="https://github.com/user-attachments/assets/d47a64b1-7077-4de3-8eb3-abaaf786bbe8" />

<img width="465" height="20" alt="Screenshot 2026-03-02 211744" src="https://github.com/user-attachments/assets/bd8f31e7-b41d-42dd-a8c8-f283da1636b6" />

### Texture mixing
Code:
<img width="500" height="100" alt="Screenshot 2026-03-02 211755" src="https://github.com/user-attachments/assets/43ad707b-26e7-4838-8bcb-b59135e189ce" />

### Normalmap
Code - mixed normals:
<img width="508" height="113" alt="Screenshot 2026-03-02 211800" src="https://github.com/user-attachments/assets/11144c3b-a78b-4ee6-bef4-f5591919b55a" />

Code - single normal map:
<img width="353" height="84" alt="Screenshot 2026-03-02 211804" src="https://github.com/user-attachments/assets/24797758-49a6-459f-ad9e-1da38acf872a" />

In scene without toonshading as wasn't clear in video:
<img width="523" height="492" alt="Screenshot 2026-03-02 211341" src="https://github.com/user-attachments/assets/7fe02773-7b87-4972-9338-66825f5ac066" />

### Skybox
Code skybox.vert:
<img width="406" height="267" alt="Screenshot 2026-03-02 211813" src="https://github.com/user-attachments/assets/2009833d-8b21-41c1-8ac3-533e2929d6fc" />

Code skybox.frag:
<img width="764" height="187" alt="Screenshot 2026-03-02 211818" src="https://github.com/user-attachments/assets/f7dc6aed-67b9-4ed3-8f16-7bce174b91b7" />

### HDR
Code - pass 1:
<img width="664" height="429" alt="Screenshot 2026-03-02 211830" src="https://github.com/user-attachments/assets/06994e39-48ca-49bb-919e-f76eac15a27d" />

Code - pass 2:
<img width="513" height="506" alt="Screenshot 2026-03-02 211835" src="https://github.com/user-attachments/assets/71df7075-5d57-4e76-85ca-c761d220c640" />

Code - main:
<img width="206" height="126" alt="Screenshot 2026-03-02 211838" src="https://github.com/user-attachments/assets/3d97835a-c950-4e4f-85a0-a541f616fba8" />

##Some Development progress images
###Torus iwth blingphong, toonshading and multiple moving lights
<img width="595" height="441" alt="Screenshot 2026-02-21 125126" src="https://github.com/user-attachments/assets/e1a0b66b-769e-4c5a-9cbb-863d69ca9af3" />

### All features other than HDR implemented on lab models
<img width="755" height="489" alt="Screenshot 2026-02-24 131404" src="https://github.com/user-attachments/assets/2cc2f632-27f6-4eb9-97ff-9ba01aa55166" />

