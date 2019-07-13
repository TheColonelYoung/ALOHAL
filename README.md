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
`device` is global object accesible from any part of code, but as object itself does not have any fuctionality other then common information about system (as date of compilation and project name)
This `device` contains references to structure of program, these are: cli, apps, components, mcu

### CLI
Provides interface for communicating with user.
Emulates serial comunication iterface. Inspired by POSIX.
Load input from user and provides it to application.
Applications cannot send data directly to screen, only send data to cli.
`cli` provides function for printing device info, as build_data, project name, procesor name, etc.

### Application
Supports input arguments, are provided to app as in pure 'C'.
Application have defined interface, containing methods for initialization of app during boot, starting application during run of device and for communication during run of application.
Defined methods: Boot, Start, Load_input
All methods above are called by `cli`.
`device` contains methods for registration of `app`, handler to `app` is provided to cli

### Components
Represents integrated circuits and groups of chips on board.
Are registred via methods of `device`. Every regsitred components have unique identifier.
These identifier could be generated or be provided by registrating components.
Component can be visible from cli, and can be directly controlled, if have correct methods. -> This is only due to a development, during user usage should be disabled

### MCU
Contains basic information about MCU model, flash size, avaible flash, page count, frequency of main parts of mcu.
Another info can be enabled if is supported. This is CPU tempretature, unique identifier, etc.
Information above can be readout in cli, if is it avaible.
MCU contain peripheral as Timers, UART, USB, I2C, SPI ...
Periferals are not accessible from cli.

# Access restrictions

