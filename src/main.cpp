#include <memory>
#include <string>
#include <iostream>
#include <NesSystem.hpp>

static std::shared_ptr<NesSystem> nes;

enum emuState
{
    PLAY,
    RECORD_TAS,
    PLAY_TAS,
};

void newSamples(const blip_sample_t *samples, size_t count)
{
    nes->newSamples(samples, count);
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        nes.reset(new NesSystem());
        nes->insertCartridge(argv[1]);

        // std::ofstream outputFile("test1.tas");
        // std::ifstream inputFile("test2.tas");

        // enum emuState state = PLAY;
        // bool done = false;

        while (nes->run())
            ;
    }
    else
    {
        std::cerr << "Invalid Execution: Must specify a relative path to a NES ROM binary." << std::endl;
    }

    return 0;
}