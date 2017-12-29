![fmod](./docs/fmod_logo.png)

# FMOD extension for Defold

> **Experimental** [FMOD] bindings for Defold. Currently macOS-only.

## Installation

~~As usual, add `https://github.com/dapetcu21/defold-fmod/master.zip` to
dependencies in `game.project`.~~

Because the FMOD license doesn't allow me to distribute FMOD with this package, you'll have to:

1. Clone/fork this repo.
2. Download FMOD API 1.10.02 for macOS.
3. Copy the FMOD headers from `api/lowlevel/inc` and `api/studio/inc` to `fmod/include`.
4. Copy the FMOD libraries from `api/lowlevel/libfmod.dylib` and `api/studio/libfmodstudio.dylib` to `fmod/lib/x86_64-osx`.
5. Change the install paths of the libraries (workaround for DEF-2732):
  ```bash
  cd fmod/lib/x86_64-osx
  install_name_tool -id @executable_path/libfmod.dylib libfmod.dylib
  install_name_tool -id @executable_path/libfmodstudio.dylib libfmodstudio.dylib
  install_name_tool -change @rpath/libfmod.dylib @executable_path/libfmod.dylib libfmodstudio.dylib
  ```
6. Copy the libraries you just patched to `fmod/res/x86_64-osx/Contents/MacOS` (workaround for DEF-2732).
7. Make a zip out of the whole thing and add its URL to dependencies in `game.project`, or just copy the `fmod` dir into your project.

## Running

Defold doesn't currently support dynamic libraries (DEF-2732), so a hacky
workaround is needed to run the game in the editor.

The game will bundle fine, but will not start in the editor unless you start
the editor with:

```bash
DYLD_LIBRARY_PATH="/path/to/defold-fmod/fmod/lib/x86_64-osx" /path/to/Defold.app/Contents/MacOS/Defold
```

## Usage

Structs and classes are exposed on the `fmod` and `fmod.studio` namespaces. All
method names are converted from `camelCase` to `snake_case`. Methods that
returned a value through a last pointer argument now actually return the value and
throw with an error string when their result is not `FMOD_OK`.

Enums are exposed on the `fmod` table without the leading `FMOD_`. (eg.: `FMOD_STUDIO_PLAYBACK_PLAYING` is exposed as `fmod.STUDIO_PLAYBACK_PLAYING`)

A fully initialised instance of `FMOD::Studio::System` is exposed to Lua as `fmod.studio.system` and the corresponding instance of `FMOD::System` (the low level system), is exposed as `fmod.system`.

See an [example script][example] to get an idea.

Refer to the [FMOD API Documentation] for details about the available APIs.

Not all APIs are exposed yet. For a full list of exposed functions, see [fmod/src/fmod_classes.cpp#L161](fmod/src/fmod_classes.cpp#L161).

Here's some sample code:

```lua
fmod.studio.system:load_bank_memory(resource.load("/resources/Master Bank.bank"), fmod.STUDIO_LOAD_BANK_NORMAL)
fmod.studio.system:load_bank_memory(resource.load("/resources/Master Bank.strings.bank"), fmod.STUDIO_LOAD_BANK_NORMAL)
fmod.studio.system:load_bank_memory(resource.load("/resources/Vehicles.bank"), fmod.STUDIO_LOAD_BANK_NORMAL)

local event_description = fmod.studio.system:get_event("event:/Vehicles/Basic Engine")
local event = event_description:create_instance()
event:start()
```

[example]: ./main/main.script
[FMOD]: https://fmod.com
[FMOD API Documentation]: https://www.fmod.com/resources/documentation-api