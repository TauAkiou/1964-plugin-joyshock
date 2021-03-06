/*
 *==========================================================================
 * Joyshock Injector - 1964 Plugin
 *==========================================================================
 * Largely based off of the original Keyboard/Mouse Injector Plugin
 * Copyright (C) 2016-2021 Carnivorous
 *
 * Joyshock Injector
 * Copyright (C) 2016-2021 TauAkiou
 * All rights reserved.
 *
 * Joyshock Injector is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, visit http://www.gnu.org/licenses/gpl-2.0.html
 *==========================================================================
 */

/*
 * game.h - Game state handling and delegation class for drivers.
 */


#ifndef INC_1964_INPUT_JOYSHOCKCPP_GAME_H
#define INC_1964_INPUT_JOYSHOCKCPP_GAME_H

#define FOV_MIN 40
#define FOV_MAX 120


#include <vector>
#include <string>
#include "driver/GameDriver.h"
#include "driver/Goldeneye.h"
#include "driver/PerfectDark.h"

class Game {
    protected:
        static inline Game* _instance;
        EmulatorLink* _link = new EmulatorLink();
        std::vector<GameDriver*> _gamedrivers = { new Goldeneye(_link), new PerfectDark(_link) };
        GameDriver* _loadedgame = nullptr;
    public:
        static Game* GetInstance();
        int Status();
        std::string Name();
        void Inject();
        void Quit();
        void AssignRamPtr(const unsigned char** ptr);
        void AssignRomPtr(const unsigned char** ptr);
};

#endif //INC_1964_INPUT_JOYSHOCKCPP_GAME_H
