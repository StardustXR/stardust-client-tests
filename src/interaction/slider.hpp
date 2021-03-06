#pragma once

#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>

#include "xinteract.hpp"

class Slider : public StardustXRFusion::Spatial {
public:
	explicit Slider(Spatial *parent, float length, float minValue, float maxValue, float barThickness = 0.004f, float orbDiameter = 0.03f, StardustXRFusion::Color = StardustXRFusion::Color{0,0,0.75f,1});
	~Slider();

	void update();
	bool isActive();
	bool activeChanged();

	float maxDistance = 0.0254f;

	float value = 0.0f;
	float minValue;
	float maxValue;

	void setSliderValue(float value);
	void setSliderLength(float length);

protected:
	float length;
	float barThickness;
	float orbDiameter;

	float orbPos = 0.0f;
	void setSliderPos(float pos);

	XInteract xInteract;
	XInteract xActive;
	float pinchDistance;
	float scroll;

	StardustXRFusion::Model base;
	StardustXRFusion::Model base_inv;
	StardustXRFusion::Model orb;
	StardustXRFusion::BoxField field;
	StardustXRFusion::InputHandler inputHandler;

	bool handInput(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);
};
