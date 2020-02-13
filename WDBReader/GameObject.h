#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT

#include "Defines.h"
#include "EscapeString.h"
#include <array>
#include <set>

#define MAX_QUEST_ITEMS 3

struct GameObject
{
    uint32 entry = 0;
    uint32 type = 0;
    uint32 displayId = 0;
    std::array<std::string, 4> name = {};
    std::array<std::string, 3> unkString = {};
    std::array<uint32, 34> data = {};
    float scale = 0.0f;
    std::array<uint32, MAX_QUEST_ITEMS> questItem = {};
    uint32 requiredLevel = 0;

    void ReadEntry(FILE*& pFile)
    {
        fread(&entry, sizeof(unsigned int), 1, pFile);

        uint32 recordSize = 0;
        fread(&recordSize, sizeof(unsigned int), 1, pFile);

        char* buffer = new char[recordSize];
        fread(buffer, sizeof(char) * recordSize, 1, pFile);

        char* buf = buffer;

        type = *((unsigned int*)buf);
        buf += 4;

        displayId = *((unsigned int*)buf);
        buf += 4;

        for (int i = 0; i < 4; i++)
        {
            std::string nameStr;
            while (*buf != 0)
            {
                nameStr += *buf;
                buf++;
            }
            name[i] = nameStr;
            buf++;
        }

        for (int i = 0; i < 3; i++)
        {
            std::string unkStr;
            while (*buf != 0)
            {
                unkStr += *buf;
                buf++;
            }
            unkString[i] = unkStr;
            buf++;
        }

        for (int i = 0; i < 34; i++)
        {
            data[i] = *((unsigned int*)buf);
            buf += 4;
        }

        scale = *((float*)buf);
        buf += 4;

        uint8 questItemsCount = *(buf++);
        if (questItemsCount > MAX_QUEST_ITEMS)
        {
            printf("ERROR: GameObject has %hhu quest item ids. SQL structure needs to be fixed.\n", questItemsCount);
            return;
        }    

        for (uint8 i = 0; i < questItemsCount; i++)
        {
            uint32 questItemId = *((unsigned int*)buf);
            questItem[i] = questItemId;
            buf += 4;
        }

        requiredLevel = *((unsigned int*)buf);
        buf += 4;

        int remainingBytes = recordSize - (buf - buffer);
        if (remainingBytes > 0)
        {
            printf("ERROR: GameObject entry not fully read. Remaining bytes: %i\n", remainingBytes);
            for (int i = 0; i < remainingBytes; i++)
            {
                printf("remainingByte[%i] = %hhu\n", i, *(buf++));
            }
        }

        delete[] buffer;
    }

    void WriteAsText(FILE*& f) const
    {
        fprintf(f, "entry = %u\n", entry);
        fprintf(f, "type = %u\n", type);
        fprintf(f, "displayId = %u\n", displayId);
        fprintf(f, "scale = %f\n", scale);
        fprintf(f, "requiredLevel = %u\n", requiredLevel);

        for (int i = 0; i < 4; i++)
        {
            fprintf(f, "name[%i] = %s\n", i, EscapeString(name[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            fprintf(f, "unkString[%i] = %s\n", i, EscapeString(unkString[i]).c_str());
        }

        for (int i = 0; i < 34; i++)
        {
            // data1 and data6 are signed
            if (i == 1 || i == 6)
                fprintf(f, "data[%i] = %i\n", i, (int32)data[i]);
            else
                fprintf(f, "data[%i] = %u\n", i, data[i]);
        }

        fprintf(f, "questItemsCount = %u\n", questItem.size());
        uint32 count = 0;
        for (uint32 itemId : questItem)
        {
            fprintf(f, "questItem[%u] = %u\n", count, itemId);
            count++;
        }
    }

    void WriteAsSQL(FILE*& f) const
    {
        fprintf(f, "(%u, %u, %u, %f, %u", entry, type, displayId, scale, requiredLevel);

        for (int i = 0; i < 4; i++)
        {
            fprintf(f, ", '%s'", EscapeString(name[i]).c_str());
        }

        for (int i = 0; i < 3; i++)
        {
            fprintf(f, ", '%s'", EscapeString(unkString[i]).c_str());
        }

        for (int i = 0; i < 34; i++)
        {
            // data1 and data6 are signed
            if (i == 1 || i == 6)
                fprintf(f, ", %i", (int32)data[i]);
            else
                fprintf(f, ", %u", data[i]);
        }

        for (uint32 itemId : questItem)
        {
            fprintf(f, ", %u", itemId);
        }

        fprintf(f, ")");
    }
};

#endif