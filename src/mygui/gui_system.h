#pragma once

#include "engine/component.h"
#include "engine/shared_data.h"
#include "engine/system.h"

#include <memory>


#include <iostream>

namespace thrive {

/**
* @brief A component for a MyGUI widget
*
*/
class WidgetComponent : public Component {
    COMPONENT(Widget)

public:

    struct Properties {
    std::string type = "";
    float top = 0;
    float left = 0;
    float width = 0;
    float height = 0;
    std::string skin = "";
    bool visible = false;
    float alpha = 100;
    bool enabled = false;
    MyGUI::Colour colour;
    MyGUI::WidgetStyle style;

    }

    /**
    * @brief Lua bindings
    *
    * Doesn't expose anything special, but this component is needed for many
    * systems.
    *
    * @return
    */
    static luabind::scope
    luaBindings();

    /**
    * @brief Pointer to the underlying Widget
    *
    * Be careful to only use this in the graphics thread
    */
    MyGUI::Widget* m_widget = nullptr;

    RenderData<Properties>
    m_properties;
};


/**
* @brief Creates scene nodes for new OgreSceneNodeComponents
*/
class GUISystem : public System {

public:

    /**
    * @brief Constructor
    */
    GUISystem();

    /**
    * @brief Destructor
    */
    ~GUISystem();

    /**
    * @brief Initializes the system
    *
    * @param engine
    *   Must be an OgreEngine
    */
    void init(Engine* engine) override;

    /**
    * @brief Shuts the system down
    */
    void shutdown() override;

    /**
    * @brief Adds new scene nodes
    */
    void update(int) override;

private:

    struct Implementation;
    std::unique_ptr<Implementation> m_impl;
};

}


