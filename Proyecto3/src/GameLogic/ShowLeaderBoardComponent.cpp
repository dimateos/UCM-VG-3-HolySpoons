#include "ShowLeaderBoardComponent.h"
#include <LogSystem.h>
#include <fstream>
#include <RenderSystemInterface.h>
#include <OgreTextAreaOverlayElement.h>

void ShowLeaderBoardComponent::display()
{
	ifstream file(jsonName);
	if (!file.is_open()) {
		LogSystem::Log("El archivo " + jsonName + "no existe aun", LogSystem::JSON);

		//crear elemento que ponga "no puntuaciones yet"
	}
	else {
		//parse it
		nap_json j;
		file >> j;
		for (int i = 0; i < j["Puntuaciones"].size(); i++) {
			int score = j["Puntuaciones"][i];
			createText(j["Nombres"][i], std::to_string(score), i);
		}
	}
}

void ShowLeaderBoardComponent::createText(std::string name, std::string score, int index)
{
	//name
	Ogre::TextAreaOverlayElement* nameText = rsi->createText(name + std::to_string(index), name);
	nameText->setMetricsMode(Ogre::GMM_RELATIVE);
	rsi->addToPanel(this->cfg_["panelName"], nameText);
	setText(nameText, offset_X, top_distance + offset_Y * index, charHeight);
	textNames.push_back(nameText->getName());

	//score
	Ogre::TextAreaOverlayElement* scoreText = rsi->createText(score + std::to_string(index), score);
	scoreText->setMetricsMode(Ogre::GMM_RELATIVE);
	rsi->addToPanel(this->cfg_["panelName"], scoreText);
	setText(scoreText, offset_X + nameText->getWidth()*btw_distance, top_distance + offset_Y * index, charHeight);
	textNames.push_back(scoreText->getName());
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

	display();
}

#include "GOFactory.h"
REGISTER_TYPE(ShowLeaderBoardComponent);
