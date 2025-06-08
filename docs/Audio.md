## Sound Effects & Music

### Sound Effects

1. **Configuration**  
   Sound effects are defined using YAML files that specify audio files, pitch variations, and other settings. Example YAML can be found at:  
   `ChimpGame/Data/Assets/Sounds/Bloon.yml`  
   Note: The YAML format used is non-standard, see the YAML section for details.

2. **Loading Sound Effects**  
   Use the `SoundEffects` resource container to load/unload sound effect YAML files.  
   Audio files referenced in the YAML are managed automatically:  
   - Loaded only once even if multiple sound effects use the same file.  
   - Unloaded when no longer needed.

3. **Playing Sound Effects**  
   Use `sound.Play()` or `sound.PlayForce()` to play sounds. Both functions optionally accept:  
   - Position (3D spatial audio)  
   - Velocity  
   - Volume

4. **Direct SoundEffect Usage**  
   You can create your own `SoundEffect` instances directly without using the resource manager, but in that case you must call their `Update()` method every frame to keep them working properly.

---

### Music

1. **Music Player**  
   The engine exposes a `MusicPlayer` instance accessible via the engine object. Use `SwitchMusic()` to start playing music from a list of audio files.

2. **Automatic Handling**  
   - Crossfade between tracks is automatic.  
   - Tracks are selected randomly and loop indefinitely until `SwitchMusic` is called again.  
   - Only two tracks are loaded in memory at once, with the next track loaded asynchronously.

3. **Customization**  
   Volume, position, velocity, and fade speed can all be configured for music playback.