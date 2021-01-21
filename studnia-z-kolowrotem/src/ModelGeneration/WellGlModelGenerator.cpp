#include "WellGlModelGenerator.h"

#include <cmath>
#include <glm/ext.hpp>
#include <iostream>

WellGlModelGenerator::WellGlModelGenerator(const WellModel& basicModel)
	: basicModel{basicModel}, sampleRate{DEFAULT_SAMPLE_RATE} {
	setupVariables();
}

void WellGlModelGenerator::setSampleRate(unsigned newSampleRate) {
	sampleRate = newSampleRate;
}

WellGlModel WellGlModelGenerator::generate() {
	basicModel.validateModel();
	setupVariables();
	createVertices();
	return WellGlModel{std::move(vertices), std::move(indices)};
}

void WellGlModelGenerator::setupVariables() {
	leftBracketGenerator.setSampleRate(sampleRate);

	innerAngle = glm::radians(360.0f / sampleRate);
	lowerInnerOffset = 0;
	lowerOuterOffset = lowerInnerOffset + sampleRate;
	higherInnerOffset = lowerOuterOffset + sampleRate;
	higherOuterOffset = higherInnerOffset + sampleRate;

	leftBracketOffset = higherOuterOffset + sampleRate;
	rightBracketOffset = leftBracketOffset + leftBracketGenerator.getVertexCount();
	logOffset = rightBracketOffset + leftBracketGenerator.getVertexCount();
}

void WellGlModelGenerator::createVertices() {
	createLowerInnerVertices();
	createLowerOuterVertices();
	createHigherInnerVertices();
	createHigherOuterVertices();
	connectOuterVertices();
	connectInnerVertices();
	connectTopVertices();
	
	prepareLeftBracketGenerator();
	prepareRightBracketGenerator();
	prepareLog();
	compoundGenerator.setArrayOffset(leftBracketOffset);
	auto [v, i] = compoundGenerator.generateModel();
	compoundGenerator.updateNormals();
	vertices.insert(vertices.end(), v.begin(), v.end());
	indices.insert(indices.end(), i.begin(), i.end());
}

void WellGlModelGenerator::createLowerInnerVertices() {
	glm::vec3 start{basicModel.getInnerRadius(), 0.0f, 0.0f};
	createCircleFrom(start);
}

void WellGlModelGenerator::createLowerOuterVertices() {
	glm::vec3 start{basicModel.getOuterRadius(), 0.0f, 0.0f};
	createCircleFrom(start);
}

void WellGlModelGenerator::createHigherInnerVertices() {
	glm::vec3 start{basicModel.getInnerRadius(), basicModel.getHeight(), 0.0f};
	createCircleFrom(start);
}

void WellGlModelGenerator::createHigherOuterVertices() {
	glm::vec3 start{basicModel.getOuterRadius(), basicModel.getHeight(), 0.0f};
	createCircleFrom(start);
}

void WellGlModelGenerator::createCircleFrom(const glm::vec3& start) {
	glm::vec3 pattern  = start;
	for(unsigned i = 0; i < sampleRate; ++i) {
		vertices.push_back(pattern);
		pattern = glm::rotateY(pattern, innerAngle);
	}
}

void WellGlModelGenerator::calculateNormals(unsigned index1, unsigned index2, unsigned index3) {
	glm::vec3 edge1;
	glm::vec3 edge2;

	edge1 = vertices[index2].position - vertices[index1].position;
	edge2 = vertices[index3].position - vertices[index1].position;

	vertices[0].normal = glm::normalize(glm::cross(edge1, edge2));
}

void WellGlModelGenerator::connectOuterVertices() {
	for(unsigned index = 0; index < sampleRate; ++index) {
		const unsigned next = nextIndex(index);
		
		indices.push_back(index + lowerInnerOffset);
		indices.push_back(next + higherInnerOffset);
		indices.push_back(next + lowerInnerOffset);
		calculateNormals(index + lowerInnerOffset, next + higherInnerOffset, next + lowerInnerOffset);
	
		indices.push_back(index + lowerInnerOffset);
		indices.push_back(next + higherInnerOffset);
		indices.push_back(index + higherInnerOffset);
		calculateNormals(index + lowerInnerOffset, next + higherInnerOffset, index + higherInnerOffset);
	}
}

void WellGlModelGenerator::connectInnerVertices() {
	for(unsigned index = 0; index < sampleRate; ++index) {
		const unsigned next = nextIndex(index);

		indices.push_back(index + lowerOuterOffset);
		indices.push_back(next + higherOuterOffset);
		indices.push_back(next + lowerOuterOffset);
		calculateNormals(index + lowerOuterOffset, next + higherOuterOffset, next + lowerOuterOffset);

		indices.push_back(index + lowerOuterOffset);
		indices.push_back(next + higherOuterOffset);
		indices.push_back(index + higherOuterOffset);
		calculateNormals(index + lowerOuterOffset, next + higherOuterOffset, index + higherOuterOffset);
	}
}

void WellGlModelGenerator::connectTopVertices() {
	for(unsigned index = 0; index < sampleRate; ++index) {
		const unsigned next = nextIndex(index);

		indices.push_back(index + higherInnerOffset);
		indices.push_back(index + higherOuterOffset);
		indices.push_back(next + higherOuterOffset);

		calculateNormals(index + higherInnerOffset, index + higherOuterOffset, next + higherOuterOffset);

		indices.push_back(index + higherInnerOffset);
		indices.push_back(next + higherInnerOffset);
		indices.push_back(next + higherOuterOffset);

		calculateNormals(index + higherInnerOffset, next + higherInnerOffset, next + higherOuterOffset);
	}
}

unsigned WellGlModelGenerator::nextIndex(unsigned index) const {
	++index;
	if(index == sampleRate) {
		index = 0;
	}
	return index;
}

void WellGlModelGenerator::prepareLeftBracketGenerator() {
	leftBracketGenerator.setRadius(basicModel.getBracketRadius());
	leftBracketGenerator.setHeight(basicModel.getHeight() + basicModel.getBracketHeight());
	leftBracketGenerator.setSampleRate(sampleRate);

	const glm::vec3 translation = {
		-(basicModel.getInnerRadius() - basicModel.getBracketRadius()),
		0.0f, 0.0
	};
	leftBracketGenerator.setTransformation(glm::translate(translation));
	compoundGenerator.addGenerator(&leftBracketGenerator);
}

void WellGlModelGenerator::prepareRightBracketGenerator() {
	rightBracketGenerator.setRadius(basicModel.getBracketRadius());
	rightBracketGenerator.setHeight(basicModel.getHeight() + basicModel.getBracketHeight());
	rightBracketGenerator.setSampleRate(sampleRate);

	const glm::vec3 translation = {
		basicModel.getInnerRadius() - basicModel.getBracketRadius(),
		0.0f, 0.0
	};
	rightBracketGenerator.setTransformation(glm::translate(translation));
	compoundGenerator.addGenerator(&rightBracketGenerator);
}

void WellGlModelGenerator::prepareLog() {
	logGenerator.setWidth(basicModel.getInnerRadius() * 2.0f);
	logGenerator.setHeight(0.2f);
	logGenerator.setLength(basicModel.getBracketRadius() * 2.0f);
	logGenerator.setTransformation(glm::translate(glm::vec3{0.0f, basicModel.getBracketHeight() + basicModel.getHeight(), 0.0f}));
	compoundGenerator.addGenerator(&logGenerator);
}
