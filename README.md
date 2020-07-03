# XMousePasteBlock

Listens for middle mouse click events and clears the primary X selection/clipboard on detection to avoid accidentially pasting stuff all over the place.

## About

No need to disable your precious middle mouse button bindings, no clearing of visual selections nor performance losses because of emptying the primary X clipboard periodically.
With the utilization of XInput and Xlibs this has _no_ measurable impact on performance whatsoever.
No elevated privileges required. Just run within your regular users' X session.

## Dependencies

You might need to install the `libev`, `libx11` and `libxi` (X11 Input Extension) headers.

On Debian derivatives:
````
sudo apt-get install libev-dev libx11-dev libxi-dev
````

## Building
````
make
sudo make install
````

## Running
Just add `xmousepasteblock` to your startup script/config.

## Known issues
In case of devices which are configured with middle mouse button hold-to-scroll (e.g. Trackpoints), it may happen that the primary selection clear action gets fired too late on older and slower machines.
You can observe the behavior by building with the DEBUG flag set (`make debug`), running `xmousepasteblock` in a shell and watching the debug output as you long press and hold the mouse buttons.

This is due to the fact that the XI_RawButtonPress event only gets fired _after_ releasing the middle mouse button (in case the user wanted to execute a scroll action).
The only option to work around this is to disable the middle mouse button hold-to-scroll functionality on Trackpoint devices (which is often not desirable).
To do so (using _libinput_):
`xinput set-prop <device id> 'libinput Button Scrolling Button' 0`
