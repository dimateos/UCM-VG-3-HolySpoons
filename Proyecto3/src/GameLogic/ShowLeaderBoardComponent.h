#ifndef SHOWLEADERBOARDCOMPONENTCOMPONENT_H_
#define SHOWLEADERBOARDCOMPONENTCOMPONENT_H_

#include "Component.h"

class RenderSystemInterface;
namespace Ogre {
	class TextAreaOverlayElement;
};

class ShowLeaderBoardComponent: public Component
{
private:
	std::string jsonName = "test.json";
	RenderSystemInterface* rsi = nullptr;
	float offset_X, offset_Y, charHeight, top_distance, btw_distance;
	std::vector<std::string> textNames;
	nap_vector3 color_top, color_bot;

	/*
	 *reads from json, creates and displays text
	 */
	void display();
	/*
	 *creates two textElements --> name / score
	 */
	void createText(std::string name, std::string score, int index = 0);
	/*
	 *sets x, y, charHeight of element
	 */
	void setText(Ogre::TextAreaOverlayElement* element, float x, float y, float charHeight);

public:
	virtual ~ShowLeaderBoardComponent();
	ShowLeaderBoardComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
};

#endif //SHOWLEADERBOARDCOMPONENTCOMPONENT_H_