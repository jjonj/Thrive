#pragma once

namespace luabind {
class scope;
}

namespace thrive {

/**
* @brief Helper for MyGUI script bindings
*/
struct MyGUIBindings {

    /**
    * @brief Lua bindings for MyGUI
    *
    * The exposed classes try to be as close to the C++ MyGUI API as
    * possible.
    *
    * The currently exported classes are
    *   - Nothing
    */
    static luabind::scope
    luaBindings();

};

}
