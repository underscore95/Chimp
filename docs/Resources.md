## Resources

Chimp includes a reference-counted resource management system designed to automatically handle loading and unloading of assets based on usage. This allows for resources to be used in multiple scenes without requiring reloading.

In the below documentation, "asset" refers to something on disk, "resource" is something in memory. Once you load an asset it becomes a resource.

---

### 1. **Data Folder**

All assets should be stored in the `Data` folder inside `ChimpGame`. Use the macro:

```cpp
GAME_DATA_FOLDER
```

- In **debug** builds, this points to the actual `Data` directory.
- In **release** builds, the `Data` folder is copied into the build directory.

---

### 2. **Accessing Resources**

The resource manager groups assets by type into separate containers (e.g., sprites, models, sounds). You **don’t** access the resource manager directly — instead, use accessors like:

```cpp
engine.GetResourceManager().GetSprites();
engine.GetResourceManager().GetModels();
engine.GetResourceManager().GetSoundEffects();
```

See the `ResourceManager` header for the full list.

---

### 3. **Reference Counting**

Resources are managed using **reference counting**. Anything that needs a resource can:

- **Depend** on it (increments reference count)
- **Release** it (decrements reference count)

Use these to manage lifetime:

```cpp
auto& models = engine.GetResourceManager().GetModels();
auto handle = models.Depend("Data/Models/Tree.model");

// Later
models.Release(handle);
```

You can also use `Chimp::ResourceDependency`, which wraps this logic and behaves like a `shared_ptr`:

```cpp
Chimp::ResourceDependency<Model> modelDep("Data/Models/Tree.model", models);
```

---

### 4. **Scene Switching**

When changing scenes:

- `UnloadUnusedResources()` is called to unload anything with a zero ref count.
- `LoadRequiredResources()` is called to preload resources with existing dependencies.

This is handled **automatically**, no manual calls needed.

---

### 5. **Model Import Settings**

The model importer has a static field `ImportSettings`. You can configure how models are processed here:

```cpp
Chimp::ModelImporter::ImportSettings.importNormals = false;
```

This can be configured in the editor.

---

### 6. **Immediate Resource Access**

If you need a resource **immediately** (synchronously), use:

```cpp
auto handle = models.ImmediateDepend("Data/Models/Tree.model");
```

Be sure to manually `Release` it later to avoid leaks.

---

### 7. **ResourceDependency (RAII)**

For automatic management of reference counts:

```cpp
Chimp::ResourceDependency<Model> myModel("Data/Models/Tree.model", models);
```

On construction, the ref count is incremented. On destruction, it's decremented. Just like a shared ptr.

---

### 8. **Editor + ECS Integration**

When an ECS is loaded from JSON, any resources referenced in its components are **automatically loaded**. This uses `Chimp::ResourceReference` internally.

If you're writing your own components and want this behavior, store your resources as `ResourceReference<T>`. They will be resolved and ref-counted automatically during deserialisation.
```
