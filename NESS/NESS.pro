TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L /usr/local/Cellar/sdl2/2.0.10/lib -lSDL2
INCLUDEPATH += /usr/local/Cellar/sdl2/2.0.10/include

INCLUDEPATH += \
    include \
    include/Apu  \
    include/Cartridge \
    include/Cartridge/Mappers \
    include/Cpu \
    include/Device-Communication \
    include/Graphics \
    include/Ppu \
    include/Ram

HEADERS += \
    include/Apu/Apu2A03.hpp \
    include/Apu/Blip_Buffer.h \
    include/Apu/Blip_Synth.h \
    include/Apu/Multi_Buffer.h \
    include/Apu/Nes_Apu.h \
    include/Apu/Nes_Namco.h \
    include/Apu/Nes_Oscs.h \
    include/Apu/Nes_Vrc6.h \
    include/Apu/Nonlinear_Buffer.h \
    include/Apu/Sound_Queue.h \
    include/Apu/apu_snapshot.h \
    include/Apu/blargg_common.h \
    include/Apu/blargg_source.h \
    include/Apu/boost/config.hpp \
    include/Apu/boost/cstdint.hpp \
    include/Apu/boost/static_assert.hpp \
    include/Cartridge/GamePak.hpp \
    include/Cartridge/Mappers/Mapper.hpp \
    include/Cartridge/Mappers/Mapper000.hpp \
    include/Cpu/AddressingModes.hpp \
    include/Cpu/Instructions.hpp \
    include/Cpu/MOS6502Instruction.hpp \
    include/Cpu/Ricoh2A03.hpp \
    include/Device-Communication/AddressableDevice.hpp \
    include/Device-Communication/Bus.hpp \
    include/Device-Communication/MMU.hpp \
    include/Graphics/Display.hpp \
    include/HwConstants.hpp \
    include/NesSystem.hpp \
    include/Ppu/RicohRP2C02.hpp \
    include/Ram/PaletteRam.hpp \
    include/Ram/Ram.hpp \
    include/Ram/Vram.hpp

SOURCES += \
        src/Apu/Apu2A03.cpp \
        src/Apu/Blip_Buffer.cpp \
        src/Apu/Multi_Buffer.cpp \
        src/Apu/Nes_Apu.cpp \
        src/Apu/Nes_Namco.cpp \
        src/Apu/Nes_Oscs.cpp \
        src/Apu/Nes_Vrc6.cpp \
        src/Apu/Nonlinear_Buffer.cpp \
        src/Apu/Sound_Queue.cpp \
        src/Apu/apu_snapshot.cpp \
        src/Cartridge/GamePak.cpp \
        src/Cartridge/Mappers/Mapper.cpp \
        src/Cartridge/Mappers/Mapper000.cpp \
        src/Cpu/AddressingModes.cpp \
        src/Cpu/Ricoh2A03.cpp \
        src/Device-Communication/AddressableDevice.cpp \
        src/Device-Communication/Bus.cpp \
        src/Device-Communication/MMU.cpp \
        src/Graphics/Display.cpp \
        src/NesSystem.cpp \
        src/Ppu/RicohRP2C02.cpp \
        src/Ram/PaletteRam.cpp \
        src/Ram/Ram.cpp \
        src/Ram/Vram.cpp \
        src/main.cpp
