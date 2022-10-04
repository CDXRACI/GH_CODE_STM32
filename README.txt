Hello!
-----------------------------------------------------------------------------

 My name is Phuc. This is my configuration for STM32 on Visual with purpose
is build and debug on it.

----------------------------------------------------------------------------

1, STEP
	*, In makefile,you must insert it:

#######################################
#openocd
#######################################
flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"
  
#######################################

	**, In cpp_configurations:

{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "~/Downloads/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc",
            "compilerArgs": [
                "-mcpu=cortrx-m4",
                "-mthumb",
                "-mfloat-abi=hard"
            ],
            "cStandard": "gnu17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        }
    ],
    "version": 4
}

#############################################################################
	***, In launch.json:

{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "MCU",
            "type": "cortex-debug",
            "request": "launch",
            "executable": "./build/12.elf",
            "cwd": "${workspaceRoot}",
            "servertype": "openocd",
            "device": "STM32F411RE",
            "configFiles": [
                "interface/stlink.cfg",
                "target/stm32f4x.cfg"
            ]
        },
    ]
}

Attention: you should creat 2 file cpp_config and launch then attacks your
link into visual.
