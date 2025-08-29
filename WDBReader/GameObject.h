#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT

#include "Defines.h"
#include "EscapeString.h"
#include "ClientBuilds.h"
#include <array>

struct GameObject
{
    uint32 entry = 0;
    uint32 type = 0;
    uint32 displayId = 0;
    std::array<std::string, 4> name = {};
    std::string unk1;
    std::array<int32, 24> data = {};

    static uint32 GetDataSize()
    {
        if (g_clientBuild >= CLIENT_BUILD_1_12_0)
            return 24;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            return 16;

        return 10;
    }

    bool ReadEntry(ByteBuffer& buffer)
    {
        buffer >> entry;
        if (!entry)
            return false;

        uint32 entrySize;
        buffer >> entrySize;
        if (!entrySize)
            return false;

        buffer >> type;
        buffer >> displayId;

        for (uint32 i = 0; i < 4; ++i)
            buffer >> name[i];

        if (g_clientBuild >= CLIENT_BUILD_1_12_0)
            buffer >> unk1;

        for (uint32 i = 0; i < GetDataSize(); ++i)
            buffer >> data[i];

        return true;
    }

    void WriteSQLRow(FILE*& f) const
    {
        fprintf(f, "(%u", entry);
        fprintf(f, ", %u", type);
        fprintf(f, ", %u", displayId);

        for (uint32 i = 0; i < 4; ++i)
            fprintf(f, ", '%s'", EscapeString(name[i]).c_str());

        if (g_clientBuild >= CLIENT_BUILD_1_12_0)
            fprintf(f, ", '%s'", EscapeString(unk1).c_str());

        for (uint32 i = 0; i < GetDataSize(); ++i)
            fprintf(f, ", %i", data[i]);

        fprintf(f, ")");
    }

    static void WriteToSQL(std::vector<GameObject> const& vGameObjects)
    {
        FILE* f = fopen("wdb_gameobject_template.sql", "w");
        fprintf(f, "REPLACE INTO `wdb_gameobject_template` (`entry`, `type`, `display_id`, `name`, `name2`, `name3`, `name4`");

        if (g_clientBuild >= CLIENT_BUILD_1_12_0)
            fprintf(f, ", `unk1`");

        for (uint32 i = 0; i < GetDataSize(); ++i)
            fprintf(f, ", `data%u`", i);

        fprintf(f, ") VALUES\n");

        uint32 count = 0;
        for (auto const& goEntry : vGameObjects)
        {
            count++;
            if (count > 1)
                fprintf(f, ",\n");

            goEntry.WriteSQLRow(f);
        }

        fprintf(f, ";\n");
        fclose(f);
    }
};

#endif