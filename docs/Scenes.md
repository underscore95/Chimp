## Scenes

Chimp uses a scene-based system to manage game states (e.g., menus, levels, pause screens). Scenes are fully managed by the engine, but it's your responsibility to implement them correctly.

---

### Entry Point

To set your game's entry scene, implement:

```cpp
std::unique_ptr<Chimp::Scene> Game::CreateRuntimeEntryScene(Chimp::Engine& engine) {
    return UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::Scene, new MainScene(engine));
}
```

This should be in your class that inherits `Chimp::IGame`.

The engine will load this scene automatically when the game starts.

---

### Scene Lifecycle

Scenes inherit from `Chimp::Scene`. The engine calls the following functions during a scene's lifetime:

```cpp
class Scene {
    // Called once the first time this scene is activated. Resources will be loaded.
    virtual void OnInit() = 0;

    // Called each time the scene becomes active (after OnInit if it's the first time).
    // `previousScene` may be stored for later use (e.g. returning to it).
    virtual void OnActivate(std::unique_ptr<Scene> previousScene = nullptr) = 0;

    // Called each time the scene is switched out.
    virtual void OnDeactivate() = 0;

    // Called every frame to update game logic.
    virtual void OnUpdate() = 0;

    // Called every frame to perform rendering.
    virtual void OnRender() = 0;

    // Called every frame to render UI with ImGui.
    virtual void OnRenderUI() = 0;

    // Called to mark required resources (via `Depend`). Chimp won't call this.
    virtual void LoadResources() = 0;

    // Called to release unneeded resources (via `Release`). Chimp won't call this.
    virtual void UnloadResources() = 0;

    // Override to define when the game should exit.
    virtual bool ShouldExit(Chimp::Engine& engine) const;
};
```

---

### Scene Switching

To queue a scene change:

```cpp
engine.GetSceneManager().QueueSceneChange(std::make_unique<YourNextScene>(engine));
```

> For an immediate switch, use `SceneChange()` instead. Use with caution; avoid during frame updates.

---

### Editor Note

Chimp's editor currently only supports a **single ECS** and does **not** support scenes. Scenes are only relevant in runtime builds.

### ECS

A scene doesn't contain an ECS by default, you will need to make your own, see the Entities documentation for how to do this.

---
