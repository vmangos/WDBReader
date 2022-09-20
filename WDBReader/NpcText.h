#ifndef H_NPCTEXT
#define H_NPCTEXT

#include "Defines.h"
#include "EscapeString.h"
#include <array>

#define NPC_TEXT_COUNT 8

struct NpcText
{
    uint32 id = 0;
    std::array<float, NPC_TEXT_COUNT> probability = {};
    std::array<uint32, NPC_TEXT_COUNT> broadcastTextId = {};

    void ReadEntry(FILE*& pFile)
    {
        fread(&id, sizeof(unsigned int), 1, pFile);

        uint32 recordSize = 0;
        fread(&recordSize, sizeof(unsigned int), 1, pFile);

        char* buffer = new char[recordSize];
        fread(buffer, sizeof(char) * recordSize, 1, pFile);

        char* buf = buffer;

        for (auto& i : probability)
        {
            i = *((float*)buf);
            buf += 4;
        }

        for (auto& i : broadcastTextId)
        {
            i = *((unsigned int*)buf);
            buf += 4;
        }

        int remainingBytes = recordSize - (buf - buffer);
        if (remainingBytes > 0)
        {
            printf("ERROR: NpcText entry not fully read. Remaining bytes: %i\n", remainingBytes);
            for (int i = 0; i < remainingBytes; i++)
            {
                printf("remainingByte[%i] = %hhu\n", i, *(buf++));
            }
        }

        delete[] buffer;
    }

    void WriteAsText(FILE*& f) const
    {
        fprintf(f, "id = %u\n", id);
        
        for (auto i = 0; i < NPC_TEXT_COUNT; i++)
        {
            fprintf(f, "probability%u = %g\n", i, probability[i]);
        }

        for (auto i = 0; i < NPC_TEXT_COUNT; i++)
        {
            fprintf(f, "broadcastTextId%u = %u\n", i, broadcastTextId[i]);
        }
    }

    void WriteAsSQL(FILE*& f) const
    {
        fprintf(f, "(%u", id);

        for (auto i = 0; i < NPC_TEXT_COUNT; i++)
        {
            fprintf(f, ", %g", probability[i]);
        }

        for (auto i = 0; i < NPC_TEXT_COUNT; i++)
        {
            fprintf(f, ", %u", broadcastTextId[i]);
        }

        fprintf(f, ")");
    }
};

#endif
