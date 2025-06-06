#pragma once

#include "api/ecs/ISystem.h"
#include "stdafx.h"

namespace Chimp {
	class SystemContainerSystem : public ISystem {
	public:
		SystemContainerSystem(Engine& engine, ECS& ecs);
		~SystemContainerSystem() override;

		void RegisterSystem(std::unique_ptr<ISystem> system);

		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

	private:
		std::vector<std::unique_ptr<ISystem>> m_Systems;
	};
}