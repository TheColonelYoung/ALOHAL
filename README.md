# ALOHAL
ACtual development platform is STM32L432KC (Nucelo-32)

# Architecture
Main root of system is object called `device` this device is representing whole board, including software and hardware.
- device
    - cli
    - apps
    - components
    - mcu

### Device
`device` is global object accessible from any part of code, but as object itself does not have any functionality other then common information about system (as date of compilation and project name)
This `device` contains references to structure of program, these are: cli, apps, components, mcu

### CLI
Provides interface for communicating with user.
Emulates serial communication interface. Inspired by POSIX Interface.
Load input from user and provides it to application.
Applications cannot send data directly to screen, only send data to cli.
Contains globally registered commands. This commands can be executed from every location in filesystem if is connected.
Provides build-in function for printing device info, as build_data, project name, processor name, etc.
Other device peripherals be register own command. Commands have predefined interface, must return type `int` and receive vector of string as argument, otherwise it cannot be registered.

#### File system
Files system is component, which can be add to basic `cli`.
Device peripherals can crete folders, files or executables inside folders.
Executables provides access to device functionality.
In memory file system support in some types of records:
- Directory - contains another entry, exists one main folder named `/`
- Executable - is link to function or to object and his method, which can be started from cli, have same parameters as command registered to `cli`
- File - Basic type contain only string as content, possible not-implemented types:
  - Binary - pointer to allocated RAM with binary data
  - In-Flash - pointer to space in FLASH memory where is file saved
All record types above are based on base class names FS_entry.

### Application
Supports input arguments, are provided to app as in pure 'C'.
Application have defined interface, containing methods for initialization of app during boot, starting application during run of device and for communication during run of application.
Defined methods: Boot, Start, Load_input
All methods above are called by `cli`.
`device` contains methods for registration of `app`, handler to `app` is provided to cli

### Components
Represents integrated circuits and groups of chips on board.
Are registered via methods of `device`. Every registered components have unique identifier.
These identifier could be generated or be provided by registration components.
Component can be visible from cli, and can be directly controlled, if have correct methods. -> This is only due to a development, during user usage should be disabled

### MCU
Contains basic information about MCU model, flash size, available flash, page count, frequency of main parts of mcu.
Another info can be enabled if is supported. This is CPU temperature, unique identifier, etc.
Information above can be readout in cli, if is it available.
MCU contain peripheral as Timers, UART, USB, I2C, SPI ...
Periferals are not accessible from cli.

# Access restrictions



