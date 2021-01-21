//
// Created by Robin on 1/18/2021.
//

#ifndef INC_1964_INPUT_JOYSHOCKCPP_COMMON_H
#define INC_1964_INPUT_JOYSHOCKCPP_COMMON_H

#define PI 3.1415927
#define __GYRO_INJECTOR_VERSION__ "v0.1"
#define __CURRENTYEAR__ "2021"
#define CONSOLE { AllocConsole(); AttachConsole(GetCurrentProcessId()); freopen("CON", "w", stdout; )};

#define FOV_MIN 40
#define FOV_MAX 120

#include "vec2.h"


enum CONTROLLERENUM {FORWARDS = 0, BACKWARDS, STRAFELEFT, STRAFERIGHT, FIRE, AIM, ACCEPT, CANCEL, START, CROUCH, KNEEL, PREVIOUSWEAPON, NEXTWEAPON, UP, DOWN, LEFT, RIGHT, RESETGYRO, TOGGLEGYRO, TOTALBUTTONS};
enum VECTORSETTINGSENUM {AIMDEADZONE = 0, TOTALVECTORSETTINGS};
enum CONFIGENUM {CONFIG = 0, STICKSENSITIVITYX, STICKSENSITIVITYY, GYROSENSITIVITYX, GYROSENSITIVITYY, ACCELERATION, CROSSHAIR, INVERTPITCH, CROUCHTOGGLE, GEAIMMODE, PDAIMMODE, STICKMODE, STICKAIMING, TOTALSETTINGS};
enum QUICKCONFIGENUM {DISABLED = 0, DEFAULT, CUSTOM};
enum STICKMODEENUM {FULL = 0, XONLY, FLICK, ALLMODES};
enum PLAYERS { PLAYER1 = 0, PLAYER2, PLAYER3, PLAYER4, ALLPLAYERS};

enum JSD_ControllerType {
    None = 0,
    JoyconLeft = 1,
    JoyconRight = 2,
    SwitchPro = 3,
    Dualshock_4 = 4
};

typedef struct  {
    int Handle;
    JSD_ControllerType ConType;
    int DS4LightColor;
    int NSPlayerLight;
} JSDevice;

// Device is either 0 or 1, meaning that this value is assigned to device 1 or 2.
typedef struct {
    int Device;
    int Button;
} ASSIGNMENT;

typedef struct PROFILE {
    // Secondary devices are used when we are in Joycon mode.
    int ControllerMode; // -1 = Disabled, 0 = Controller, 1 = Joycon
    JSDevice AssignedDevicePrimary;
    JSDevice AssignedDeviceSecondary;
    ASSIGNMENT BUTTONPRIM[TOTALBUTTONS];
    ASSIGNMENT BUTTONSEC[TOTALBUTTONS];
    vec2<float> VECTORSETTINGS[TOTALVECTORSETTINGS];
    float FLOATSETTINGS[TOTALVECTORSETTINGS];
    int SETTINGS[TOTALSETTINGS];
} PROFILE;

typedef struct {
    vec2<float> AIMSTICK, GYRO;
    float LTRIGGER, RTRIGGER;
    int POSX, POSY; // Just in case we want kb/m support
    int BUTTONPRIM[TOTALBUTTONS];
    int BUTTONSEC[TOTALBUTTONS];
    int ARROW[4];
    bool GYROISACTIVE;
    bool CALIBRATING;
} DEVICE;

#endif //INC_1964_INPUT_JOYSHOCKCPP_COMMON_H
