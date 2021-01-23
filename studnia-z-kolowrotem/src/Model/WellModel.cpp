#include "WellModel.h"

#include <stdexcept>

float WellModel::getInnerRadius() const noexcept {
	return innerRadius;
}

void WellModel::setInnerRadius(float newInnerRadius) {
	innerRadius = newInnerRadius;
}

float WellModel::getOuterRadius() const noexcept {
	return outerRadius;
}

void WellModel::setOuterRadius(float newOuterRadius) {
	outerRadius = newOuterRadius;
}

float WellModel::getHeight() const noexcept {
	return height;
}

void WellModel::setHeight(float newHeight) {
	if(newHeight <= 0.0f) {
		throw std::invalid_argument{"WellModel: newHeight <= 0"};
	} else {
		height = newHeight;
	}
}

float WellModel::getWheelHeight() const noexcept {
	return wheelHeight;
}

void WellModel::setWheelHeight(float newWheelHeight) {
	wheelHeight = newWheelHeight;
}

float WellModel::getBracketRadius() const noexcept {
	return bracketRadius;
}

void WellModel::setBracketRadius(float newBracketRadius) {
	if(newBracketRadius <= 0.0f) {
		throw std::invalid_argument{"WellModel: bracketRadius <= 0"};
	} else {
		bracketRadius = newBracketRadius;
	}
}

float WellModel::getBracketHeight() const noexcept {
	return bracketHeight;
}

void WellModel::setBrackedHeight(float newBracketHeight) {
	bracketHeight = newBracketHeight;
}

void WellModel::validateModel() const {
	if(outerRadius <= innerRadius) {
		throw std::invalid_argument{"WellModel: outerRadius <= innerRadius"};
	}
}
