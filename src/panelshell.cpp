#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/math_util.hpp"

#include <linux/input-event-codes.h>

#include "interaction/xinteract.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/data/receiver.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputactionhandler.hpp>
#include <stardustxr/fusion/types/input/actions/singleactor.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/items/acceptors/panel.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

PanelItem *panel = nullptr;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root(Root(), vec3{0, -0.5f, -0.5f}, quat_identity, vec3_one, true, true, false, true);

	Model crt(&root, "../res/panelshell/crt.glb", vec3_zero, quat_from_angles(0, 180, 0));

	BoxField panelAcceptorField(&root, vec3{0, 0.23f, -0.08f}, quat_identity, vec3{0.471f, 0.46f, 0.168f});
	PanelItemAcceptor panelAcceptor(&root, panelAcceptorField, vec3_zero, quat_identity);

	Spatial screenCenter(&root, vec3{0, 0.27f, -0.004927f});
	BoxField inputField(&screenCenter, vec3_zero, quat_identity, vec3{0.4f, 0.312254f, 0.022191f});
	InputActionHandler inputHandler(&screenCenter, inputField, vec3_zero, quat_identity);
	PulseReceiver keyReceiver(&screenCenter, inputField);

	keyReceiver.setMask([&](flexbuffers::Builder &fbb) {
		fbb.String("type", "keyboard");
	});

	keyReceiver.onDataReceived = [&](std::string senderUUID, flexbuffers::Reference ref) {
		if(panel && ref.IsMap()) {
			flexbuffers::Map map = ref.AsMap();

			if(map["keymap"].IsString())
				panel->setKeymap(map["keymap"].AsString().str());
			if(map["pressed"].IsUInt())
				panel->setKeyState(map["pressed"].AsUInt32(), true);
			if(map["released"].IsUInt())
				panel->setKeyState(map["released"].AsUInt32(), false);

			if(map["modifiers"].IsMap()) {
				flexbuffers::Map modMap = map["modifiers"].AsMap();

				panel->setKeyModStates(
					modMap["depressed"].AsUInt32(),
					modMap["latched"].AsUInt32(),
					modMap["locked"].AsUInt32(),
					modMap["group"].AsUInt32());
			}
		}
	};

	float maxDistance = 0.005f;
	SingleActorAction inRangeAction;
	inRangeAction.captureOnTrigger = false;
	inRangeAction.pointerActiveCondition = [maxDistance](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
		return pointer.origin.z > 0 && pointer.distance < maxDistance;
	};
	inRangeAction.handActiveCondition = [maxDistance](const std::string uuid, const HandInput &hand, const Datamap &datamap){
		const vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
		return pinchPos.z > 0 && hand.distance < maxDistance;
	};
	inputHandler.actions.push_back(&inRangeAction);

	float scrollMultiplier = 5;
	StardustXRFusion::OnLogicStep([&](double delta, double) {
		inputHandler.update();
		if(!panel)
			return;
		if(inRangeAction.actorActing) {
			vec2 cursor = vec2_zero;
			Datamap *datamap = &inRangeAction.actor->datamap;

			PointerInput *pointer = inRangeAction.actor->pointer.get();
			if(pointer) {
				cursor.x = map(pointer->deepestPoint.x,        -0.2f,         0.2f, 0, 800);
				cursor.y = map(pointer->deepestPoint.y,  0.312254f/2, -0.312254f/2, 0, 600);
				panel->setPointerPosition(cursor);

				const float selectPressed = datamap->getFloat("select");
				panel->setPointerButtonPressed(BTN_LEFT, selectPressed > 0.9f);

				const vec2 scroll = datamap->getVec2("scroll");
				if(scroll.x != 0 && scroll.y != 0)
					panel->scrollPointerAxis(0, scroll.x * scrollMultiplier, scroll.y * scrollMultiplier, (int32_t) scroll.x, (int32_t) scroll.y);
			}

			HandInput *hand = inRangeAction.actor->hand.get();
			if(hand) {
				const vec3 pinchPos = (hand->thumb().tip().pose.position + hand->index().tip().pose.position) * 0.5f;
				cursor.x = map(pinchPos.x,        -0.2f,         0.2f, 0, 800);
				cursor.y = map(pinchPos.y,  0.312254f/2, -0.312254f/2, 0, 600);
				panel->setPointerPosition(cursor);

				const float pinchStrength = datamap->getFloat("pinchStrength");
				panel->setPointerButtonPressed(BTN_LEFT, pinchStrength > 0.9f);
			}
		}
	});

	panelAcceptor.itemCapturedMethod = [&crt](PanelItem &panelItem, PanelItem::Data data) {
		if(panel && *panel != panelItem) {
			panel->setPose(pose_t{vec3{0, -0.25f, -0.4f}, quat_identity});
			panel->release();
		}
		panel = new PanelItem(panelItem);
		panel->applySurfaceMaterial(crt, 7);
		panel->resize(800, 600);
		panel->setPointerActive(true);
		panel->setKeyboardActive(true);
	};
	panelAcceptor.itemReleasedMethod = [&](PanelItem &releasedPanel) {
		if(panel && *panel == releasedPanel)
			panel = nullptr;
	};

	StardustXRFusion::StallMainThread();
}
