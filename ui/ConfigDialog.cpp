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

#include "ConfigDialog.h"

void ConfigDialog::_processPrimaryLayout(int value) {
    if (!_locked) {
        _locked = true;
        _mapButtonToCommand(static_cast<CONTROLLERENUM>(value), false);
        _locked = false;
    }
}

void ConfigDialog::_processSecondaryLayout(int value) {
    if (!_locked) {
        _locked = true;
        _mapButtonToCommand(static_cast<CONTROLLERENUM>(value), true);
        _locked = false;

    }

}

void ConfigDialog::_createPrimaryButtonLayouts() {
    auto vlayout = new QVBoxLayout(_configform->scrollAreaWidgetContents);
    _configform->scrollAreaWidgetContents->setLayout(vlayout);

    for(int entry = 0; entry < TOTALBUTTONS; entry++) {
        auto layout = new QHBoxLayout();
        layout->setSpacing(6);
        auto label = new QLabel(_getNameFromButtonIndex(static_cast<CONTROLLERENUM>(entry)));
        auto sizepolicy = QSizePolicy();
        sizepolicy.setVerticalPolicy(QSizePolicy::Preferred);
        sizepolicy.setHorizontalPolicy(QSizePolicy::Minimum);
        label->setSizePolicy(sizepolicy);
        label->setMinimumWidth(120);
        layout->addWidget(label);
        layout->addWidget(_mappingButtonListPrimary[entry]);
        layout->addWidget(_mappingButtonListSecondary[entry]);
        vlayout->addLayout(layout);
    }
}

QString ConfigDialog::_getNameFromButtonIndex(CONTROLLERENUM index) {
    switch(index) {
        case FIRE:
            return QString::fromStdString("Fire");
        case AIM:
            return QString::fromStdString("Aim");
        case ACCEPT:
            return QString::fromStdString("Next Weapon/Accept");
        case CANCEL:
            return QString::fromStdString("Reload/Use/Cancel");
        case START:
            return QString::fromStdString("Start");
        case CROUCH:
            return QString::fromStdString("Crouch");
        case KNEEL:
            return QString::fromStdString("Kneel (Perfect Dark)");
        case PREVIOUSWEAPON:
            return QString::fromStdString("Previous Weapon");
        case NEXTWEAPON:
            return QString::fromStdString("Next Weapon");
        case FORWARDS:
            return QString::fromStdString("Forwards");
        case BACKWARDS:
            return QString::fromStdString("Backwards");
        case STRAFELEFT:
            return QString::fromStdString("Strafe Left");
        case STRAFERIGHT:
            return QString::fromStdString("Strafe Right");
        case UP:
            return QString::fromStdString("Analog Up");
        case DOWN:
            return QString::fromStdString("Analog Down");
        case LEFT:
            return QString::fromStdString("Analog Left");
        case RIGHT:
            return QString::fromStdString("Analog Right");
        case RESETGYRO:
            return QString::fromStdString("Center Viewpoint");
        case TOGGLEGYRO:
            return QString::fromStdString("Toggle Gyroscope");
        case CALIBRATEGYRO:
            return QString::fromStdString("Calibrate Gyroscope");
        case TOTALBUTTONS:
        default:
            return QString::fromStdString("Invalid");
    }
}

bool ConfigDialog::GetConfigDialogState() {
    return _configdialogisopen;
}

void ConfigDialog::_loadDevicesIntoDeviceBox(CONTROLLERMODE mode) {
    int deviceno = 0;
    switch(mode) {
        case FULLCONTROLLER:
        // Disable and clear secondary device box.
        _configform->secondaryDeviceBox->clear();
        _configform->secondaryDeviceBox->setDisabled(true);

        _configform->primaryDeviceBox->clear();
        _configform->primaryDeviceBox->setEnabled(true);

        for(JSDevice dev : _loadedfull) {
            auto strbuild = std::stringstream();
            deviceno++;
            strbuild << deviceno << ": " << _jsdriver->GetNameOfDevice(dev);
            _configform->primaryDeviceBox->addItem(
                    QString::fromStdString(strbuild.str()));
        }
        break;
        case JOYCONS:
            // Disable and clear secondary device box.
            _configform->secondaryDeviceBox->clear();
            _configform->secondaryDeviceBox->setEnabled(true);

            _configform->primaryDeviceBox->clear();
            _configform->primaryDeviceBox->setEnabled(true);

            for(JSDevice dev : _loadedjoyconprimary) {
                auto strbuild = std::stringstream();
                deviceno++;
                strbuild << deviceno << ": " << _jsdriver->GetNameOfDevice(dev);
                _configform->primaryDeviceBox->addItem(
                        QString::fromStdString(strbuild.str()));
            }

            deviceno = 0;
            for(JSDevice dev : _loadedjoyconsecondary) {
                auto strbuild = std::stringstream();
                deviceno++;
                strbuild << deviceno << ": " << _jsdriver->GetNameOfDevice(dev);
                _configform->secondaryDeviceBox->addItem(
                        QString::fromStdString(strbuild.str()));
            }
            break;
        case DISCONNECTED:
        default:
            _configform->secondaryDeviceBox->clear();
            _configform->secondaryDeviceBox->setEnabled(false);

            _configform->primaryDeviceBox->clear();
            _configform->primaryDeviceBox->setEnabled(false);
    }
}

ConfigDialog::ConfigDialog(QDialog *parent) : QDialog(parent), _configform(new Ui::ConfigDialog) {
    _configform->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    _getCurrentConfigState();

    // Dynamically generate all mapper buttons and assign them to the ScrollBox
    auto *mapperPrimary = new QSignalMapper(this);
    auto *mapperSecondary = new QSignalMapper(this);
    connect(mapperPrimary, SIGNAL(mapped(int)), this, SIGNAL(primaryClicked(int)));
    connect(mapperSecondary, SIGNAL(mapped(int)), this, SIGNAL(secondaryClicked(int)));

    for(int index = 0; index < TOTALBUTTONS; index++) {
        auto primaryButton = new QPushButton(this);
        auto secondaryButton = new QPushButton(this);
        primaryButton->setText(QString::fromStdString("None"));
        secondaryButton->setText(QString::fromStdString("None"));
        _mappingButtonListPrimary.append(primaryButton);
        _mappingButtonListSecondary.append(secondaryButton);
        mapperPrimary->setMapping(primaryButton, index);
        mapperSecondary->setMapping(secondaryButton, index);

        connect(primaryButton, SIGNAL(clicked()), mapperPrimary, SLOT(map()));
        connect(secondaryButton, SIGNAL(clicked()), mapperSecondary, SLOT(map()));

    }

    connect(this, SIGNAL(primaryClicked(int)), this, SLOT(_processPrimaryLayout(int)));
    connect(this, SIGNAL(secondaryClicked(int)), this, SLOT(_processSecondaryLayout(int)));

    _createPrimaryButtonLayouts();

    _loadedfull = _jsdriver->GetConnectedFullControllers();
    _loadedjoyconprimary = _jsdriver->GetConnectedLeftJoycons();
    _loadedjoyconsecondary = _jsdriver->GetConnectedRightJoycons();
    _selectedplayer = PLAYER1;

    _configform->controllerModeBox->setCurrentIndex(_localassignments[_selectedplayer].ControllerMode);

    _loadDevicesIntoDeviceBox(_localassignments[_selectedplayer].ControllerMode);
    _loadMappingsIntoUi(_localprofiles[_selectedplayer], _localassignments[_selectedplayer]);
}

void ConfigDialog::_getCurrentConfigState() {
    for(int player = PLAYER1; player < ALLPLAYERS; player++) {
        _localprofiles[player] = _settingsptr->GetProfileForPlayer(static_cast<PLAYERS>(player));
        _localassignments[player] = _settingsptr->GetAssignmentForPlayer(static_cast<PLAYERS>(player));
    }
}

void ConfigDialog::_loadMappingsIntoUi(PROFILE &profile, Assignment &asgn) {

    // Only use the primary device. We merge the joycon input into one mask anyway, so it doesn't matter what controller
    // we 'read' from.

    for(int index = 0; index < TOTALBUTTONS; index++) {
        _mappingButtonListPrimary[index]->setText(QString::fromStdString(JoyShockDriver::GetButtonLabelForController(asgn.PrimaryDevice, profile.BUTTONPRIM[index])));
        _mappingButtonListSecondary[index]->setText(QString::fromStdString(JoyShockDriver::GetButtonLabelForController(asgn.SecondaryDevice, profile.BUTTONSEC[index])));
    }
}

ConfigDialog::~ConfigDialog() {
    delete _configform;
}

// -----------------------------------------------------------------------------------------------
// Slots & Signals
// -----------------------------------------------------------------------------------------------

void ConfigDialog::on_cancelButton_clicked() {
    this->close();
}

void ConfigDialog::on_primaryDeviceBox_currentIndexChanged(int index) {
    auto cmode = _localassignments[_selectedplayer].ControllerMode;

    // bounds checking
    switch(cmode) {
        case DISCONNECTED:
        default:
            return;
        case FULLCONTROLLER:
            if (index >= _loadedfull.size() || index < 0) {
                return;
            }
            _localassignments[_selectedplayer].PrimaryDevice = _loadedfull[index];
            _loadMappingsIntoUi(_localprofiles[_selectedplayer], _localassignments[_selectedplayer]);
            break;
        case JOYCONS:
            if (index >= _loadedjoyconprimary.size() || index < 0) {
                return;
            }
            _localassignments[_selectedplayer].PrimaryDevice = _loadedjoyconprimary[index];
            _loadMappingsIntoUi(_localprofiles[_selectedplayer], _localassignments[_selectedplayer]);
            break;
    }
}

void ConfigDialog::on_secondaryDeviceBox_currentIndexChanged(int index) {
    auto cmode = _localassignments[_selectedplayer].ControllerMode;

    // bounds checking
    switch(cmode) {
        case DISCONNECTED:
        case FULLCONTROLLER:
        default:
            return;
        case JOYCONS:
            if (index >= _loadedjoyconsecondary.size() || index < 0)  {
                return;
            }
            _localassignments[_selectedplayer].SecondaryDevice = _loadedjoyconsecondary[index];
            _loadMappingsIntoUi(_localprofiles[_selectedplayer], _localassignments[_selectedplayer]);
            break;
    }
}


void ConfigDialog::on_controllerModeBox_currentIndexChanged(int index) {
    switch(index) {
        default:
        case DISCONNECTED:
            _loadDevicesIntoDeviceBox(DISCONNECTED);
            _localassignments[_selectedplayer].ControllerMode = DISCONNECTED;
            _localassignments[_selectedplayer].PrimaryDevice = {-1, None};
            _localassignments[_selectedplayer].SecondaryDevice = {-1, None};
            _loadMappingsIntoUi(_localprofiles[_selectedplayer],  _localassignments[_selectedplayer]);
            break;
        case FULLCONTROLLER:
            _loadDevicesIntoDeviceBox(FULLCONTROLLER);
            _localassignments[_selectedplayer].ControllerMode = FULLCONTROLLER;
            _localassignments[_selectedplayer].PrimaryDevice = _loadedfull.front(); // Always select the first con.
            _loadMappingsIntoUi(_localprofiles[_selectedplayer],  _localassignments[_selectedplayer]);
            break;
        case JOYCONS:
            _loadDevicesIntoDeviceBox(JOYCONS);
            _localassignments[_selectedplayer].ControllerMode = JOYCONS;
            _localassignments[_selectedplayer].PrimaryDevice = _loadedjoyconprimary.front(); // Always select the first con.
            _localassignments[_selectedplayer].SecondaryDevice = _loadedjoyconsecondary.front();
            _loadMappingsIntoUi(_localprofiles[_selectedplayer],  _localassignments[_selectedplayer]);
            break;
    }
}

// insane boilerplate for linking all of the sliders to their respective textboxes...
void ConfigDialog::on_playerSettingsTabGyroXAxisSensitivitySlider_valueChanged(int value) {
    _configform->playerSettingsTabGyroXAxisSensitivitySpinbox->setValue(value / 100.0);
    _localprofiles[_selectedplayer].GyroscopeSensitivity.x = (value / 100.0);
}

void ConfigDialog::on_playerSettingsTabGyroXAxisSensitivitySpinbox_valueChanged(double value) {
    _configform->playerSettingsTabGyroXAxisSensitivitySlider->setValue(value * 100);
    _localprofiles[_selectedplayer].GyroscopeSensitivity.x = (float)value;
}

void ConfigDialog::on_playerSettingsTabGyroYAxisSensitivitySlider_valueChanged(int value) {
    _configform->playerSettingsTabGyroYAxisSensitivitySpinbox->setValue(value / 100.0);
    _localprofiles[_selectedplayer].GyroscopeSensitivity.y = value / 100.0;

}

void ConfigDialog::on_playerSettingsTabGyroYAxisSensitivitySpinbox_valueChanged(double value) {
    _configform->playerSettingsTabGyroYAxisSensitivitySlider->setValue(value * 100);
    _localprofiles[_selectedplayer].GyroscopeSensitivity.y = (float)value;

}

void ConfigDialog::on_reconnectControllers_clicked() {
    _jsdriver->ReconnectControllers();
    _loadedfull = _jsdriver->GetConnectedFullControllers();
    _loadedjoyconprimary = _jsdriver->GetConnectedLeftJoycons();
    _loadedjoyconsecondary = _jsdriver->GetConnectedRightJoycons();
    _getCurrentConfigState();

    on_primaryDeviceBox_currentIndexChanged(0);
    on_secondaryDeviceBox_currentIndexChanged(0);
}

void ConfigDialog::_mapButtonToCommand(CONTROLLERENUM command, bool isSecondary) {
    _locked = true;
    auto asgn = _localassignments[_selectedplayer];
    auto start = std::chrono::steady_clock::now();
    auto now = start;
    QList<QPushButton*> buttonlist;

    if(!isSecondary)
        buttonlist = _mappingButtonListPrimary;
    else
        buttonlist = _mappingButtonListSecondary;

    if (asgn.ControllerMode == DISCONNECTED) {
        return;
    }

    std::chrono::duration<float> dur = start - now;
    buttonlist[command]->setText(QString::fromStdString("5..."));
    while (dur.count() < 5.0f) {
        QApplication::processEvents();
        if (asgn.ControllerMode == FULLCONTROLLER) {
            auto btn = JoyShockDriver::GetFirstButtonFromDevice(asgn.PrimaryDevice);
            if (btn != 0) {

                if(isSecondary) _localprofiles[_selectedplayer].BUTTONSEC[command] = btn;
                else _localprofiles[_selectedplayer].BUTTONPRIM[command] = btn;

                buttonlist[command]->setText(QString::fromStdString(
                        JoyShockDriver::GetButtonLabelForController(asgn.PrimaryDevice, btn)));
                return;
            }
        }
        else if(asgn.ControllerMode == JOYCONS) {
            auto btn_primary = JoyShockDriver::GetFirstButtonFromDevice(asgn.PrimaryDevice);
            auto btn_secondary = JoyShockDriver::GetFirstButtonFromDevice(asgn.SecondaryDevice);
            auto btn = btn_primary & btn_secondary;

            if (btn != 0) {

                if(isSecondary) _localprofiles[_selectedplayer].BUTTONSEC[command] = btn;
                else _localprofiles[_selectedplayer].BUTTONPRIM[command] = btn;

                buttonlist[command]->setText(QString::fromStdString(
                        JoyShockDriver::GetButtonLabelForController(asgn.PrimaryDevice, btn)));
                return;
            }
        }
            if (dur.count() > 4.0f)
                buttonlist[command]->setText(QString::fromStdString("1..."));
            else if (dur.count() > 3.0f)
                buttonlist[command]->setText(QString::fromStdString("2..."));
            else if (dur.count() > 2.0f)
                buttonlist[command]->setText(QString::fromStdString("3..."));
            else if (dur.count() > 1.0f)
                buttonlist[command]->setText(QString::fromStdString("4..."));

            now = std::chrono::steady_clock::now();
            dur = now - start;
    }
    if(isSecondary) _localprofiles[_selectedplayer].BUTTONSEC[command] = 0;
    else _localprofiles[_selectedplayer].BUTTONPRIM[command] = 0;
    buttonlist[command]->setText(
            QString::fromStdString(JoyShockDriver::GetButtonLabelForController(asgn.PrimaryDevice, -1)));
}