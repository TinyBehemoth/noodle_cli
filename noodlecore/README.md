# Noodle

Noodle is a system for organizing concepts into a multiplex graph.
Visit [the project site](https://noodle.franciscogg.site) to learn more.
There is also [a mirror on I2P](http://noodle.franciscogg.i2p) for the privacy inclined.

## License

This repo is distributed under a [GPLv3 license]().
Copyright Francisco J. Garcia-Gonzalez 2023

## This Repo

The noodle core is implemented as a static C++ library using CMake.
It is meant to be linked with other code to create noodle-based programs.

The `include/noodlecore.h` file provides the noodle core abstraction layer.
The `libnoodlecore` library has the functionality,

The core abstraction layer is split up into sections to make it easier to work with.

  - Basic Manipulations
  - Manipulations
  - Navigation
  - Layer
  - Selection
  - Utilities

## Documentation

This repo and its parent project are documented using noodle.
This readme exists primarily to conform with convention but also as a repo summary.

The noodle documentation provides project level information and much more.
