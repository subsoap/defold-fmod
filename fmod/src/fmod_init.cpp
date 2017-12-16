#include <fmod_errors.h>
#include <dmsdk/sdk.h>
#include <LuaBridge/LuaBridge.h>

#include "fmod_bridge.hpp"

using namespace FMODBridge;
using namespace luabridge;

FMOD::Studio::System* FMODBridge::system = NULL;
FMOD::System* FMODBridge::lowLevelSystem = NULL;

void FMODBridge::init(lua_State *L) {
    FMOD_RESULT res;

    res = FMOD::Studio::System::create(&system);
    if (res != FMOD_OK) {
        printf("FMOD Error: %s\n", FMOD_ErrorString(res));
        system = NULL;
        return;
    }

    res = system->getLowLevelSystem(&lowLevelSystem);
    if (res != FMOD_OK) {
        printf("FMOD Error: %s\n", FMOD_ErrorString(res));
        system->release();
        system = NULL;
        return;
    }

    void* extraDriverData = NULL;
    res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
    if (res != FMOD_OK) {
        printf("FMOD Error: %s\n", FMOD_ErrorString(res));
        system->release();
        system = NULL;
        return;
    }

    registerEnums(L);
    registerClasses(L);

    getGlobalNamespace(L)
        .beginNamespace("fmod")
            .addVariable("system", &lowLevelSystem, false)
            .beginNamespace("studio")
                .addVariable("system", &system, false)
            .endNamespace()
        .endNamespace();
}

void FMODBridge::finalize() {
    if (system) {
        FMOD_RESULT res = system->release();
        if (res != FMOD_OK) { printf("FMOD Error: %s\n", FMOD_ErrorString(res)); }
        system = NULL;
    }
}