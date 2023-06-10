# My little DX11 Playground

For one of the university courses I was working on a HoloLens 2 project, for which I was given a source code for an UWP application built with OpenXR and DirectX 11. However, I hardly understood what most of the code did, as I had had no prior experience with those technologies. 

For that reason I created this little project to learn the basics, step by step: 
- [x] Build the basic example following the [official tutorial](https://learn.microsoft.com/en-us/windows/win32/direct3dgetstarted/getting-started-with-a-directx-game),
- [x] Add ImGui and rewrite the code to be more modular 
- [x] Add loading and transforming custom 3D models
- [x] Add support for different shaders per model
- [x] Add loading custom cubemaps (skyboxes) and allow for switching between them at runtime
- [x] Understand and implement Spherical Harmonics Lighting using [Robin's Green Spherical Harmonics Lighting: The gritty detail](https://www.cse.chalmers.se/~uffe/xjobb/Readings/GlobalIllumination/Spherical%20Harmonic%20Lighting%20-%20the%20gritty%20details.pdf) as the main source

This allowed me to build a good enough understanding of DX11 to complete the original project, [SHARCC](https://jaolejnik.github.io/portfolio-site/#projects).

![Example](https://i.imgur.com/w80SYAu.png)
