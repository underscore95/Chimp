## Graphics

Chimp provides a rendering pipeline with lighting, shadows, and support for custom rendering via systems and ImGui.

---

### 1. **Models and Textures**

See the [Resources](#resources) documentation for how to load models and textures using the resource manager.

---

### 2. **Lighting + Shader**

Chimp includes a built-in lit shader with support for:

- Ambient lights (unlimited)
- 4 directional lights
- 4 spot lights
- 4 point lights

Shadows are rendered using **shadow mapping**, with **PCF** for soft shadows.

Access the lit shader like so:

```cpp
auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
```

Every frame, you need to set the active lighting from your ECS:

```cpp
shader.SetLighting(m_ecs->GetLightManager().GenerateLighting());
```

This step is required **every frame** (or technically, whenever a light component changes).

---

### 3. **Rendering ECS**

To render your ECS:

```cpp
m_ecs->Render(shader);
```

This:

- Executes all registered render systems
- Renders all entities that have a `MeshComponent`, `TransformComponent`, and valid `EntityId`

---

### 4. **Render Manager**

```cpp
auto& rm = m_engine.GetRenderingManager();
```

#### Useful functions:
```cpp
rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
rm.ClearColorBuffer();
rm.ClearDepthBuffer();
```

You can also:

- Create and bind custom render targets
- Reset to default render target
- Use ImGui for any UI rendering

---

### 5. **UI**

To render UI with ImGui, call:

```cpp
m_ecs->RenderUI();
```

You can also render any custom ImGui elements inside `OnRenderUI()` of your scene or systems.

---

### 6. **Custom Rendering**

If you need custom drawing logic or post-processing:

- Write your own system
- Or manually render in `Scene::OnRender()` using the rendering manager

Chimp is flexible enough to support custom pipelines layered on top of the built-in ECS + lighting system.

You can create your own shaders, see the built in LitShader class as an example.
The LitShader glsl files are located in ChimpFramework/ChimpData.
