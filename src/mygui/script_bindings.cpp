#include "mygui/script_bindings.h"

#include "scripting/luabind.h"

#include <MyGUI/MyGUI.h>
#include <MyGUI/MyGUI_OgrePlatform.h>

using namespace luabind;
using namespace MyGUI;

static luabind::scope
widgetBindings() {
    return class_<Widget>("Widget")
        //.def(const_self == other<Widget>())
        .def("setPosition", static_cast<
             void (Widget::*) (int, int)>(&Widget::setPosition))
        .def("setSize", static_cast<
             void (Widget::*) (int, int)>(&Widget::setSize))
        .def("setCoord", static_cast<
             void (Widget::*) (int, int, int, int)>(&Widget::setCoord))
        .def("setRealPosition", static_cast<
             void (Widget::*) (float, float)>(&Widget::setRealPosition))
        .def("setRealSize", static_cast<
             void (Widget::*) (float, float)>(&Widget::setRealSize))
        .def("setRealCoord", static_cast<
             void (Widget::*) (float, float,float,float)>(&Widget::setRealCoord))
        .def("setVisible", &Widget::setVisible)
        .def("getVisible", &Widget::getVisible)
        .def("setAlpha", &Widget::setAlpha)
        .def("getAlpha", &Widget::getAlpha)
        .def("setColour", &Widget::setColour)
        .def("setInheritsAlpha", &Widget::setInheritsAlpha)
        .def("getInheritsAlpha", &Widget::getInheritsAlpha)
        .def("setEnabled", &Widget::setEnabled)
        .def("setEnabledSilent", &Widget::setEnabledSilent)
        .def("getEnabled", &Widget::getEnabled)
        .def("setProperty", &Widget::setProperty)
    ;
}

static luabind::scope
buttonBindings() {
    return class_<Button, Widget, std::shared_ptr<Widget>>("Button")
        .def(constructor<>())
        .def("setStateSelected", &Button::setStateSelected)
        .def("getStateSelected", &Button::getStateSelected)
        .def("setModeImage", &Button::setModeImage)
        .def("getModeImage", &Button::getModeImage)
        .def("setImageResource", &Button::setImageResource)
    ;
}

luabind::scope
thrive::MyGUIBindings::luaBindings() {
    return(
        widgetBindings(),
        buttonBindings()
    );
}

