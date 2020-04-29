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
            nes->insertCartridge(argv[2]);
        }
        else if (!strcmp(argv[1], "recordtas") && argc == 4)
        {
            nes.reset(new NesSystem(NesSystem::RECORD_TAS));
        }
        else if (!strcmp(argv[1], "playtas") && argc == 4)
        {
            nes.reset(new NesSystem(NesSystem::PLAY_TAS));
        }
        else
        {
            throw std::invalid_argument("The usage is:\nness <play | recordtas | playtas> <PATH_TO_ROM>");
        }

        while (nes->run())
            ;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid Execution Commands - " << e.what() << std::endl;
    }

    return 0;
}