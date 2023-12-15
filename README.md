# Final Year Project

![Preview image](preview.png)

## Table of Contents

- [About](#About)
- [Instructions](#Instructions)
  - [Linux](#Linux)
  - [Windows](#Windows)
- [Dependencies](#Dependencies)
- [References](#References)

### About

This project represents the culmination of all the programming work I have completed throughout my (ongoing) final year at university.

It is an extension of my 3DGP assignment from second year, and is expected to undergo changes and transformations in the upcoming months as more additions and modifications are implemented.

If you are assessing this project, please search for instances of `"@Assessor"` in the project's source code for notes and references. Please also refer to the [Dependencies](#Dependencies) and [References](#References) sections of this document for a comprehensive list of external sources.

### Instructions

#### Linux:

The SDL2 and GLEW development packages are necessary for this project. If they are not already installed on your system, you may need to install them:

##### Debian-Based

```sudo apt-get install libsdl2-dev libglew-dev```

#### Windows:

I have not yet tested building for Windows. SDL2 and GLEW are already included in the project's "external" folder and linked through CMakeLists.txt, but you may need to do additional setup. 

If you are able to successfully build for Windows, please let me know so that I can officially add build support for this platform!

### Dependencies

- [cereal v1.3.2](https://uscilab.github.io/cereal/)
- [OpenGL Extension Wrangler Library 2.1.0 (GLEW)](https://www.opengl.org/sdk/libs/GLEW/)
- [OpenGL Mathematics 0.9.9.8 (GLM)](https://www.opengl.org/sdk/libs/GLM/)
- [Dear ImGui v1.90 (IMGUI)](https://www.dearimgui.com/)
- [Simple Direct Media Layer v2.x (SDL2)](https://www.libsdl.org/index.php)
- [stb_image v2.27](https://github.com/nothings/stb/blob/master/stb_image.h)

### References

#### Code Sources

The following are a list of code sources I have used either directly or for reference and the files they appear in:

##### include/common_utils.glsl

 - Penov, F. and Slayden, G. (2016). image - Formula to determine perceived brightness of RGB color. [online] Stack Overflow. Available at: https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color [Accessed 15 Dec. 2023].


  - JeGX (2009). How to Visualize the Depth Buffer in GLSL | Geeks3D. [online] Geeks3D. Available at: https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/ [Accessed 15 Dec. 2023].

##### include/lighting_utils.glsl

- Myers, K., Fernando , R., Bavoil, L. and NVIDIA Corporation (2008). Integrating Realistic Soft Shadows into Your Game Engine. [online] Available at: https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf [Accessed 15 Dec. 2023].


- de Vries, J. (n.d.). LearnOpenGL - Parallax Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Parallax-Mapping [Accessed 15 Dec. 2023].


- Rabbid76 (2019). Adding Shadows to Parallax Occlusion Map. [online] Stack Overflow. Available at: https://stackoverflow.com/questions/55089830/adding-shadows-to-parallax-occlusion-map [Accessed 15 Dec. 2023].


- de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].


- de Vries, J. (n.d.). LearnOpenGL - Point Shadows. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows [Accessed 15 Dec. 2023].

##### include/rand.glsl

- Cerisano, D. (2022). Random / noise functions for GLSL. [online] Stack Overflow. Available at: https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl [Accessed 15 Dec. 2023].

##### bloom/downscale.glsl

- Jimenez, J. (n.d.). LearnOpenGL - Phys. Based Bloom. [online] learnopengl.com. Available at: https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom [Accessed 15 Dec. 2023].

##### render_passes/pass_normals.glsl

- de Vries, J. (n.d.). LearnOpenGL - Normal Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Normal-Mapping [Accessed 15 Dec. 2023].

##### aces.glsl

- Taylor, M. (2019). Tone Mapping. [online] 64.github.io. Available at: https://64.github.io/tonemapping/ [Accessed 15 Dec. 2023].

##### ao.glsl

- de Vries, J. (n.d.). LearnOpenGL - SSAO. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/SSAO [Accessed 15 Dec. 2023].

##### fxaa.glsl

- Flick, J. (2018). FXAA. [online] catlikecoding.com. Available at: https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/ [Accessed 15 Dec. 2023].

##### pbr.glsl

- Siejak, M. (2018). /PBR/data/shaders/glsl/pbr_fs.glsl. [online] GitHub. Available at: https://github.com/Nadrin/PBR/blob/master/data/shaders/glsl/pbr_fs.glsl [Accessed 15 Dec. 2023].

##### Camera.cpp

- de Vries, J. (n.d.). LearnOpenGL - Gamma Correction. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Gamma-Correction [Accessed 15 Dec. 2023].
- 
#### Academic Sources

These are the academic sources I used for reference while researching for this project:

- Fernando, R. and NVIDIA Corporation (2005). Percentage-Closer Soft Shadows. [online] Available at: https://developer.download.nvidia.com/shaderlibrary/docs/shadow_PCSS.pdf [Accessed 15 Dec. 2023].


- Chernikov, Y. (2021). Bloom. [online] www.youtube.com. Available at: https://www.youtube.com/watch?v=tI70-HIc5ro [Accessed 15 Dec. 2023].


- Opsenica, B. (2019). Automatic Exposure Using a Luminance Histogram. [online] bruop.github.io. Available at: https://bruop.github.io/exposure/ [Accessed 15 Dec. 2023].

#### Asset Sources