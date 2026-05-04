# Arrow dodge game for showing off lighting and texturing techniques

To compile the project it should just compile assuming you have opengl and gald.c set up as suggested in the brief. However in testing sometimes I had to add glad.c again even though the file path hadn't changed but I am not sure why. If there is an issue with glad.c remove the current one from the project then add it back to the same place and it should work. I have this issue when first using the project template before changing anything and this was my fix.

## Project info

### To run (this section is only on github not readme in submission as this was added just in case after my submission but before submission deadline)
To run the executable go into the executable folder in the extracted zip folder. Then in the debug folder is "Project_Template.exe." double click on this to run the program. Pause to explore the scene with "p" else avoid the arrows as getting hit closes the program.

### Github:
[https://github.com/AJ-West/Comp-3015-30-](https://github.com/AJ-West/Comp-3015-70)

### Video:


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

particles.vert/.frag
Shaders for the particles. Outputs to HDRColour so that particles can be used with HDR. Creates risk of outputing to the wrong location but is seperated to make code more readable. Must ensure when developing if HDR location changes to update the location in both basic_uniform and particles shaders.

## Technique screenshots from shaders

### Surface animation
Surface animation works by using the current time passed to update the vertex positions using the below calculations. The normal vector is then also calculated. The blingphong method used for flags is different to the ones used for crossbows as the flags do not have calculated tangents used for normal mapping like the crossbows do.
float wave = (dot(pos.xy, vec2(0.5)) + Time*2.0) ; //works out the vectors points based off the time passed by updating the wave
pos.y = Amp*sin(wave) * (pos.x+8) * 0.5 + 0.5; // uses the calculated wave point then amplifies it by the amplitude and how far awar from the flags origin it is. Distance from flags origin increases amplitude to simulate the flag being attached to a pole so has more freedom to move the further from the pole it is. With more time I would like to of added more complex and realistic calculations to create a better flag waving effect.

Vert code:
<img width="351" height="185" alt="Screenshot 2026-05-04 150425" src="https://github.com/user-attachments/assets/e5af7aa9-21d2-452f-807f-8c1340800fb1" />

Frag code:
<img width="419" height="134" alt="Screenshot 2026-05-04 150416" src="https://github.com/user-attachments/assets/65fb85c2-5353-4246-ab28-7a30b10280d0" />

In game:
Front
<img width="692" height="380" alt="Screenshot 2026-05-04 150947" src="https://github.com/user-attachments/assets/50ffdd86-6cd0-41f8-8000-7956b8da5507" />

From above
<img width="381" height="596" alt="Screenshot 2026-05-04 151001" src="https://github.com/user-attachments/assets/5b22b878-51e6-4a15-845d-225c34373013" />

### Particle fountain/smoke particles
Upon a crossbow firing an arrow a particle smoke effect is spawned. This effect creates a little ball of smoke around the point the arrow was spawned. This helps hide the arrow being spawned out of nowhere. The particles require 2 passes but don't use a framebuffer for the passes. The first pass updates the particles positions and lifetime. The second pass renders the particle passing the output to HDR colour so that is can be used with HDR.

Vert code:
<img width="631" height="672" alt="Screenshot 2026-05-04 150456" src="https://github.com/user-attachments/assets/ec85e52b-eebe-4d23-83bf-e688a6212f75" />
<img width="591" height="476" alt="Screenshot 2026-05-04 150504" src="https://github.com/user-attachments/assets/0be26f1d-1509-4faf-863b-59003e79f2e1" />

Frag code:
<img width="315" height="188" alt="Screenshot 2026-05-04 150511" src="https://github.com/user-attachments/assets/a0b01605-120e-471a-9fd3-7763eb5f012a" />

In game:
<img width="212" height="185" alt="Screenshot 2026-05-04 150935" src="https://github.com/user-attachments/assets/6906f034-a56a-46f6-9ded-6fb5883daca0" />


Smoke texture is the texture given in the lab session for smoke particles.

### Cloud effect
Move across quad and is procedurally generated during runtime creating a natural looking cloud effect. Cloud opacity is calculated then is blended with the skybox texture for that point to determine what the render should look like. The distance from the camera to the current position of the rendered object is calculated. Then using that the further away it is the lower it's opacity becomes this is to gradually fade in/out clouds as they approach the edges of the quad. Without this it looks like clouds are suddenly disapearing in the sky while a gradule effect makes it more realistic.

Vert code:
N/A

Frag code:
<img width="395" height="457" alt="Screenshot 2026-05-04 150409" src="https://github.com/user-attachments/assets/aba50b04-bfdf-4549-9749-8bbb3c398728" />

In Game:
<img width="764" height="574" alt="Screenshot 2026-05-04 151020" src="https://github.com/user-attachments/assets/784838cf-51ac-4d62-bc53-097bc54908f2" />

## Idea
The idea for the game originally was a parkore course. The idea was to include shooting objects which I began on first. Upon making the crossbows I decided to adjust for making it a dodging game. A big factor for the reduce in scope was to avoid time being spent on building a physics system for the characters movement. This is what was submitted for the 30% coursework. There wasn't much that made it a game. Environment was (and still is slightly) bland with no sort of game loop as it just closed the game upon losing. To improve on this a bit of themeing was added meant to be at a training exercise at camp. Cloud effects were adding to show time passing. ImGUI was used to add a timer and record timer as well as a home screen. Animated flags were added to improve themeing. Particles were added on crossbow shots to give them more of an impact when fired as well as crossbows direction changing with time to avoid spots the player could stand to indefinitely avoid being hit.

## What was started with
The 30% coursework can be found here to see what the base scene was compared to the current scene.
Github:
Video:

## What makes it unique
What makes the project unique is how the components are put together. The game loop is not particularly original. The cloud effect uses the skybox texture to blend in better making it more difficult to see the outline of the quad used to display it. Changes made from the 30% (excluding the new features) includes reworking the skybox shader to be in the main shader, fix issues with not rendering the project on nvidia GPUs with regards to HDR (HDR is still used), added a home screen, added a timer and best score, added music and sound effects using Irrklang. The only sound effect is a death sound effect. A sound effect for crossbow fire was tested but crossbows are fired so frequently is became annoying and made the game worse. 


