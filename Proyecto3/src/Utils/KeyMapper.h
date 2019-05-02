#ifndef KEYMAPPER_H_
#define KEYMAPPER_H_

#include <string>
#include <map>

typedef signed __int32 SDL_Keycode; //fowarded

//maps strings to SDL_Keycodes
class KeyMapper
{
public:
	static SDL_Keycode getKeycode(std::string k);

private:
	static std::map<std::string, SDL_Keycode> specialKeys;
};

#endif /* KEYMAPPER_H_ */

