cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ChimpGame")

add_library (ChimpGame STATIC "src/GameMain.cpp" "src/menu/MenuScene.h" "src/menu/MenuScene.cpp" "src/networking/GameClient.h" "src/networking/GameClient.cpp" "src/stdafx.h" "src/networking/GameServer.h" "src/networking/GameServer.cpp" "src/Logger.h" "src/game/GameScene.h" "src/game/GameScene.cpp" "src/networking/Networking.h" "src/networking/packets/ClientMatchStartPacket.h" "src/networking/server/MatchHandler.h" "src/networking/server/MatchHandler.cpp" "src/networking/server/ServerMatch.h" "src/networking/matches/MatchResult.h" "src/networking/server/ServerMatch.cpp" "src/networking/server/MatchSet.h" "src/networking/server/MatchSet.cpp" "src/networking/packets/Packets.h" "src/networking/client/ClientCurrentMatchHandler.h" "src/networking/client/ClientCurrentMatchHandler.cpp" "src/game/Simulation.h" "src/game/Simulation.cpp" "src/game/Entities.h" "src/entities/BaseEntity.h" "src/entities/BloonsEntities.h" "src/entities/BloonsEntities.cpp" "src/gameover/GameOverScene.h" "src/gameover/GameOverScene.cpp" "src/networking/client/GameWinListener.h" "src/networking/packets/ClientMatchWinPacket.h" "src/networking/client/GameWinListener.cpp" "src/networking/packets/ServerMatchEndPacket.h" "../src/api/ecs/components/HealthComponent.h" "src/entities/health/HealthSystem.h" "../src/api/ecs/components/EntityIdComponent.h" "src/game/bloons/BloonManager.h" "src/entities/bloons/MoveableComponent.h" "src/game/bloons/BloonManager.cpp" "src/game/waves/GameWave.h" "src/game/waves/GameWave.cpp" "src/game/waves/WaveManagerBuilder.h" "src/game/waves/waves/Wave1.h" "src/networking/packets/ClientStartWavePacket.h" "src/networking/packets/ServerWaveEndPacket.h" "src/networking/server/WaveHandler.h" "src/networking/server/WaveHandler.cpp" "src/networking/client/WaveStartListener.h" "src/networking/client/WaveStartListener.cpp" "src/game/waves/waves/Wave2.h" "src/game/waves/waves/Wave3.h" "src/game/waves/WaveStartHandler.h" "src/game/waves/WaveStartHandler.cpp" "src/game/match_end/MatchWinLoseHandler.h" "src/game/match_end/MatchWinLoseHandler.cpp" "src/Debug.h" "src/game/towers/TowerManager.h" "src/entities/towers/MonkeyTowerComponent.h" "src/entities/towers/MonkeyTowerComponent.cpp" "src/entities/towers/TowerTypes.h" "src/game/towers/TowerManager.cpp" "src/entities/bloons/BloonComponent.h" "src/entities/towers/TowerComponent.h" "src/game/towers/systems/TowerAttackSystem.h" "src/game/towers/systems/TowerAttackSystem.cpp" "src/game/towers/systems/TowerTargetSystem.h" "src/game/towers/systems/TowerTargetSystem.cpp" "src/entities/towers/projectiles/Projectile.h" "src/game/towers/systems/TowerProjectileSystem.h" "src/game/towers/systems/TowerProjectileSystem.cpp" "src/game/towers/TowerEditor.h" "src/game/towers/TowerEditor.cpp" "src/networking/packets/ClientTowerPlacePacket.h" "src/networking/client/TowerListener.h" "src/networking/client/TowerListener.cpp" "src/game/towers/systems/TowerSelectionSystem.h" "src/game/towers/systems/TowerSelectionSystem.cpp" "src/entities/towers/WorthComponent.h" "src/entities/towers/UpgradableComponent.h" "src/networking/packets/ClientTowerRemovePacket.h" "src/entities/NetworkedIdentifierComponent.h" "src/entities/NetworkedIdentifierComponent.cpp" "src/game/towers/systems/TowerUpgradeDisplaySystem.h" "src/game/towers/systems/TowerUpgradeDisplaySystem.cpp" "src/networking/packets/ClientTowerUpgradePacket.h" "src/game/bloons/BloonSpawner.h" "src/game/bloons/BloonSpawner.cpp" "src/networking/packets/ClientSpawnBloonPacket.h" "src/networking/client/BloonListener.h" "src/networking/client/BloonListener.cpp" "src/game/MoneyManager.h" "src/game/MoneyManager.cpp"  "src/menu/Settings.h" "src/menu/Settings.cpp" "src/game/waves/waves/Wave4.h" "src/game/waves/waves/Wave5.h" "src/game/waves/waves/Wave6.h" "src/game/waves/waves/Wave7.h" "src/game/waves/waves/Wave8.h" "src/game/waves/waves/Wave9.h" "src/game/waves/waves/Wave10.h")

# Add src as include dir
target_include_directories(ChimpGame PUBLIC "src/")

# Include chimp
add_compile_definitions(CHIMP_FLECS=1)
add_compile_definitions(CHIMP_LIBSNDFILE=1)
target_include_directories(ChimpGame PUBLIC "../src/")
target_link_libraries(ChimpGame PRIVATE imgui flecs::flecs_static)

# In debug mode, we'll just use the data folder in the source directory
# in release mode, we'll copy it to the build directory and use that one
if (CMAKE_BUILD_TYPE STREQUAL "Release")
add_compile_definitions(GAME_DATA_FOLDER="Data")
else()
add_compile_definitions(GAME_DATA_FOLDER="${CMAKE_CURRENT_SOURCE_DIR}/Data")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChimpGame PROPERTY CXX_STANDARD 20)
endif()

# Copy data folder into build directory
# only copy in release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/Data)
add_custom_command(TARGET ChimpGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/Data ${CMAKE_BINARY_DIR}/Data
)
endif()