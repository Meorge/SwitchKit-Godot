#include "gd_switch_controller.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void SwitchController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("connect_controller"), &SwitchController::connect_controller);
    ClassDB::bind_method(D_METHOD("poll"), &SwitchController::poll);
    ClassDB::bind_method(D_METHOD("get_button", "p_button"), &SwitchController::get_button);
    ClassDB::bind_method(D_METHOD("get_button_pressed_this_frame", "p_button"), &SwitchController::get_button_pressed_this_frame);
    ClassDB::bind_method(D_METHOD("get_stick", "p_stick"), &SwitchController::get_stick);
    ClassDB::bind_method(D_METHOD("get_accel"), &SwitchController::get_accel);
    ClassDB::bind_method(D_METHOD("get_gyro"), &SwitchController::get_gyro);

    ClassDB::bind_method(D_METHOD("get_battery_level"), &SwitchController::get_battery_level);
    ClassDB::bind_method(D_METHOD("get_battery_charging"), &SwitchController::get_battery_charging);
    ClassDB::bind_method(D_METHOD("get_controller_type"), &SwitchController::get_controller_type);
    ClassDB::bind_method(D_METHOD("get_external_device"), &SwitchController::get_external_device);

    ClassDB::bind_method(D_METHOD("set_imu_enabled", "p_enabled"), &SwitchController::set_imu_enabled);
    ClassDB::bind_method(D_METHOD("set_mcu_enabled", "p_enabled"), &SwitchController::set_mcu_enabled);


    ClassDB::bind_method(D_METHOD("request_device_info"), &SwitchController::request_device_info);
    ClassDB::bind_method(D_METHOD("request_stick_calibration"), &SwitchController::request_stick_calibration);
    ClassDB::bind_method(D_METHOD("request_color_data"), &SwitchController::request_color_data);

    ClassDB::bind_method(D_METHOD("set_player_lights", "p_p1", "p_p2", "p_p3", "p_p4"), &SwitchController::set_player_lights);

    ClassDB::bind_method(D_METHOD("rumble", "p_rumble"), &SwitchController::rumble);

    ClassDB::bind_method(D_METHOD("get_color", "p_role"), &SwitchController::get_color);

    ClassDB::bind_method(D_METHOD("enable_ringcon"), &SwitchController::enable_ringcon);
    ClassDB::bind_method(D_METHOD("disable_ringcon"), &SwitchController::disable_ringcon);
    ClassDB::bind_method(D_METHOD("get_ringcon_connected"), &SwitchController::get_ringcon_connected);
    ClassDB::bind_method(D_METHOD("get_ringcon_enabled"), &SwitchController::get_ringcon_enabled);
    ClassDB::bind_method(D_METHOD("get_ringcon_flex"), &SwitchController::get_ringcon_flex);

    BIND_ENUM_CONSTANT(BTN_A);
    BIND_ENUM_CONSTANT(BTN_B);
    BIND_ENUM_CONSTANT(BTN_X);
    BIND_ENUM_CONSTANT(BTN_Y);

    BIND_ENUM_CONSTANT(BTN_SR);
    BIND_ENUM_CONSTANT(BTN_SL);
    BIND_ENUM_CONSTANT(BTN_R);
    BIND_ENUM_CONSTANT(BTN_ZR);

    BIND_ENUM_CONSTANT(BTN_MINUS);
    BIND_ENUM_CONSTANT(BTN_PLUS);
    BIND_ENUM_CONSTANT(BTN_STICK_R);
    BIND_ENUM_CONSTANT(BTN_STICK_L);
    BIND_ENUM_CONSTANT(BTN_HOME);
    BIND_ENUM_CONSTANT(BTN_CAPTURE);
    BIND_ENUM_CONSTANT(BTN_CHARGING_GRIP);

    BIND_ENUM_CONSTANT(BTN_DOWN);
    BIND_ENUM_CONSTANT(BTN_UP);
    BIND_ENUM_CONSTANT(BTN_RIGHT);
    BIND_ENUM_CONSTANT(BTN_LEFT);
    BIND_ENUM_CONSTANT(BTN_L);
    BIND_ENUM_CONSTANT(BTN_ZL);
    
    BIND_ENUM_CONSTANT(STICK_LEFT);
    BIND_ENUM_CONSTANT(STICK_RIGHT);

    BIND_ENUM_CONSTANT(BATTERY_FULL);
    BIND_ENUM_CONSTANT(BATTERY_MEDIUM);
    BIND_ENUM_CONSTANT(BATTERY_LOW);
    BIND_ENUM_CONSTANT(BATTERY_CRITICAL);
    BIND_ENUM_CONSTANT(BATTERY_EMPTY);

    BIND_ENUM_CONSTANT(CONTROLLER_L);
    BIND_ENUM_CONSTANT(CONTROLLER_R);
    BIND_ENUM_CONSTANT(CONTROLLER_PRO);

    BIND_ENUM_CONSTANT(EXT_NONE);
    BIND_ENUM_CONSTANT(EXT_RINGCON);
    BIND_ENUM_CONSTANT(EXT_STARLINK);

    BIND_ENUM_CONSTANT(LIGHT_OFF);
    BIND_ENUM_CONSTANT(LIGHT_ON);
    BIND_ENUM_CONSTANT(LIGHT_FLASH);

    BIND_ENUM_CONSTANT(COLOR_BODY);
    BIND_ENUM_CONSTANT(COLOR_BUTTON);
    BIND_ENUM_CONSTANT(COLOR_LEFT_GRIP);
    BIND_ENUM_CONSTANT(COLOR_RIGHT_GRIP);
}

SwitchController::SwitchController() {}
SwitchController::~SwitchController() {
    // TODO: close handle?
    delete controller;
}

void SwitchController::connect_controller() {
    int res = hid_init();
    if (res == -1) {
        print_error(vformat("Error initializing hidapi: %s", hid_error(NULL)));
        return;
    }

    handle = hid_open(0x57E, 0x2007, NULL); // for Joy-Con (R)

    if (handle == nullptr) {
        print_error(vformat("Error opening handle for device: %s", hid_error(NULL)));
        return;
    }

    controller = new SwitchKit::SwitchController(handle);
	controller->set_input_report_mode(SwitchKit::InputReportMode::MODE_STANDARD);
    controller->request_device_info();
    controller->request_stick_calibration();
    controller->request_imu_calibration();
    controller->request_color_data();
}

void SwitchController::poll() {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->poll();

    SwitchKit::Vector3 raw_gyro = controller->get_gyro();
    accumulated_gyro = Vector3(raw_gyro.y, -raw_gyro.z, -raw_gyro.x);
}

bool SwitchController::get_button(SwitchControllerButton p_button) const {
    ERR_FAIL_COND_V(handle == NULL, false);
    ERR_FAIL_COND_V(controller == nullptr, false);
    return controller->get_button((SwitchKit::SwitchControllerReport::Button)p_button);
}

bool SwitchController::get_button_pressed_this_frame(SwitchControllerButton p_button) const {
    ERR_FAIL_COND_V(handle == NULL, false);
    ERR_FAIL_COND_V(controller == nullptr, false);
    return controller->get_button_pressed_this_frame((SwitchKit::SwitchControllerReport::Button)p_button);
}

Vector2 SwitchController::get_stick(SwitchControllerStick stick) const {
    ERR_FAIL_COND_V(handle == NULL, Vector2());
    ERR_FAIL_COND_V(controller == NULL, Vector2());
    SwitchKit::Vector2 raw_stick = controller->get_stick((SwitchKit::SwitchController::Stick)stick);
    return Vector2(raw_stick.x, raw_stick.y);
}

Vector3 SwitchController::get_accel() const {
    ERR_FAIL_COND_V(handle == NULL, Vector3());
    ERR_FAIL_COND_V(controller == NULL, Vector3());
    SwitchKit::Vector3 raw_acc = controller->get_accel();
    return Vector3(raw_acc.x, raw_acc.y, raw_acc.z);
}

Vector3 SwitchController::get_gyro() const {
    ERR_FAIL_COND_V(handle == NULL, Vector3());
    ERR_FAIL_COND_V(controller == NULL, Vector3());
    return accumulated_gyro;
}

SwitchController::BatteryLevel SwitchController::get_battery_level() const {
    ERR_FAIL_COND_V(handle == NULL, BATTERY_EMPTY);
    ERR_FAIL_COND_V(controller == nullptr, BATTERY_EMPTY);
    return (SwitchController::BatteryLevel)controller->get_battery_level();
}

bool SwitchController::get_battery_charging() const {
    ERR_FAIL_COND_V(handle == NULL, false);
    ERR_FAIL_COND_V(controller == nullptr, false);
    return controller->get_battery_charging();
}

SwitchController::ControllerType SwitchController::get_controller_type() const {
    ERR_FAIL_COND_V(handle == NULL, CONTROLLER_L);
    ERR_FAIL_COND_V(controller == nullptr, CONTROLLER_L);
    return (SwitchController::ControllerType)controller->get_controller_type();
}

SwitchController::ExternalDevice SwitchController::get_external_device() const {
    ERR_FAIL_COND_V(handle == NULL, EXT_NONE);
    ERR_FAIL_COND_V(controller == nullptr, EXT_NONE);
    return (SwitchController::ExternalDevice)controller->get_external_device_id();
}

void SwitchController::set_imu_enabled(bool enabled) {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->set_imu_enabled(enabled);
}

void SwitchController::set_mcu_enabled(bool enabled) {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->set_mcu_enabled(enabled);
}

void SwitchController::request_device_info() {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->request_device_info();
}

void SwitchController::request_stick_calibration() {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->request_stick_calibration();
}

void SwitchController::request_color_data() {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->request_color_data();
}

void SwitchController::set_player_lights(PlayerLightStatus p1, PlayerLightStatus p2, PlayerLightStatus p3, PlayerLightStatus p4) {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->set_player_lights(
        (SwitchKit::SwitchController::PlayerLight)p1,
        (SwitchKit::SwitchController::PlayerLight)p2,
        (SwitchKit::SwitchController::PlayerLight)p3,
        (SwitchKit::SwitchController::PlayerLight)p4
    );
}

void SwitchController::rumble(const Dictionary &p_rumble) const {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);

    Variant left_high_freq = p_rumble.get("left_high_freq", 320.0);
    Variant left_high_amp = p_rumble.get("left_high_amp", 0.0);

    Variant left_low_freq = p_rumble.get("left_low_freq", 160.0);
    Variant left_low_amp = p_rumble.get("left_low_amp", 0.0);

    Variant right_high_freq = p_rumble.get("right_high_freq", 320.0);
    Variant right_high_amp = p_rumble.get("right_high_amp", 0.0);

    Variant right_low_freq = p_rumble.get("right_low_freq", 160.0);
    Variant right_low_amp = p_rumble.get("right_low_amp", 0.0);

    SwitchKit::HDRumbleConfig config;
    config.left.high.frequency = left_high_freq;
    config.left.high.amplitude = left_high_amp;

    config.left.low.frequency = left_low_freq;
    config.left.low.amplitude = left_low_amp;

    config.right.high.frequency = right_high_freq;
    config.right.high.amplitude = right_high_amp;

    config.right.low.frequency = right_low_freq;
    config.right.low.amplitude = right_low_amp;

    controller->rumble(config);
}

Color SwitchController::get_color(ColorRole role) const {
    ERR_FAIL_COND_V(handle == NULL, Color());
    ERR_FAIL_COND_V(controller == nullptr, Color());
    SwitchKit::Color24 c = controller->get_color((SwitchKit::SwitchController::ColorRole)role);
    return Color(
        (double)c.r / 255.0,
        (double)c.g / 255.0,
        (double)c.b / 255.0
    );
}

void SwitchController::enable_ringcon() {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->enable_ringcon();
}

void SwitchController::disable_ringcon() {
    ERR_FAIL_COND(handle == NULL);
    ERR_FAIL_COND(controller == nullptr);
    controller->disable_ringcon();
}

bool SwitchController::get_ringcon_connected() {
    ERR_FAIL_COND_V(handle == NULL, false);
    ERR_FAIL_COND_V(controller == nullptr, false);
    return controller->get_ringcon_connected();
}

bool SwitchController::get_ringcon_enabled() {
    ERR_FAIL_COND_V(handle == NULL, false);
    ERR_FAIL_COND_V(controller == nullptr, false);
    return controller->get_ringcon_enabled();
}

float SwitchController::get_ringcon_flex() {
    ERR_FAIL_COND_V(handle == NULL, 0.0);
    ERR_FAIL_COND_V(controller == nullptr, 0.0);
    return controller->get_ringcon_flex();
}
