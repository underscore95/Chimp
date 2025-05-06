#pragma once

namespace Chimp {
	class Engine;
	class ECS;

	class ISystem {
	public:
		ISystem(Engine& engine, ECS& ecs) : m_Engine(engine), m_ECS(ecs) {}

		virtual void OnInit() {};
		virtual void OnUpdate() {};
		virtual void OnRender() {};
		virtual void OnRenderUI() {};

	protected:
		inline Engine& GetEngine() { return m_Engine; }
		inline ECS& GetECS() { return m_ECS; }

	private:
		Engine& m_Engine;
		ECS& m_ECS;
	};
}