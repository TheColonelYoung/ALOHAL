# ALOHAL  
ACtual development platform is STM32L432KC (Nucelo-32)  

# Architecture  
Main root of system is object called `device` this device is representing whole board, including software and hardware.  
- device  
    - apps  
    - cli  
    - file system  
    - components  
    - tools  
    - mcu  

### Device  
`device` is global object accessible from any part of code, but as object itself does not have any functionality other then common information about system (as date of compilation and project name)  
Rather device is used to access all another parts of it, like mcu and all components on board which has SW representation
Device is implemented as singleton, this means access via function device(), which return pointer to device

### CLI  
Provides interface for communicating with user.  
Emulates serial communication interface. Inspired by POSIX Interface.  
Load input from user and provides it to application.  
Applications cannot send data directly to screen, only send data to cli.  
Contains globally registered commands. This commands can be executed from every location in filesystem if is connected.  
Provides build-in function for printing device info, as build_data, project name, processor name, etc.  
Other device peripherals be register own command. Commands have predefined interface, must return type `int` and receive vector of string as argument, otherwise it cannot be registered.  

### File system  
Files system is another part of device, which can be connected to `cli`.  
Device components can crete folders, files or executables inside folders.  
Executables provides access to device functionality.  
In memory file system support in some types of records:  
- Directory - contains another entry, exists one main folder named `/`  
- Executable - is link to function or to object and his method, which can be started from cli, have same parameters as command registered to `cli`  
- File - Basic type contain only string as content, possible not-implemented types:  
  - Binary - pointer to allocated RAM with binary data  
  - In-Flash - pointer to space in FLASH memory where is file saved  
All record types above are based on base class names FS_entry.  
Folder `/` always contains folders: app, components, tools, mcu  

### Application  
Supports input arguments, are provided to app as vector of strings.  
Application have defined interface, containing methods for initialization of app during boot, starting application during run of device
Defined methods: Init, Run  
    - Init is called for initialization of all parts which app needs, is called by programmer   
    - Run is called when app is executed from cli, have parameter which is vector of arguments (strings)  
All methods above can be called from cli connected to filesystem or even from code through device structure  
`device` contains methods for registration of `app`, pointer to `app` is provided to filesystem  
Application is executable inside of filesystem  

### Components  
Represents integrated circuits and groups of chips on board.  
Are registered via methods of `device`. Every registered components have unique identifier.  
These identifier could be generated or be provided by registration components.  
Component can be visible from cli, and can be directly controlled, if have correct methods. -> This is only due to a development, during user usage should be disabled  
Components is registered as form of directory, which has identifier of component, in this folder are executables or informations which component provides into cli.  

### Tools  
Are software components used in device.  
This means any manager, handler or simply anything which is not tightly connected to hardware and should be globally accessible.  
For example this means planners, tasker, etc.

### MCU  
Contains basic information about MCU model, flash size, available flash, page count, frequency of main parts of mcu.  
Another info can be enabled if is supported. This is CPU temperature, unique identifier, etc.  
Information above can be readout in cli, if is it available.  
MCU contain peripheral as Timers, UART, USB, I2C, SPI ...  
Peripherals are not accessible from cli.  

# Access restrictions  
No security for now  


