#pragma once

#include "Model/WellGlModel.h"
#include "Model/WellModel.h"

/**
 * @brief Class used to generate well OpenGl model from basic model.
 */
class WellGlModelGenerator {
public:
	static constexpr unsigned DEFAULT_SAMPLE_RATE = 64;

	explicit WellGlModelGenerator(const WellModel& basicModel);
	WellGlModelGenerator(const WellGlModelGenerator&) = delete;
	WellGlModelGenerator& operator=(const WellGlModelGenerator&) = delete;

	void setSampleRate(unsigned newSampleRate);

	[[nodiscard]] WellGlModel generate();

private:
	void setupVariables();

	void createVertices();
	void createLowerInnerVertices();
	void createLowerOuterVertices();
	void createHigherInnerVertices();
	void createHigherOuterVertices();
	void createCircleFrom(const glm::vec3& start);
	void calculateNormals(unsigned index1, unsigned index2, unsigned index3);

	void connectOuterVertices();
	void connectInnerVertices();
	void connectTopVertices();
	unsigned nextIndex(unsigned index) const;

	const WellModel& basicModel;

	unsigned sampleRate;
	WellGlModel::Vertices vertices;
	WellGlModel::Indices indices;
	WellGlModel::Normals normals;

	float innerAngle;
	unsigned lowerInnerOffset;
	unsigned lowerOuterOffset;
	unsigned higherInnerOffset;
	unsigned higherOuterOffset;
};
