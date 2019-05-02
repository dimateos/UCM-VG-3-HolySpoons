#include "KeyMapper.h"

#include <SDL_keycode.h>

std::map<std::string, SDL_Keycode> KeyMapper::specialKeys = {
	{"esc", SDLK_ESCAPE},
	{"space", SDLK_SPACE},
	{"shift", SDLK_LSHIFT},
};

SDL_Keycode KeyMapper::getKeycode(std::string k) {
	//try finding it in the special keys map
	if (specialKeys.count(k) != 0) return specialKeys[k];

	//direct get the key from the char
	return SDL_Keycode(k[0]);
}
