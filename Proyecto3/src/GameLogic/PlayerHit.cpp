#include "PlayerHit.h"
#include "LogSystem.h"

#include "MessageSystem.h"

//at some point move to json

void PlayerHit::setUp() {
	//damage dealers
	for (auto dealer : cfg_["damageDealers"]) {
		std::string g = dealer["group"];
		int d = dealer["dmg"];
		damageDealers_.push_back(damageDeal(g, d));
	}

	inv_t.setDuration(cfg_["inv_t"]);
	configActive();
}

void PlayerHit::update(GameObject * o, double time) {
	if (inv_t.update(time)) inv = false;
}

void PlayerHit::onCollision(ID * other) {
	if (inv) return;

	//check if is a damage dealer
	for (damageDeal & dealer : damageDealers_) {
		if (other->group_ == dealer.group) {
			MessageSystem::getSingleton()->sendMessageGameObject(&Msg_HP_DAMAGE(dealer.dmg), owner_);
			MessageSystem::getSingleton()->sendMessageGroup(&Message(CHECK_HP), "manage_gameObjects");
			inv = true;
			inv_t.start();
		}
	}
}

void PlayerHit::configActive() {
	inv_t.stop();
}

#include "GOFactory.h"
REGISTER_TYPE(PlayerHit);
