# Getting Started with Chimp Engine

Chimp is a 3D C++ game engine built around four projects:

- **ChimpEditor**: The main executable for the engine's editor. This is what you run to design and debug your game.
- **ChimpFramework**: A dynamic library (DLL) that contains the core engine code.
- **ChimpGame**: A DLL where your actual game logic lives. This is where you write your game.
- **ChimpRuntime**: A small executable that simply loads and executes ChimpGame, used to launch the game without the editor.

## Project Flow

1. **Build `ChimpGame`**: This is your game's code and entry point. It is compiled as a DLL and should contain your gameplay systems, scripts, and scenes.
2. **Run `ChimpEditor`**: The editor allows you to design levels, place entities, tweak components, and manage assets. It will load `ChimpGame.dll` to access your game logic.
3. **Changes to `ChimpGame` require editor restart**: Due to lack of hot-reloading, you must **restart the editor every time you rebuild `ChimpGame.dll`**.
4. **Use `ChimpRuntime` to launch the final game**: This executable runs the game standalone by loading the compiled `ChimpGame.dll`.

## ECS Architecture

Chimp uses an Entity-Component-System (ECS) model:

- **Entities**: Represent anything in your game world, this could be players, NPCs, items, UI elements, etc.
- **Components**: Store data like transform, animation, sound, etc. These are attached to entities.
- **Systems & Scripts**: Written in C++, these operate on entities with specific components to implement game logic.

## Assets

Your `ChimpGame` project should contain a `Data` folder. Store all game assets here:

- Textures
- Sound effects
- Models
- Any other runtime resources

Chimp will look here when loading assets during both editor time and runtime.

# Rendering a Model in Chimp Engine

This guide walks you through rendering something in Chimp.

---

## 1. Create Your Game Class

Make a class that inherits from `Chimp::IGame`.
You will need to `#include <Chimp.h>` to access Chimp classes, you should put this in all future files.

```cpp
class Game : public Chimp::IGame { /* ... */ };
```

---

## 2. Create Your Scene Class

Make a class that inherits from `Chimp::Scene`.

```cpp
class MainScene : public Chimp::Scene { /* ... */ };
```

---

## 3. Set Your Entry Scene

Inside your `Game` class, override `CreateRuntimeEntryScene`:

```cpp
std::unique_ptr<Chimp::Scene> Game::CreateRuntimeEntryScene(Chimp::Engine& engine)
{
    return UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::Scene, new MainScene(engine));
}
```

---

## 4. Set Up the Camera

In your `MainScene::OnActivate` function:

```cpp
m_camera.SetPosition(Chimp::Vector3f{ 10, 6, 10 });
m_camera.SetForwardVector(m_camera.GetPosition() * -1.0f); // Look at 0 0 0

auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
shader.SetCamera(m_camera);
```

---

## 5. Load ECS Data

In your scene's constructor:

```cpp
m_ecs = engine.LoadECS(std::string(GAME_DATA_FOLDER) + "/ecs.json");
```

Chimp currently supports only one ECS and one scene at a time.

---

## 6. Update the ECS Each Frame

In `MainScene::OnUpdate`:

```cpp
m_ecs->Update();
```

---

## 7. Clear the Screen

At the start of `MainScene::OnRender`:

```cpp
auto& rm = m_engine.GetRenderingManager();

rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
rm.ClearColorBuffer();
rm.ClearDepthBuffer();
```

---

## 8. Set Up Lighting

```cpp
auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
shader.SetLighting(m_ecs->GetLightManager().GenerateLighting());
```

Chimp supports:
- Max 4 directional lights
- Max 4 point lights
- Max 4 spot lights
- Unlimited ambient lights

---

## 9. Render the ECS

```cpp
m_ecs->Render(shader);
```

---

## 10. (Optional) Reset Render Target

If any system or script modifies the render target and doesn't restore it, you may want to reset it to default:

```cpp
rm.SetDefaultRenderTarget(std::weak_ptr<Chimp::IRenderTexture>()/*nullptr*/);
rm.BindDefaultRenderTarget();
```

---

## 11. Render ECS UI

In `MainScene::OnRenderUI`:

```cpp
m_ecs->RenderUI();
```

---

## 12. Add a Model in the Editor

1. Place a model file in the `ChimpGame/Data` folder.
2. Build `ChimpGame`.
3. Launch `ChimpEditor`.
4. Add an entity with a `TransformComponent` and a `MeshComponent`.

---

## 13. View It in Runtime

Close the editor, launch `ChimpRuntime`, and you should see your model rendered in-game.
