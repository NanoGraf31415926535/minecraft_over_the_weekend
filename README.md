# Minecraft, but I made it in 3 days*

\*This started as a 48-hour challenge but has since been improved. Developed by Artem.
![image](https://github.com/user-attachments/assets/7d808885-1f31-4040-b1cd-cca231d18aba)

#### Features:

- Infinite, procedurally generated world
- Infinite height/depth
- Day/night cycle
- Biomes
- ECS-driven player and entities with full collision and movement
- Full RGB lighting
- Full transparency + translucency support
- Sprite blocks (flowers)
- Animated blocks (water + lava)
- Distance fog
- A whole lot of different block types
- More

#### Building

##### Unix-like

`$ git clone --recurse-submodules https://github.com/NanoGraf31415926535/minecraft_over_the_weekend.git `

`$ make`

The following static libraries under `lib/` must be built before the main project can be built:

- GLAD `lib/glad/src/glad.o`
- CGLM `lib/cglm/.libs/libcglm.a`
- GLFW `lib/glfw/src/libglfw3.a`
- libnoise `lib/noise/libnoise.a`

All of the above have their own Makefile under their respective subdirectory and can be built with `$ make libs`.

If libraries are not found, ensure that submodules have been cloned.

The game binary, once built with `$ make`, can be found in `./bin/`.

*Be sure* to run with `$ ./bin/game` out of the root directory of the repository.
If you are getting "cannot open file" errors (such as "cannot find ./res/shaders/*.vs"), this is the issue.

##### Windows

You might have better luck building under WSL with an X environment to pass graphics through. Otherwise, good luck! 🤷‍♂️

##### Windows

good luck 🤷‍♂️ probably try building under WSL and using an X environment to pass graphics through.
