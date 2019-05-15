#include "ShowLeaderBoardComponent.h"
#include <LogSystem.h>
#include <fstream>
#include <RenderSystemInterface.h>
#include <OgreTextAreaOverlayElement.h>
#include <GlobalConfig.h>

void ShowLeaderBoardComponent::display()
{
	ifstream file(jsonName);
	if (!file.is_open()) {
		LogSystem::Log("El archivo " + jsonName + "no existe aun", LogSystem::JSON);

		//crear elemento que ponga "no puntuaciones yet"
		createTextFormated("empty_lb", "Empty leaderboard", offset_X, top_distance, charHeight);
	}
	else {
		//parse it
		nap_json j;
		file >> j;
		for (int i = 0; i < j["Puntuaciones"].size(); i++) {
			int score = j["Puntuaciones"][i];
			createRowText(j["Nombres"][i], std::to_string(score), i);
		}
	}
}

void ShowLeaderBoardComponent::createRowText(std::string name, std::string score, int index)
{
	//name
	float nx = offset_X, ny = top_distance + offset_Y * index;
	auto nameText = createTextFormated(name + std::to_string(index), name, nx, ny, charHeight);

	//score
	float sx = offset_X + nameText->getWidth()*btw_distance, sy = top_distance + offset_Y * index;
	createTextFormated(score + std::to_string(index), score, sx, sy, charHeight);
}

Ogre::TextAreaOverlayElement* ShowLeaderBoardComponent::createTextFormated(std::string nodeName, std::string s, float x, float y, float h) {
	Ogre::TextAreaOverlayElement* text = rsi->createText(nodeName, s);
	text->setMetricsMode(Ogre::GMM_RELATIVE);
	rsi->addToPanel(this->cfg_["panelName"], text);
	setText(text, x, y, h);
	textNames.push_back(text->getName());
	return text;
}

void ShowLeaderBoardComponent::setText(Ogre::TextAreaOverlayElement * element, float x, float y, float charHeight)
{
	rsi->setTextPosition(element, x, y);
	rsi->setTextSize(element, charHeight);
	rsi->setTextColourTop(element, color_top.x_, color_top.y_, color_top.z_);
	rsi->setTextColourBot(element, color_bot.x_, color_bot.y_, color_bot.z_);
}

ShowLeaderBoardComponent::~ShowLeaderBoardComponent()
{
	for(std::string name : textNames) {
		rsi->destroyOverlayElement(name);
	}
}

void ShowLeaderBoardComponent::setUp() {
	if (isInited()) return;
	setInited();

	rsi = RenderSystemInterface::getSingleton();
	offset_X = this->cfg_["offset_X"];
	offset_Y = this->cfg_["offset_Y"];
	top_distance = this->cfg_["top_distance"]; //text -> top of the screen distance
	btw_distance = this->cfg_["btw_distance"]; //dist between name and score
	charHeight = this->cfg_["charHeight"];

	color_top = { this->cfg_["color_top"]["r"], this->cfg_["color_top"]["g"], this->cfg_["color_top"]["b"] };
	color_bot = { this->cfg_["color_bot"]["r"], this->cfg_["color_bot"]["g"], this->cfg_["color_bot"]["b"] };

	jsonName = GlobalCFG::paths["leaderBoard_folder"] + GlobalCFG::paths["leaderBoard_filename"];

	display();
}

#include "GOFactory.h"
REGISTER_TYPE(ShowLeaderBoardComponent);
