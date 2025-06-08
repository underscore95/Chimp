# Entities and ECS in Chimp

Chimp uses an **Entity-Component-System (ECS)** architecture. Here's a simple breakdown:

- **Entity**: An ID that represents something in your game (like a character, light, UI element). In Chimp, this is a Chimp::EntityId.
- **Component**: A small data container (e.g. position, pointer to a mesh, etc).
- **System**: Code that operates on entities with specific components (e.g. a render system that draws all entities with mesh and transform components).

---

## Creating an ECS

You can create an ECS from scratch:

```cpp
Chimp::ECS& ecs = engine.CreateECS();
```

Or you can load one from a JSON string:

```cpp
std::unique_ptr<Chimp::ECS> ecs = Chimp::ECS::Deserialise(jsonString);
```

---

## Registering Systems

Register a system like this:

```cpp
ecs.GetSystems().RegisterSystem(std::make_unique<MySystem>());
```

Your system class must inherit from `Chimp::ISystem`.

For finer control, you can create and run systems manually inside your scene's `OnUpdate` function.

---

## Entity Hierarchy

Chimp supports parent-child relationships between entities. Children inherit their parent's transform.

Use the hierarchy manager to manage relationships:

```cpp
ecs.GetHierarchy().SetParent(childEntity, parentEntity);
```

---

## Modifying Transforms

**Don't** modify the `TransformComponent` directly.

Instead, use the `TransformManager`:

```cpp
ecs.GetTransformManager().SetWorldPosition(entity, Chimp::Vector3f{1, 2, 3});
```

Modifying transforms this way ensures matrix updates and child propagation work correctly.

---

## Looking Up Entities

You can find entities by name:

```cpp
Chimp::EntityID id = ecs.FindFirstEntityByName("MyEntityName");
```

This is useful for accessing editor-created entities.

---

## Iterating Over Entities with Components

To operate on entities with specific components:

```cpp
auto view = ecs.GetEntitiesWithComponents<Chimp::TransformComponent, Chimp::MeshComponent>();

for (auto& [transform, mesh] : view)
{
    // do stuff
}
```

---

## Scripting

Chimp supports scripting directly in C++. Scripts are useful for adding custom logic to specific entities, especially when creating systems would be unnecessary or overkill. For example, controlling a unique entity like a player or a boss might be best handled via a script.

> ⚠️ **Performance Note:** Scripts are slower than systems. If you're running logic on many entities, **use a system** instead.

---

### Using Scripts

You can access the script interface with:

```cpp
ecs.GetScripts();
```

### Attaching & Detaching Scripts

```cpp
ScriptId id = ecs.GetScripts().AttachScript(entity, "MyScript");

ecs.GetScripts().DetachScript(entity, id);
```

---

### Accessing Scripts

```cpp
// Get a script by ID
auto script = ecs.GetScripts().GetScript(entity, id);

// Get first matching script with a predicate
auto match = ecs.GetScripts().GetFirstScript(entity, [](Chimp::IEntityScript* s) {
    return s->IsEnabled();
});

// Get first script of specific type
auto typed = ecs.GetScripts().GetFirstScript<MyScript>(entity);
```

---

### Registering Scripts

Scripts must be registered in your `IGame::Setup` function:

```cpp
CHIMP_GAME_REGISTER_SCRIPT(MyScript, MyScriptClass);
```

---

## Component Registration

Before using a custom component in Chimp you must register it.

This is done by creating a static instance of a class that inherits from `Chimp::ComponentRegister<YourComponent>`.

---

### Example: Registering a `HealthComponent`

```cpp
namespace Chimp {
	struct HealthComponent {
		float Health = 1.0f; // If reaches 0, entity dies (you must implement this logic in a system)
	};

	namespace Unused { // Namespace doesn't matter, it is just put in a different namespace for organisation
		class HealthComponentRegister : public ComponentRegister<HealthComponent> {
		public:
            // Optional function, can be used to render ImGui controls in the inspector in the editor
			void RenderInspectorUI(EntityId id, HealthComponent& comp) override {
				auto label = std::format("Health##{}", (long)id);
				ImGui::InputFloat(label.c_str(), &comp.Health, 1.0f, 10.0f);
			}

            // Required
			HealthComponent Deserialise(const Json& json) override {
				return {
					.Health = json["Health"]
				};
			}

            // Required
			void Serialise(Json& json, const HealthComponent& comp) override {
				json["Health"] = comp.Health;
			}
		};

        // This macro creates a static instance of your class, don't forget it!
        // expands to: inline static Register ComponentRegister_##Register where Register is the class name 
        // of your register class.
		COMPONENT_REGISTER(HealthComponentRegister); 
	}
}
```

---

## Other ECS Functionality

The ECS also includes functions to:

- **Create and destroy entities**
- **Get, set, and remove components**
- **Manage the scene's state**
- **Access specific managers like lights, transforms, hierarchy, etc.**

Explore the ECS interface for full control over your game logic and entity state.
