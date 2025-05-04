# Chimp

Chimp is a game framework originally started as a university project and later continued as a passion project.

Blaboon is a Bloons TD Battles clone developed by me using Chimp v1: https://github.com/underscore95/Blaboon

## Main Features
### Graphics
- Render textured sprites and 3D models
- Basic lighting (4 spotlights, 4 point lights, 4 directional lights)
- Shadow mapping (including omnidirectional shadow mapping)
- Supports custom user-written shaders (no lighting in custom shaders though)
- OpenGL backend

### Networking
- Packets can be sent as binary data over TCP protocol
- Chimp can host a server or connect to another server
- Packets can be listened for using events
- Packets support callbacks (lambda function that is called with the server/client's response to your packet)
- Clients can communicate with other clients (server forwards packets, not peer to peer)
- Networking happens on a dedicated thread, but events and callbacks are on main thread
- ENet backend

### Entities
- Chimp provides an ECS which wraps FLECS
- Entities can have parents and children, transforms respect this hierarchy

### Resource Management
- Resources can be loaded and then will automatically be unloaded when nothing is referencing them
- Won't reload shared resources when switching scenes
  
### Audio
- Sound effect system (setup a list of sound files, pitch and volume range for extra variety)
- Positional audio
- Music system supporting fading between tracks
- OpenAL backend
