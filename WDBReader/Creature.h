#ifndef H_CREATURE
#define H_CREATURE

#include "Defines.h"
#include "EscapeString.h"
#include "ByteBuffer.h"
#include "ClientBuilds.h"

struct Creature
{
    uint32 entry = 0;
    std::string name[4];
    std::string subname;
    uint32 typeFlags = 0;
    uint32 type = 0;
    uint32 petFamily = 0;
    uint32 rank = 0;
    uint32 unk1 = 0;
    uint32 petSpellListId = 0;
    uint32 displayId = 0;
    uint8 civilian = 0;
    uint8 leader = 0;

    bool ReadEntry(ByteBuffer& buffer)
    {
        buffer >> entry;
        if (!entry)
            return false;

        uint32 entrySize;
        buffer >> entrySize;

        if (!entrySize)
            return false;

        for (uint32 i = 0; i < 4; ++i)
            buffer >> name[i];

        buffer >> subname;
        buffer >> typeFlags;
        buffer >> type;
        buffer >> petFamily;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            buffer >> rank;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            buffer >> unk1;

        if (g_clientBuild >= CLIENT_BUILD_1_8_0)
            buffer >> petSpellListId;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            buffer >> displayId;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            buffer >> civilian;

        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            buffer >> leader;

        return true;
    }

    void WriteSQLRow(FILE*& f) const
    {
        fprintf(f, "(%u", entry);

        for (uint32 i = 0; i < 4; ++i)
            fprintf(f, ", '%s'", EscapeString(name[i]).c_str());

        fprintf(f, ", '%s'", EscapeString(subname).c_str());
        fprintf(f, ", %u", typeFlags);
        fprintf(f, ", %u", type);
        fprintf(f, ", %u", petFamily);

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", %u", rank);

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", %u", unk1);

        if (g_clientBuild >= CLIENT_BUILD_1_8_0)
            fprintf(f, ", %u", petSpellListId);

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", %u", displayId);

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", %u", (uint32)civilian);

        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", %u", (uint32)leader);

        fprintf(f, ")");
    }

    static void WriteToSQL(std::vector<Creature> const& vCreatures)
    {
        FILE* f = fopen("wdb_creature_template.sql", "w");
        fprintf(f, "REPLACE INTO `wdb_creature_template` (`entry`, `name`, `name2`, `name3`, `name4`, `subname`");

        if (g_clientBuild >= CLIENT_BUILD_1_11_0)
            fprintf(f, ", `type_flags`");
        else
            fprintf(f, ", `static_flags1`");

        fprintf(f, ", `type`");
        fprintf(f, ", `pet_family`");

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", `rank`");

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", `unk1`");

        if (g_clientBuild >= CLIENT_BUILD_1_8_0)
            fprintf(f, ", `pet_spell_list_id`");

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", `display_id`");

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", `civilian`");

        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", `leader`");

        fprintf(f, ") VALUES\n");

        uint32 count = 0;
        for (auto const& creatureEntry : vCreatures)
        {
            count++;
            if (count > 1)
                fprintf(f, ",\n");

            creatureEntry.WriteSQLRow(f);
        }

        fprintf(f, ";\n");
        fclose(f);
    }
};

#endif