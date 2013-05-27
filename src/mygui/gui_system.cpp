#include "ogre/scene_node_system.h"

#include "common/transform.h"
#include "engine/component_registry.h"
#include "engine/entity_filter.h"
#include "ogre/ogre_engine.h"
#include "scripting/luabind.h"

#include <OgreSceneManager.h>

using namespace thrive;

luabind::scope
WidgetComponent::luaBindings() {
    using namespace luabind;
    return class_<WidgetComponent, Component, std::shared_ptr<Component>>("WidgetComponent")
        .scope [
            def("TYPE_NAME", &OgreSceneNodeComponent::TYPE_NAME),
            def("TYPE_ID", &OgreSceneNodeComponent::TYPE_ID)
        ]
        //.def(constructor<>())
    ;
}

REGISTER_COMPONENT(WidgetComponent)

////////////////////////////////////////////////////////////////////////////////
// GUISystem
////////////////////////////////////////////////////////////////////////////////

struct GUISystem::Implementation {

    MyGUI::Gui* m_GUI = nullptr;

    std::unordered_map<EntityId, MyGUI::Widget*> m_widgets;

    EntityFilter<WidgetComponent> m_entities = {true};
};


GUISystem::GUISystem()
  : m_impl(new Implementation())
{
}


GUISystem::~GUISystem() {}


void
GUISystem::init(
    Engine* engine
) {
    System::init(engine);
    assert(m_impl->m_GUI == nullptr && "Double init of system");
    OgreEngine* ogreEngine = dynamic_cast<OgreEngine*>(engine);
    assert(ogreEngine != nullptr && "System requires an OgreEngine");
    m_impl->m_GUI = ogreEngine->GUI();
    m_impl->m_entities.setEngine(engine);
}


void
GUISystem::shutdown() {
    m_impl->m_entities.setEngine(nullptr);
    m_impl->m_GUI = nullptr;
    System::shutdown();
}


void
GUISystem::update(int) {
    auto& added = m_impl->m_entities.addedEntities();
    for (const auto& entry : added) {
        EntityId entityId = entry.first;
        WidgetComponent* widgetComponent = std::get<0>(entry.second);
        const auto& properties = widgetComponent->m_properties.stable();
        widgetComponent->m_widget = m_impl->m_GUI->createWidgetRealT(
            properties.type,properties.skin,properties.left,properties.top,
            properties.width,properties.height,properties.align,properties.layer,properties.name);
        m_impl->m_widgets[entityId] = std::move(widgetComponent->m_widget);

    }
    m_impl->m_entities.addedEntities().clear();
    added.clear();

    for (EntityId entityId : m_impl->m_entities.removedEntities()) {
        WidgetComponent* widget = m_impl->m_widgets[entityId].get();
        m_impl->m_GUI->destroyChildWidget(widget);
        m_impl->m_widgets.erase(entityId);
    }
    m_impl->m_entities.removedEntities().clear();
}
