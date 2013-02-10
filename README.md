`fand` is a simple fan control daemon.  It is modelled after the
program `fancontrol` (from [lm-sensors](http://www.lm-sensors.org/)),
but is written as a minimal C daemon rather than a shell script.  It
monitors temperature sensors (via the Linux sysfs interface) and uses
them to control fans.

## Requirements

- libconfuse
- libdaemon
  
That's it.

## Configuration

`fand` reads its configuration (fans, sensors, etc.) from a
configuration file, by default located in
`${sysconfdir}/etc/fand.conf`:

```
interval = 10
sensor main
{
  path = "/sys/devices/platform/mv64xxx_i2c.0/i2c-0/0-0048/temp1_input"
}
fan main
{
  path = "/sys/devices/platform/mv64xxx_i2c.0/i2c-0/0-003e/pwm1"
  sensor = "main"
  min_temp = 20
  max_temp = 45
  min_power = 0
  max_power = 255
}
```

If the temp is below `min_temp`, the power is set to `min_power`;
respectively `max_power` if it is at or above `max_temp`.  Otherwise,
the power is linearly interpolated between the two power levels.  The
sensors and fans are updated every `interval` seconds.

## License

Copyright 2013 Michael Ekstrand <michael@elehack.net>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301 USA.

### Pull requests

By submitting a pull request, you agree to license your contributions
under this project's license unless another (compatible) license is
specified with the pull request.
