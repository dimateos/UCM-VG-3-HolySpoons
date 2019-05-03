#ifndef SKYBOXCOMPONENT_H_
#define SKYBOXCOMPONENT_H_

#include "Component.h"

namespace Ogre {
	class SceneNode;
}

class SkyBoxComponent : public Component
{
private:
	std::string _skyBox;
	void updateSkyBox();
public:
	virtual ~SkyBoxComponent() {};
	SkyBoxComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	void setSkyBox(std::string nSkyBox);
	const std::string getSkyBox() const;
};

#endif //SKYBOXCOMPONENT_H_