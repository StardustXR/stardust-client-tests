#include <chrono>
#include <cstdio>
#include <iostream>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <thread>
#include <vector>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

#include "interaction/xinteract.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root = Spatial::create(vec3_forward*0.5f, quat_identity, vec3_one, true, true, false);

	float size = 0.1f;

	XInteract xInteract;

    BoxField field(vec3_zero, quat_identity, vec3_one * size);
	field.setSpatialParent(&root);
    InputHandler handler(&root, field, vec3_zero, quat_identity);
    handler.setSpatialParent(&root);
	handler.handHandlerMethod = [&xInteract](const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
		if(hand.distance > 0.0f)
			return false;
		const float pinchStrength = datamap.getFloat("pinchStrength");
		xInteract.input(pinchStrength > 0.9f);

		printf("\rXInteract is %s", xInteract.isActive() ? "active  " : "inactive");

		return xInteract.isActive();
	};

	LifeCycle()->onLogicStep([&xInteract](double delta, double) {
		xInteract.update();
		fflush(stdout);
	});

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}