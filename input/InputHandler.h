//
// Created by robin on 1/20/2021.
//

#ifndef INC_1964_INPUT_JOYSHOCKCPP_INPUTHANDLER_H
#define INC_1964_INPUT_JOYSHOCKCPP_INPUTHANDLER_H

#define SMOOTHBUFLEN 16

#include <cmath>
#include <chrono>
#include "../common.h"
#include "../settings/Settings.h"
#include "../vec2.h"

class InputHandler {
    private:

        const float flickthreshold = 0.9;
        const float flicktime = 0.1;
        const float turnsmooththreshold = 0.1;

        // Store flick state for up to 4 devices.
        float flickprogress[4] = { 0, 0, 0, 0 };
        float flicksize[4] = { 0, 0, 0, 0 };
        vec2<float> laststick[4];
        bool isflicking[4];

        float aimstickbuffer[ALLPLAYERS][SMOOTHBUFLEN];
        int bufferpos[ALLPLAYERS] = { 0 };

        Controls* _ctrlptr = Controls::getInstance();
        float getFlickState(PLAYERS player);
        float getStickLength(float stickX, float stickY);
        float WarpEaseOut(float input);
        float getDirectStickRotation(float input);
        float getSmoothedStickRotation(PLAYERS player, float input);
        float getTieredSmoothedStickRotation(PLAYERS player, float input, float threshold1, float threshold2);
        float zeroTurnSmoothing(PLAYERS player);
        float ClampFloat(const float value, const float min, const float max);

    public:
        InputHandler() = default;
        vec2<float> ProcessAimStickInputForPlayer(PLAYERS player);



};

#endif //INC_1964_INPUT_JOYSHOCKCPP_INPUTHANDLER_H
