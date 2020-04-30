#include <memory>
#include <string>
#include <iostream>
#include <stdexcept>
#include <NesSystem.hpp>

static std::shared_ptr<NesSystem> nes;

void newSamples(const blip_sample_t *samples, size_t count)
{
    nes->newSamples(samples, count);
}

int main(int argc, char *argv[])
{
    try
    {
        if (!strcmp(argv[1], "play") && argc == 3)
        {
            nes.reset(new NesSystem(NesSystem::PLAY));
        }
        else if (!strcmp(argv[1], "maketas") && argc == 4)
        {
            nes.reset(new NesSystem(NesSystem::RECORD_TAS, argv[3]));
        }
        else if (!strcmp(argv[1], "playtas") && argc == 4)
        {
            nes.reset(new NesSystem(NesSystem::PLAY_TAS, argv[3]));
        }
        else
        {
            throw std::invalid_argument("Usage:\n\\
            To play a ROM: ./ness play <PATH_TO_ROM>\n\\
            To record a TAS: ./ness maketas <PATH_TO_ROM> <DESTINATION_PATH_FOR_TAS_SCRIPT>\n\\
            To replay a TAS: ./ness playtas <PATH_TO_ROM> <PATH_TO_TAS_SCRIPT>\n");
        }

        nes->insertCartridge(argv[2]);

        while (nes->run())
            ;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid Execution Commands - " << e.what() << std::endl;
    }

    return 0;
}