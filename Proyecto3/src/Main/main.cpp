//Nap_Time_Studios

//#include "checkML.h" //for memory leaks
#include "Game.h"
#include <SoundManager.h>
#include <irrKlang.h>
#include <windows.h>

using namespace irrklang;

int main(int argc, char *argv[])
{
	//see if there are memory leaks (in debug mode)
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*Game game;
	game.start();*/

	// SOUND MANAGER EXAMPLE: playing a 3d sound in movement
	SoundManager* soundManager = SoundManager::getSingleton();

	if (!soundManager) return 0; // error starting up the engine

	// Now play some sound stream as music in 3d space, looped.
	// We are setting the last parameter named 'track' to 'true' to
	// make irrKlang return a pointer to the played sound. (This is also returned
	// if the parameter 'startPaused' is set to true, by the way). Note that you
	// MUST call ->drop to the returned pointer if you don't need it any longer and
	// don't want to waste any memory. This is done in the end of the program.

	soundManager->play3DSound("ophelia.mp3", 0,0,0, true, false);
	Sleep(2000);
	ISound* m = soundManager->play3DSound("ophelia.mp3", 0,0,0, true, false, true);
	ISound* music = soundManager->findByName("ophelia.mp3"); m->setVolume(0);

	// the following step isn't necessary, but to adjust the distance where
	// the 3D sound can be heard, we set some nicer minimum distance
	// (the default min distance is 1, for a small object). The minimum
	// distance simply is the distance in which the sound gets played
	// at maximum volume.

	if (music) music->setMinDistance(10.0f);

	// Print some help text and start the display loop

	printf("\nPlaying streamed sound in 3D.");
	printf("\nPress ESCAPE to quit, any other key to play sound at random position.\n\n");

	printf("+ = Listener position\n");
	printf("o = Playing sound\n");

	float posOnCircle = 0;
	const float radius = 15;

	/*while (true) // endless loop until user exits
	{
		// Each step we calculate the position of the 3D music.
		// For this example, we let the
		// music position rotate on a circle:

		posOnCircle += 0.04f;
		vec3df pos3d(radius * cosf(posOnCircle), 0,
			radius * sinf(posOnCircle * 0.5f));

		// After we know the positions, we need to let irrKlang know about the
		// listener position (always position (0,0,0), facing forward in this example)
		// and let irrKlang know about our calculated 3D music position

		soundManager->getEngine()->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));

		if (music)
			music->setPosition(pos3d);

		// Now print the position of the sound in a nice way to the console
		// and also print the play position

		char stringForDisplay[] = "          +         ";
		int charpos = (int)((pos3d.X + radius) / radius * 10.0f);
		if (charpos >= 0 && charpos < 20)
			stringForDisplay[charpos] = 'o';
		int playPos = music ? music->getPlayPosition() : 0;

		printf("\rx:(%s)   3dpos: %.1f %.1f %.1f, playpos:%d:%.2d    ",
			stringForDisplay, pos3d.X, pos3d.Y, pos3d.Z,
			playPos / 60000, (playPos % 60000) / 1000);

		// Handle user input: Every time the user presses a key in the console,
		// play a random sound or exit the application if he pressed ESCAPE.
		Sleep(100);
	}*/

	SoundManager::shutdownSingleton();
	return 0;
}