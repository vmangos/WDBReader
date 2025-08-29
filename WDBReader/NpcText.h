#ifndef H_NPCTEXT
#define H_NPCTEXT

#include "Defines.h"
#include "EscapeString.h"
#include "ByteBuffer.h"
#include <array>

#define NPC_TEXT_COUNT 8

struct NpcText
{
    uint32 id = 0;

    struct NpcTextEntry
    {
        float probability = 0;
        std::string maleText;
        std::string femaleText;
        uint32 languageId = 0;
        uint32 emoteDelay1 = 0;
        uint32 emoteId1 = 0;
        uint32 emoteDelay2 = 0;
        uint32 emoteId2 = 0;
        uint32 emoteDelay3 = 0;
        uint32 emoteId3 = 0;
    };

    std::array<NpcTextEntry, NPC_TEXT_COUNT> texts = {};

    bool ReadEntry(ByteBuffer& buffer)
    {
        buffer >> id;
        if (!id)
            return false;

        uint32 entrySize;
        buffer >> entrySize;

        for (auto& bct : texts)
        {
            buffer >> bct.probability;
            buffer >> bct.maleText;
            buffer >> bct.femaleText;
            buffer >> bct.languageId;
            buffer >> bct.emoteDelay1;
            buffer >> bct.emoteId1;
            buffer >> bct.emoteDelay2;
            buffer >> bct.emoteId2;
            buffer >> bct.emoteDelay3;
            buffer >> bct.emoteId3;
        }

        return true;
    }

    void WriteSQLRow(FILE*& f) const
    {
        fprintf(f, "(%u", id);

        for (auto i = 0; i < NPC_TEXT_COUNT; i++)
        {
            fprintf(f, ", %g", texts[i].probability);
            fprintf(f, ", '%s'", EscapeString(texts[i].maleText).c_str());
            fprintf(f, ", '%s'", EscapeString(texts[i].femaleText).c_str());
            fprintf(f, ", %u", texts[i].languageId);
            fprintf(f, ", %u", texts[i].emoteDelay1);
            fprintf(f, ", %u", texts[i].emoteId1);
            fprintf(f, ", %u", texts[i].emoteDelay2);
            fprintf(f, ", %u", texts[i].emoteId2);
            fprintf(f, ", %u", texts[i].emoteDelay3);
            fprintf(f, ", %u", texts[i].emoteId3);
        }

        fprintf(f, ")");
    }

    static void WriteToSQL(std::vector<NpcText> const& vNpcTexts)
    {
        FILE* f = fopen("wdb_npc_text.sql", "w");
        fprintf(f, "REPLACE INTO `npc_text` (`id`, ");

        for (int i = 0; i < NPC_TEXT_COUNT; ++i)
        {
            fprintf(f, "`probability_%i`, `male_text_%i`, `female_text_%i`, `language_id_%i`, `emote_delay1_%i`, `emote_id1_%i`, `emote_delay2_%i`, `emote_id2_%i`, `emote_delay3_%i`, `emote_id3_%i`", i, i, i, i, i, i, i, i, i, i);
        }

        fprintf(f, ") VALUES\n");

        uint32 count = 0;
        for (auto const& npcTextEntry : vNpcTexts)
        {
            count++;
            if (count > 1)
                fprintf(f, ",\n");

            npcTextEntry.WriteSQLRow(f);
        }
        fprintf(f, ";\n");
        fclose(f);
    }
};

#endif