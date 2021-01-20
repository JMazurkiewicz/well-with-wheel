#include "HollowCylinderGenerator.h"

HollowCylinderGenerator::HollowCylinderGenerator()
    : CylinderGenerator(), innerRadius{1.0f}, outerRadius{1.0f} { }

void HollowCylinderGenerator::setInnerRadius(float newInnerRadius) {
    innerRadius = newInnerRadius;
}

void HollowCylinderGenerator::setOuterRadius(float newOuterRadius) {
    outerRadius = newOuterRadius;
}

unsigned HollowCylinderGenerator::getVertexCount() const {
    return 2 * CylinderGenerator::getVertexCount();
}

unsigned HollowCylinderGenerator::getOuterCyilnderOffset() const {
    return 0;
}

unsigned HollowCylinderGenerator::getInnerCylinderOffset() const {
    return CylinderGenerator::getVertexCount();
}

void HollowCylinderGenerator::constructModel() {
    setRadius(outerRadius);
    CylinderGenerator::constructModel();

    setRadius(innerRadius);
    setArrayOffset(getInnerCylinderOffset());
    CylinderGenerator::constructModel();

    connectTop();
}

void HollowCylinderGenerator::connectTop() {
    for(unsigned index = 0; index < CylinderGenerator::getVertexCount(); ++index) {
        const unsigned next = nextIndex(index);

        indices.push_back(index + getOuterCyilnderOffset());
        indices.push_back(index + getInnerCylinderOffset());
        indices.push_back(next + getInnerCylinderOffset());

        indices.push_back(index + getOuterCyilnderOffset());
        indices.push_back(next + getOuterCyilnderOffset());
        indices.push_back(next + getInnerCylinderOffset());
    }
}

unsigned HollowCylinderGenerator::nextIndex(unsigned index) const {
    ++index;
    if(index == CylinderGenerator::getVertexCount()) {
        index = 0;
    }
    return index;
}