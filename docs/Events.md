## Event System

Chimp includes a flexible templated event system designed for type-safe, single-threaded event broadcasting and listening. It's commonly used in the networking code and can be integrated into gameplay logic or systems requiring decoupled communication.

---

### Creating an Event Handler

You can create an event handler using:

```cpp
auto& eventHandler = engine.CreateEventHandler<MyEventEnum, MyEventBase>();
```

- `MyEventEnum` should be an `enum` representing different event types.
- `MyEventBase` is a base class (often a simple struct) that all event structs will derive from.

---

### Subscribing and Unsubscribing

The returned `Chimp::EventHandler` allows listeners to register callbacks for specific event types:

```cpp
eventHandler.Subscribe(MyEventEnum::PlayerDied, [](const MyEventBase& event) {
    // handle event
});

eventHandler.Unsubscribe(MyEventEnum::PlayerDied, yourCallbackPtr); // optional
```

Listeners are type-safe and scoped to the event enum value.

---

### Broadcasting Events

The same call to `CreateEventHandler` also gives you access to an `EventHandler::Broadcaster` instance, which you can use to fire events:

```cpp
MyEvent event{/* init */};
broadcaster.Broadcast(MyEventEnum::PlayerDied, event);
```

All subscribers to that event type will be called with the event data.

---

### Multithreading Notes

- **Not thread-safe.** If you're using events in a multithreaded environment (e.g. from a networking thread), you must queue events manually.
- You can use `Chimp::ThreadSafeQueue` to store events and dispatch them on the main thread.
  - See `impl/networking/IClient.cpp` and `impl/networking/enet/Client.cpp` for how networking uses a buffer for cross-thread dispatch.

---