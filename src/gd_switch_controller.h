#ifndef GD_SWITCH_CONTROLLER_H
#define GD_SWITCH_CONTROLLER_H

#include "switch_controller.h"
#include <hidapi/hidapi.h>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class SwitchController : public Node {
    GDCLASS(SwitchController, Node)

private:
    SwitchKit::SwitchController *controller;
    hid_device *handle;

    Vector3 accumulated_gyro = Vector3(0,0,0);
protected:
    static void _bind_methods();
public:
    SwitchController();
    ~SwitchController();

    void connect_controller();
    void poll();

    enum SwitchControllerButton {
        BTN_A,
        BTN_B,
        BTN_X,
        BTN_Y,

        BTN_SR,
        BTN_SL,
        BTN_R,
        BTN_ZR,

        BTN_MINUS,
        BTN_PLUS,
        BTN_STICK_R,
        BTN_STICK_L,
        BTN_HOME,
        BTN_CAPTURE,
        BTN_CHARGING_GRIP,

        BTN_DOWN,
        BTN_UP,
        BTN_RIGHT,
        BTN_LEFT,
        BTN_L,
        BTN_ZL
    };
    bool get_button(SwitchControllerButton p_button) const;

    enum SwitchControllerStick {
        STICK_LEFT,
        STICK_RIGHT
    };
    Vector2 get_stick(SwitchControllerStick stick) const;

    Vector3 get_accel() const;
    Vector3 get_gyro() const;

    enum BatteryLevel {
        BATTERY_FULL = 8,
        BATTERY_MEDIUM = 6,
        BATTERY_LOW = 4,
        BATTERY_CRITICAL = 2,
        BATTERY_EMPTY = 0
    };
    SwitchController::BatteryLevel get_battery_level() const;
    bool get_battery_charging() const;
    enum ControllerType {
        CONTROLLER_L = 1,
        CONTROLLER_R = 2,
        CONTROLLER_PRO = 3
    };
    ControllerType get_controller_type() const;

    void set_imu_enabled(bool enabled);
    void set_mcu_enabled(bool enabled);

    void request_device_info();
    void request_stick_calibration();
    void request_color_data();

    enum PlayerLightStatus {
        LIGHT_OFF = 0x00,
        LIGHT_ON = 0x01,
        LIGHT_FLASH = 0x10
    };
    void set_player_lights(PlayerLightStatus p1, PlayerLightStatus p2, PlayerLightStatus p3, PlayerLightStatus p4);

    void rumble(const Dictionary &p_rumble) const;

    enum ColorRole {
        COLOR_BODY,
        COLOR_BUTTON,
        COLOR_LEFT_GRIP,
        COLOR_RIGHT_GRIP
    };
    Color get_color(ColorRole role) const;

    // Ring-Con methods
    void enable_ringcon();
    float get_ringcon_flex();
};

}

VARIANT_ENUM_CAST(SwitchController::SwitchControllerButton);
VARIANT_ENUM_CAST(SwitchController::SwitchControllerStick);
VARIANT_ENUM_CAST(SwitchController::BatteryLevel);
VARIANT_ENUM_CAST(SwitchController::ControllerType);
VARIANT_ENUM_CAST(SwitchController::PlayerLightStatus);
VARIANT_ENUM_CAST(SwitchController::ColorRole);

#endif