#ifndef H_PAGETEXT
#define H_PAGETEXT

#include "Defines.h"
#include "EscapeString.h"
#include "ByteBuffer.h"
#include <cassert>

struct PageText
{
    uint32 entry = 0;
    uint32 nextPageId = 0;
    std::string text;

    bool ReadEntry(ByteBuffer& buffer)
    {
        buffer >> entry;
        if (!entry)
            return false;

        uint32 entrySize;
        buffer >> entrySize;

        if (!entrySize)
            return false;

        buffer >> text;
        buffer >> nextPageId;
        return true;
    }

    void WriteSQLRow(FILE*& f) const
    {
        fprintf(f, "(%u, %u, '%s')", entry, nextPageId, EscapeString(text).c_str());
    }

    static void WriteToSQL(std::vector<PageText> const& vPageTexts)
    {
        FILE* f = fopen("wdb_page_text.sql", "w");
        fprintf(f, "REPLACE INTO `wdb_page_text` (`entry`, `next_page`, `text`) VALUES \n");
        uint32 count = 0;
        for (auto const& pageTextEntry : vPageTexts)
        {
            count++;
            if (count > 1)
                fprintf(f, ",\n");

            pageTextEntry.WriteSQLRow(f);
        }
        fprintf(f, ";\n");
        fclose(f);
    }
};

#endif