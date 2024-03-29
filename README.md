Before Installing
=================

You are required to have `gcc`, `make` and `kernel-headers` installed for building the module.

To detect your kernel version run:
```
uname -r
```
It will output something like
```
5.3.11-1-MANJARO
```
Then, on Manjaro, run:
```
sudo pacman -S linux53-headers
```

To Install
==========
Clone the repo into a GitHub directory in your home folder:
```
mkdir github
cd github
git clone https://github.com/twifty/aura-gpu.git
cd aura-gpu
```
Then run:
```
sudo make dkms
```

After Install
=============
The `i2c-dev` module is required:
```
sudo pacman -S i2c-tools
sudo modprobe i2c-dev
```
List all i2c devices with:
```
sudo i2cdetect -l
```
It will output something like
```
i2c-3   smbus           SMBus PIIX4 adapter port 4 at 0b00      SMBus adapter
i2c-10  i2c             AMDGPU DM i2c hw bus 4                  I2C adapter
i2c-1   smbus           SMBus PIIX4 adapter port 2 at 0b00      SMBus adapter
i2c-8   i2c             AMDGPU DM i2c hw bus 2                  I2C adapter
i2c-6   i2c             AMDGPU DM i2c hw bus 1                  I2C adapter
i2c-4   i2c             AMDGPU DM i2c hw bus 0                  I2C adapter
i2c-11  i2c             AURA GPU adapter                        I2C adapter
i2c-2   smbus           SMBus PIIX4 adapter port 3 at 0b00      SMBus adapter
i2c-0   smbus           SMBus PIIX4 adapter port 0 at 0b00      SMBus adapter
i2c-9   i2c             AMDGPU DM i2c hw bus 3                  I2C adapter
i2c-7   i2c             dmdc                                    I2C adapter
i2c-5   i2c             dmdc                                    I2C adapter
```
Here we can see this module "AURA GPU adapter" is detected as device number 11. We can now scan that device with:
```
sudo i2cdetect -y 11
```
It should output all clients available on that bus:
```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- 08 -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- 29 -- -- -- -- -- --
30: 30 -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- --
70: 70 -- -- -- -- -- -- --   
```
If the output it's only dashes, the driver is not working.

Troubleshooting
===============

Some C knowledge may be needed.

### Device not detected by the driver

If the device is not being detected, `sudo i2cdetect -l` will not show any "AURA GPU adapter", and running `sudo dmesg`  you should see "Failed to find any valid device".

In this case, the pci id of your device is probably missing. Check the file "pci_ids.h", the id of your device can be found using the command `lspci -vnn`.

### Device is detected by the driver but won't work (not detected by OpenRGB)

In this case `sudo i2cdetect -l` will show the device, but `sudo i2cdetect -y <number>` will show only dashes, like so:
```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --   
```
This usually means that the driver is not using the correct line. The known lines are `6` for Polaris cards, and `7` for
Vega cards, but we can only test the cards we have.
Check the function `line_number_from_asic_type` in file `aura-gpu-hw.c`.

If the line for your asic is unknown, the driver will default to line `6` and you will find "Unknown uc line for asic,
defaulting to line <default line>" in the output of `sudo dmesg`.

Although that's been the case up to now, it is possible that the type of card is not enough to choose line, in that case
some work will be needed to select line based on pci_id.

### Multiple GPUs

This driver *should* work with multiple gpus, even of different generations, and allow to configure each one.
Due to limitations (e.g. not having two GPUs) this is not tested. If having multiple GPUs causes real issues, you can
set `MAX_AURA_DEVICES` (currently 4, as you won't usually see more than 4 cards in a system) to `1` so that it behaves
as it did before multi-GPU support.

You can also increase the value if you have more than 4 GPUs. Increasing the number has a cost, don't do it if not
needed, and don't put unrealistic values, `4` should be enough for anyone.