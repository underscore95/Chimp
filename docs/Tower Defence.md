## Tower Defence Utilities

Chimp was originally developed as a tower defense engine, which is why it includes built-in utilities like the Enemy Wave Manager and path-following tools tailored for enemy spawning and movement patterns.

### Enemy Wave Manager

1. **Base Wave Class**  
   Create a class inheriting from `Chimp::Wave`. This will be the base class for all your waves.  
   Implement protected helper functions that spawn enemies using the following `Chimp::Wave` methods:  
   - `RunFunction`  
   - `RunRepeatingFunction`  
   - `Delay`  
   (See `game/waves/GameWave.cpp/h` for examples.)

2. **Specific Wave Classes**  
   For each wave, create a class inheriting your base wave class. Use your spawning helper functions within the constructor to define the wave's enemy spawns.  
   (See `game/waves/waves/Wave1.h`.)

3. **Wave Manager Setup**  
   Instantiate a `Chimp::WaveManager::Builder`, add your waves using `AddWave()`, then call `Build()` to create the manager.

4. **Updating Waves**  
   Call `Update()` on the wave manager every frame to process wave logic.

5. **Starting Waves**  
   You can control wave start either manually using `AllowStartNextWave()`, or enable automatic wave starts with `SetWaveAutoStart(bool)`.

---

### Paths Utility

- `Chimp::Path` is a templated utility for defining movement paths with points in 1 to 4 dimensions.  
- It is commonly used to make enemies follow predefined paths.  

---

### Example

This game built in Chimp 1.0 uses these utilities.

https://github.com/underscore95/Blaboon
