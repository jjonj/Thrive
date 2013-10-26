#ifndef GAMESTATE_SYSTEM_H
#define GAMESTATE_SYSTEM_H

#include "engine/system.h"
#include "engine/entity.h"

#include <vector>

namespace thrive {

    class GameState {

    public:

        GameState(std::vector<System> activeSystems);
        ~GameState();

    private:

        std::vector<System> m_stateSystems;
        std::vector<Entity> m_stateEntities;
    }


    class GameStateSystem : public system {

    public:

        GameStateSystem();
        ~GameStateSystem();

        void switchState(GameState newState);

    private:

        struct Implementation;
        std::unique_ptr<Implementation> m_impl;

    };

}

#endif // GAMESTATE_SYSTEM_H
