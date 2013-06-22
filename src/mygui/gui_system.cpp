#include "mygui/gui_system.h"

#include "common/transform.h"
#include "engine/component_registry.h"
#include "engine/entity_filter.h"
#include "ogre/ogre_engine.h"
#include "scripting/luabind.h"

#include <OgreSceneManager.h>

using namespace thrive;

WidgetComponent::WidgetComponent(MyGUI::Widget* widget, MyGUI::Widget* parent){
    m_widget.workingCopy() = widget;
    //if (parent=!0)
        m_parentWidget = parent;
}

luabind::scope
WidgetComponent::luaBindings() {
    using namespace luabind;
    return class_<WidgetComponent, Component, std::shared_ptr<Component>>("WidgetComponent")
        .scope [
            def("TYPE_NAME", &WidgetComponent::TYPE_NAME),
            def("TYPE_ID", &WidgetComponent::TYPE_ID)
        ]
        .def(constructor<MyGUI::Widget*,MyGUI::Widget*>())
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
        if (widgetComponent->m_parentWidget==0)
            widgetComponent->m_widget.workingCopy()->attachToWidget(dynamic_cast<MyGUI::Widget*>(m_impl->m_GUI));
        else
            widgetComponent->m_widget.workingCopy()->attachToWidget(widgetComponent->m_parentWidget);
        m_impl->m_widgets[entityId] = std::move(widgetComponent->m_widget.workingCopy());

    }
    m_impl->m_entities.addedEntities().clear();
    added.clear();

    for (EntityId entityId : m_impl->m_entities.removedEntities()) {
        MyGUI::Widget* widget = m_impl->m_widgets[entityId];
        m_impl->m_GUI->destroyChildWidget(widget);
        m_impl->m_widgets.erase(entityId);
    }
    m_impl->m_entities.removedEntities().clear();
}
