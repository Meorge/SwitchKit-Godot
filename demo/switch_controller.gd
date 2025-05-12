extends SwitchController

@onready var model: MeshInstance3D = %Model

# Called when the node enters the scene tree for the first time.
func _ready():
    connect_controller()
    request_stick_calibration()
    set_imu_enabled(true)

    set_player_lights(SwitchController.LIGHT_OFF, SwitchController.LIGHT_FLASH, SwitchController.LIGHT_OFF, SwitchController.LIGHT_FLASH)

    var mat: StandardMaterial3D = model.material_override
    mat.albedo_color = get_color(SwitchController.COLOR_BODY)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
    poll()
    var gyro = get_gyro()
    model.rotation = gyro

    if get_button(SwitchController.BTN_A):
        rumble({
            "right_high_amp": 0.3,
            "right_high_freq": 110.0
        })
    else:
        rumble({})
    
