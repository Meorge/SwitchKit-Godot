extends SwitchController


# Called when the node enters the scene tree for the first time.
func _ready():
    connect_controller()
    request_stick_calibration()

    set_player_lights(SwitchController.LIGHT_OFF, SwitchController.LIGHT_FLASH, SwitchController.LIGHT_OFF, SwitchController.LIGHT_FLASH)

    var body_color_rect: ColorRect = %BodyColor
    body_color_rect.color = get_color(SwitchController.COLOR_BODY)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
    poll()
    
