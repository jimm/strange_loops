# Strange Loops

**NOTE: Strange Loops is a work in progess. Don't use it yet.**

Strange Loops is a MIDI looper. Up to 128 _scenes_ can store up to 16
looping _tracks_, one for each MIDI channel.

Although SL can listen to more than one MIDI device, it only pays attention
to the incoming MIDI channel. That is, playing MIDI channel 1 on any
input instrument will send data to channel 1 on all output instruments.

MIDI controller messages can be used to start and stop recording tracks,
mute them, and perform other actions. A simple configuration file tells
Strange Loops what MIDI controller to use for what action.

Scenes can be loaded from and saved to MIDI files.

## Installation

### Requirements

- The [PortMidi](http://portmedia.sourceforge.net/portmidi/) library. If you
  use [Homebrew](https://brew.sh/) it's a simple `brew install portmidi`
  away.
- A C++11 compiler and the `make` program

### Making Strange Loops

Download the Strange Loops code from
[Github](https://github.com/jimm/strange_loops) and run `make install`. If
you're on a Mac, you will need the XCode Command Line Tools package. (I
think you will be prompted to download them if you don't have them installed
already.) If you're not running MacOS, you'll have to change a few of the
compile-time flags in the Makefile first.

By default, `strange_loops` will be installed in `/usr/local/bin`. To
install it elsewhere run `make install bindir=/path/to/desired/dir`.

## Quick Start

## Usage

TODO: Write usage instructions here

## Development

TODO: Write development instructions here

## Contributing

1. Fork it (<https://github.com/jimm/strange_loops/fork>)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request

## Contributors

- [Jim Menard](https://github.com/jimm) - creator and maintainer
