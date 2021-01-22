//
// Created by Robin on 1/18/2021.
//

#ifndef INC_1964_INPUT_JOYSHOCKCPP_JOYSHOCKDRIVER_H
#define INC_1964_INPUT_JOYSHOCKCPP_JOYSHOCKDRIVER_H

#include <vector>
#include <map>
#include <windows.h>
#include <ctime>
#include "../common.h"
#include "../settings/Settings.h"
#include "../JoyShockLibrary/JoyShockLibrary.h"
#include "../game/Game.h"

class JoyShockDriver {
    protected:
        static JoyShockDriver* instance;
        HANDLE _inputthread = nullptr;
        Settings* _settings = nullptr;
        ControlState* _cstateptr = nullptr;
        Game* _gameptr = nullptr;
        bool _terminatethread = true;
        int _devicecount = 0;
        int _windowactive = 1;
        int _initialized = 0;
        std::vector<JSDevice> *_devices = new std::vector<JSDevice>();
        HWND _emulatorwindow = nullptr;

        // Flick Stick state
        float flickProgress[ALLPLAYERS];
        float flicksize[ALLPLAYERS];

        static DWORD WINAPI startinjectionloop(void* param);
        DWORD injectionloop();
        float ProcessFlickStick();

        // Gyro-specific methods


    public:
        static JoyShockDriver* getInstance();
        JoyShockDriver();
        void Initialize(const HWND hw);
        void Terminate();
        void AssignEmulatorWindow(const HWND hw);

        void StartInjectionThread();
        void EndInjectionThread();
        bool IsThreadRunning();
        int GetConnectedDeviceCount() const;
        int SetPlayerHandle(PLAYERS player, int deviceclass, int phandle, int sechandle);
        std::vector<JSDevice> GetConnectedFullControllers();
        std::vector<JSDevice> GetConnectedLeftJoycons();
        std::vector<JSDevice> GetConnectedRightJoycons();
        int GetConnectedDS4Count();
        int GetConnectedSPCCount();
        int GetConnectedJCLCount();
        int GetConnectedJCRCount();
        void SetPlayerDS4Color(const int playernumber, const int color);
        void SetPlayerSPCJCNumber(const int playernumber, const int number);
        void CalibrateGyroscope(JSDevice &jsd);
        void CalibrateAllGyroscopes();
};

#endif //INC_1964_INPUT_JOYSHOCKCPP_JOYSHOCKDRIVER_H
