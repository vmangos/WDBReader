#ifndef H_PAGETEXT
#define H_PAGETEXT

#include "Defines.h"
#include "EscapeString.h"
#include <cassert>

struct PageText
{
    uint32 entry = 0;
    uint32 nextPageId = 0;
    uint32 playerConditionId = 0;
    uint8 flags = 0;
    std::string text;

    void ReadEntry(FILE*& pFile)
    {
        fread(&entry, sizeof(unsigned int), 1, pFile);

        unsigned int recordSize = 0;
        fread(&recordSize, sizeof(unsigned int), 1, pFile);

        char* buffer = new char[recordSize];
        fread(buffer, sizeof(char) * recordSize, 1, pFile);

        char* buf = buffer;

        // Why do they store this twice?
        unsigned int pageId = *((unsigned int*)buf);
        assert(entry == pageId);
        buf += 4;
        
        nextPageId = *((unsigned int*)buf);
        buf += 4;

        playerConditionId = *((unsigned int*)buf);
        buf += 4;

        flags = *((char*)buf);
        buf += 1;

        char textLengthByte1 = *((char*)buf);
        buf += 1;
        char textLengthByte2 = *((char*)buf);
        buf += 1;
        char textLengthBytes[2] = { textLengthByte2 , textLengthByte1 };
        unsigned short textLength = (*((unsigned short*)textLengthBytes) >> 4);

        if (textLength)
        {
            for (unsigned int i = 0; i < textLength; i++)
            {
                text += *buf;
                buf++;
            }
        }

        int remainingBytes = recordSize - (buf - buffer);
        if (remainingBytes > 0)
        {
            printf("ERROR: PageText entry not fully read. Remaining bytes: %i\n", remainingBytes);
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
        fprintf(f, "nextPageId = %u\n", nextPageId);
        fprintf(f, "playerConditionId = %u\n", playerConditionId);
        fprintf(f, "flags = %hhu\n", flags);
        fprintf(f, "text = %s\n", text.c_str());
    }

    void WriteAsSQL(FILE*& f) const
    {
        fprintf(f, "(%u, %u, %u, %hhu, '%s')", entry, nextPageId, playerConditionId, flags, EscapeString(text).c_str());
    }
};

#endif