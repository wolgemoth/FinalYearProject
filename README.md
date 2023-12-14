# Final Year Project

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

### Instructions

#### Linux:

The SDL2 and GLEW development packages are necessary for this project. If they are not already installed on your system, you may need to install them:

##### Debian-Based

```sudo apt-get install libsdl2-dev libglew-dev```

#### Windows:

I have not yet tested building for Windows. SDL2 and GLEW are already included in the project's "external" folder and linked through CMakeLists.txt, but you may need to do additional setup. 

If you are able to successfully build for Windows, please let me know so that I can officially add build support for this platform.

### Dependencies

- [cereal v1.3.2](https://uscilab.github.io/cereal/)
- [OpenGL Extension Wrangler Library 2.1.0 (GLEW)](https://www.opengl.org/sdk/libs/GLEW/)
- [OpenGL Mathematics 0.9.9.8 (GLM)](https://www.opengl.org/sdk/libs/GLM/)
- [Dear ImGui v1.90 (IMGUI)](https://www.dearimgui.com/)
- [Simple Direct Media Layer v2.x (SDL2)](https://www.libsdl.org/index.php)
- [stb_image v2.27](https://github.com/nothings/stb/blob/master/stb_image.h)

### References

#### Code Sources

The following are a list of code sources I have used (with or without adaption) and the files they appear in:

##### include/common_utils.glsl

 - Convert RGB to Luminosity - https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

##### include/lighting_utils.glsl

- NVIDIA PCSS - https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf


- Learn OpenGL Parallax Mapping - https://learnopengl.com/Advanced-Lighting/Parallax-Mapping


- Parallax Occlusion Shadows - https://stackoverflow.com/questions/55089830/adding-shadows-to-parallax-occlusion-map


- Learn OpenGL Shadow Mapping - https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping


- Learn OpenGL Point Shadows - https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

##### include/rand.glsl

- Gold Noise - https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl

##### bloom/downscale.glsl

- Learn OpenGL Phys. Based Bloom - https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom


##### render_passes/pass_normals.glsl

- Learn OpenGL Normal Mapping - https://learnopengl.com/Advanced-Lighting/Normal-Mapping


##### ao.glsl

- Learn OpenGL SSAO - https://learnopengl.com/Advanced-Lighting/SSAO

##### fxaa.glsl

- Catlike Coding: FXAA - https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/

##### Camera.cpp

- Learn OpenGL Gamma Correction - https://learnopengl.com/Advanced-Lighting/Gamma-Correction

#### Academic Sources

These are academic sources I used for reference while researching for this project:

- NVIDIA PCSS Whitepaper - https://developer.download.nvidia.com/shaderlibrary/docs/shadow_PCSS.pdf


- The Cherno: "Bloom." - https://www.youtube.com/watch?v=tI70-HIc5ro&pp=ygUQdGhlIGNoZXJubyBibG9vbQ%3D%3D


- Auto Exposure - https://bruop.github.io/exposure/

#### Asset Sources