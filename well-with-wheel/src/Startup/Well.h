#pragma once

#include "Controller/WheelController.h"
#include "GlModel/GlModel.h"
#include "Graphics/Texture.h"
#include "Model/RoofModel.h"
#include "Model/WellModel.h"
#include "Model/WheelModel.h"
#include "System/Stopwatch.h"
#include "System/Window.h"
#include "View/DynamicGlView.h"
#include "View/GlView.h"
#include "View/StaticGlView.h"

class Well {
public:
    explicit Well(Window& window);
    Well(const Well&) = delete;
    Well& operator=(const Well&) = delete;

    void update();

private:
    void create();
    void createBase();
    void createWoodenStand();
    void createWheel();
    void createRoof();

    WellModel basicModel;
    WheelModel basicWheelModel;
    RoofModel basicRoofModel;

    GlModel woodenStandModel;
    StaticGlView woodenStandView;

    GlModel baseModel;
    StaticGlView baseView;

    GlModel wheelModel;
    DynamicGlView wheelView;
    WheelController wheelController;

    GlModel roofModel;
    StaticGlView roofView;

    Stopwatch stopwatch;

    Texture stone;
    Texture wood;
    Texture plank;
};
