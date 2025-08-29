#ifndef H_QUEST
#define H_QUEST

#include "Defines.h"
#include "EscapeString.h"
#include "ByteBuffer.h"
#include "ClientBuilds.h"
#include <cassert>

#define QUEST_OBJECTIVES_COUNT 4
#define QUEST_REWARD_CHOICES_COUNT 6
#define QUEST_REWARDS_COUNT 4

struct Quest
{
    uint32 entry = 0;
    uint32 method = 0;
    uint32 questLevel = 0;
    int32 zoneOrSort = 0;
    uint32 type = 0;
    uint32 repObjectiveFaction = 0;
    uint32 repObjectiveValue = 0;
    uint32 requiredOpositeRepFaction = 0;
    uint32 requiredOpositeRepValue = 0;
    uint32 nextQuestInChain = 0;
    uint32 rewOrReqMoney = 0;
    uint32 rewMoneyMaxLevel = 0;
    uint32 rewSpell = 0;
    uint32 srcItemId = 0;
    uint32 questFlags = 0;
    uint32 rewItemId[QUEST_REWARDS_COUNT] = {};
    uint32 rewItemCount[QUEST_REWARDS_COUNT] = {};
    uint32 rewChoiceItemId[QUEST_REWARD_CHOICES_COUNT] = {};
    uint32 rewChoiceItemCount[QUEST_REWARD_CHOICES_COUNT] = {};
    uint32 pointMapId = 0;
    float pointX = 0;
    float pointY = 0;
    uint32 pointOpt = 0;
    std::string title;
    std::string objectives;
    std::string details;
    std::string endText;
    uint32 reqCreatureOrGOId[QUEST_OBJECTIVES_COUNT] = {};
    uint32 reqCreatureOrGOCount[QUEST_OBJECTIVES_COUNT] = {};
    uint32 reqItemId[QUEST_OBJECTIVES_COUNT] = {};
    uint32 reqItemCount[QUEST_OBJECTIVES_COUNT] = {};
    std::string objectiveText[QUEST_OBJECTIVES_COUNT];

    bool ReadEntry(ByteBuffer& buffer)
    {
        buffer >> entry;
        if (!entry)
            return false;

        uint32 entrySize;
        buffer >> entrySize;

        if (!entrySize)
            return false;

        buffer >> entry;
        buffer >> method;
        buffer >> questLevel;
        buffer >> zoneOrSort;
        buffer >> type;

        buffer >> repObjectiveFaction;
        buffer >> repObjectiveValue;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            buffer >> requiredOpositeRepFaction;

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            buffer >> requiredOpositeRepValue;

        buffer >> nextQuestInChain;
        buffer >> rewOrReqMoney;

        if (g_clientBuild >= CLIENT_BUILD_1_10_0)
            buffer >> rewMoneyMaxLevel;

        buffer >> rewSpell;
        buffer >> srcItemId;
        buffer >> questFlags;

        for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        {
            buffer >> rewItemId[i];
            buffer >> rewItemCount[i];
        }

        for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        {
            buffer >> rewChoiceItemId[i];
            buffer >> rewChoiceItemCount[i];
        }
        
        buffer >> pointMapId;
        buffer >> pointX;
        buffer >> pointY;
        buffer >> pointOpt;
        buffer >> title;
        buffer >> objectives;
        buffer >> details;
        buffer >> endText;

        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        {
            buffer >> reqCreatureOrGOId[i];
            buffer >> reqCreatureOrGOCount[i];
            buffer >> reqItemId[i];
            buffer >> reqItemCount[i];
        }

        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            buffer >> objectiveText[i];

        return true;
    }

    void WriteSQLRow(FILE*& f) const
    {
        fprintf(f, "(%u", entry);
        fprintf(f, ", %u", method);
        fprintf(f, ", %u", questLevel);
        fprintf(f, ", %i", zoneOrSort);
        fprintf(f, ", %u", type);
        fprintf(f, ", %u", repObjectiveFaction);
        fprintf(f, ", %u", repObjectiveValue);

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", %u", requiredOpositeRepFaction);
        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", %u", requiredOpositeRepValue);

        fprintf(f, ", %u", nextQuestInChain);
        fprintf(f, ", %u", rewOrReqMoney);

        if (g_clientBuild >= CLIENT_BUILD_1_10_0)
            fprintf(f, ", %u", rewMoneyMaxLevel);

        fprintf(f, ", %u", rewSpell);
        fprintf(f, ", %u", srcItemId);
        fprintf(f, ", %u", questFlags);

        for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
            fprintf(f, ", %u", rewItemId[i]);
        for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
            fprintf(f, ", %u", rewItemCount[i]);
        for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
            fprintf(f, ", %u", rewChoiceItemId[i]);
        for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
            fprintf(f, ", %u", rewChoiceItemCount[i]);

        fprintf(f, ", %u", pointMapId);
        fprintf(f, ", %g", pointX);
        fprintf(f, ", %g", pointY);
        fprintf(f, ", %u", pointOpt);
        fprintf(f, ", '%s'", EscapeString(title).c_str());
        fprintf(f, ", '%s'", EscapeString(objectives).c_str());
        fprintf(f, ", '%s'", EscapeString(details).c_str());
        fprintf(f, ", '%s'", EscapeString(endText).c_str());

        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", %u", reqCreatureOrGOId[i]);
        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", %u", reqCreatureOrGOCount[i]);
        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", %u", reqItemId[i]);
        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", %u", reqItemCount[i]);
        for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", '%s'", EscapeString(objectiveText[i]).c_str());

        fprintf(f, ")");
    }

    static void WriteToSQL(std::vector<Quest> const& vQuests)
    {
        FILE* f = fopen("wdb_quest_template.sql", "w");
        fprintf(f, "REPLACE INTO `wdb_quest_template` (`entry`");

        fprintf(f, ", `Method`");
        fprintf(f, ", `QuestLevel`");
        fprintf(f, ", `ZoneOrSort`");
        fprintf(f, ", `Type`");
        fprintf(f, ", `RepObjectiveFaction`");
        fprintf(f, ", `RepObjectiveValue`");

        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", `RequiredOpositeRepFaction`");
        if (g_clientBuild >= CLIENT_BUILD_0_6_0)
            fprintf(f, ", `RequiredOpositeRepValue`");

        fprintf(f, ", `NextQuestInChain`");
        fprintf(f, ", `RewOrReqMoney`");

        if (g_clientBuild >= CLIENT_BUILD_1_10_0)
            fprintf(f, ", `RewMoneyMaxLevel`");

        fprintf(f, ", `RewSpell`");
        fprintf(f, ", `SrcItemId`");
        fprintf(f, ", `QuestFlags`");

        for (int i = 1; i <= QUEST_REWARDS_COUNT; ++i)
            fprintf(f, ", `RewItemId%i`", i);
        for (int i = 1; i <= QUEST_REWARDS_COUNT; ++i)
            fprintf(f, ", `RewItemCount%i`", i);
        for (int i = 1; i <= QUEST_REWARD_CHOICES_COUNT; ++i)
            fprintf(f, ", `RewChoiceItemId%i`", i);
        for (int i = 1; i <= QUEST_REWARD_CHOICES_COUNT; ++i)
            fprintf(f, ", `RewChoiceItemCount%i`", i);

        fprintf(f, ", `PointMapId`");
        fprintf(f, ", `PointX`");
        fprintf(f, ", `PointY`");
        fprintf(f, ", `PointOpt`");
        fprintf(f, ", `Title`");
        fprintf(f, ", `Objectives`");
        fprintf(f, ", `Details`");
        fprintf(f, ", `EndText`");

        for (int i = 1; i <= QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", `ReqCreatureOrGOId%i`", i);
        for (int i = 1; i <= QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", `ReqCreatureOrGOCount%i`", i);
        for (int i = 1; i <= QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", `ReqItemId%i`", i);
        for (int i = 1; i <= QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", `ReqItemCount%i`", i);
        for (int i = 1; i <= QUEST_OBJECTIVES_COUNT; ++i)
            fprintf(f, ", `ObjectiveText%i`", i);

        fprintf(f, ") VALUES \n");

        uint32 count = 0;
        for (auto const& questEntry : vQuests)
        {
            count++;
            if (count > 1)
                fprintf(f, ",\n");

            questEntry.WriteSQLRow(f);
        }
        fprintf(f, ";\n");
        fclose(f);
    }
};

#endif
