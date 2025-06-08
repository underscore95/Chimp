## Window & Input

### Window

- The engine automatically creates and opens a window; you can access and modify it via `engine.GetWindow()`.
- `IWindow` is the base interface for windows, with core functions including:
  - `Update()` — called at the start of every frame; polls events and updates internal state such as whether the window should close.
  - `SwapBuffers()` — called after rendering to swap front and back buffers.
  - `GetSize()` / `SetSize()` — query or set the window size.
  - `SetTitle()` — set the window title.
  - `SetResizable()` — enable or disable window resizing.
  - `HasResized()` — returns true if the window was resized since the last call (also true on first call, since opening counts as resize).
  - `GetStatus()` — returns the current window status (e.g. successfully initialized or failed).
  - `GetInputManager()` — retrieves the input manager for the window.

### InputManager

- Manages keyboard and mouse input states.
- Tracks keys and mouse buttons in two states:
  - **Down**: the key/button is currently held down.
  - **Pressed**: the key/button was pressed this frame (transitions from up to down).
- Provides the following methods:
  - `IsKeyDown(Keyboard::Key key)` — returns true if the specified key is currently held down.
  - `IsKeyPressed(Keyboard::Key key)` — returns true if the specified key was pressed this frame.
  - `IsMouseButtonDown(Mouse::Button button)` — returns true if the specified mouse button is currently held down.
  - `IsMouseButtonPressed(Mouse::Button button)` — returns true if the specified mouse button was pressed this frame.
  - `GetMousePosition()` — returns the current mouse position relative to the window.

  ---