# SwitchKit for Godot

This GDExtension wraps the [SwitchKit](https://github.com/Meorge/SwitchKit) library, allowing Godot games to use the Joy-Con (L), Joy-Con (R), and Pro Controller for the Nintendo Switch.

## Features

SwitchKit for Godot supports the following SwitchKit features:

- Basic button and stick inputs
- Acceleration and controller orientation via gyroscope
- Battery level
- Controller type
- Player LEDs
- HD Rumble
- Controller colors
- Ring-Con
  - Flex value
  - Press detected as button

## Installation/building

Place the object file `libjoycon.o` compiled from SwitchKit in the `lib` folder, then run `scons`.

See the demo project in `demo` for an example of usage.

I hope to have more comprehensive install instructions soon!

## License

MIT
