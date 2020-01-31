#include "Defines.h"
#include "GameObject.h"
#include "NpcText.h"
#include "PageText.h"
#include "stdio.h"
#include <string>
#include <vector>

enum
{
    WRITE_NONE,
    WRITE_AS_TEXT,
    WRITE_AS_SQL,
};

unsigned int g_counter = 0;

void ReadCreatureEntry(FILE*& pFile)
{
    g_counter++;
    std::string const filename = std::string("creature") + std::to_string(g_counter) + std::string(".txt");
    FILE* f = fopen(filename.c_str(), "w");
    if (f == nullptr)
    {
        printf("Error creating file!\n");
        exit(1);
    }

    unsigned int entry = 0;
    fread(&entry, sizeof(unsigned int), 1, pFile);
    fprintf(f, "entry = %u\n", entry);

    unsigned int recordSize = 0;
    fread(&recordSize, sizeof(unsigned int), 1, pFile);
    fprintf(f, "recordSize = %u\n", recordSize);

    char* buffer = new char[recordSize];
    fread(buffer, sizeof(char) * recordSize, 1, pFile);

    // 11 bits
    char titleBytes[2] = { buffer[1], buffer[0] };
    unsigned short titleLength = (*((unsigned short*)titleBytes) >> 5) & 0b11111111111;
    fprintf(f, "titleLength = %hu\n", titleLength);

    // 11 bits
    char titleAltBytes[2] = { buffer[2], buffer[1] };
    unsigned short titleAltLength = (*((unsigned short*)titleAltBytes) >> 2) & 0b11111111111;
    fprintf(f, "titleAltLength = %hu\n", titleAltLength);

    // 8 bits between the string lengths
    // second bit may be Civilian
    char unkBytes[2] = { buffer[3], buffer[2] };
    unsigned short unkByte = (*((unsigned short*)unkBytes) >> 2) & 0b11111111;
    fprintf(f, "unkByte = %hu\n", unkByte);

    // 11 bits
    char nameBytes[4] = { buffer[5], buffer[4], buffer[3], 0 };
    unsigned int nameLength = (*((unsigned int*)nameBytes) >> 7) & 0b11111111111;
    fprintf(f, "nameLength = %i\n", nameLength);

    // 11 bits
    char name2Bytes[2] = { buffer[6], buffer[5] };
    unsigned short name2Length = (*((unsigned short*)name2Bytes) >> 4) & 0b11111111111;
    fprintf(f, "name2Length = %hu\n", name2Length);

    // 11 bits
    char name3Bytes[2] = { buffer[7], buffer[6] };
    unsigned short name3Length = (*((unsigned short*)name3Bytes) >> 1) & 0b11111111111;
    fprintf(f, "name3Length = %hu\n", name3Length);

    // 11 bits
    char name4Bytes[4] = { buffer[9], buffer[8], buffer[7], 0 };
    unsigned int name4Length = (*((unsigned int*)name4Bytes) >> 6) & 0b11111111111;
    fprintf(f, "name4Length = %i\n", name4Length);

    // 11 bits
    char name5Bytes[2] = { buffer[10], buffer[9] };
    unsigned short name5Length = (*((unsigned short*)name5Bytes) >> 3) & 0b11111111111;
    fprintf(f, "name5Length = %hu\n", name5Length);

    // 11 bits
    char name6Bytes[2] = { buffer[11], buffer[10] };
    unsigned short name6Length = (*((unsigned short*)name6Bytes)) & 0b11111111111;
    fprintf(f, "name6Length = %hu\n", name6Length);

    // 11 bits
    char name7Bytes[2] = { buffer[13], buffer[12] };
    unsigned short name7Length = (*((unsigned short*)name7Bytes) >> 5) & 0b11111111111;
    fprintf(f, "name7Length = %hu\n", name7Length);

    // 11 bits
    char name8Bytes[2] = { buffer[14], buffer[13] };
    unsigned short name8Length = (*((unsigned short*)name8Bytes) >> 2) & 0b11111111111;
    fprintf(f, "name8Length = %hu\n", name8Length);

    char* buf = &buffer[15];
    if (nameLength)
    {
        std::string name;
        for (unsigned int i = 0; i < nameLength; i++)
        {
            name += *buf;
            buf++;
        }
        fprintf(f, "name = %s\n", name.c_str());
    }

    if (name2Length)
    {
        std::string name2;
        for (unsigned int i = 0; i < name2Length; i++)
        {
            name2 += *buf;
            buf++;
        }
        fprintf(f, "name2 = %s\n", name2.c_str());
    }

    if (name3Length)
    {
        std::string name3;
        for (unsigned int i = 0; i < name3Length; i++)
        {
            name3 += *buf;
            buf++;
        }
        fprintf(f, "name3 = %s\n", name3.c_str());
    }

    if (name4Length)
    {
        std::string name4;
        for (unsigned int i = 0; i < name4Length; i++)
        {
            name4 += *buf;
            buf++;
        }
        fprintf(f, "name4 = %s\n", name4.c_str());
    }

    if (name5Length)
    {
        std::string name5;
        for (unsigned int i = 0; i < name5Length; i++)
        {
            name5 += *buf;
            buf++;
        }
        fprintf(f, "name5 = %s\n", name5.c_str());
    }

    if (name6Length)
    {
        std::string name6;
        for (unsigned int i = 0; i < name6Length; i++)
        {
            name6 += *buf;
            buf++;
        }
        fprintf(f, "name6 = %s\n", name6.c_str());
    }

    if (name7Length)
    {
        std::string name7;
        for (unsigned int i = 0; i < name7Length; i++)
        {
            name7 += *buf;
            buf++;
        }
        fprintf(f, "name7 = %s\n", name7.c_str());
    }

    if (name8Length)
    {
        std::string name8;
        for (unsigned int i = 0; i < name8Length; i++)
        {
            name8 += *buf;
            buf++;
        }
        fprintf(f, "name8 = %s\n", name8.c_str());
    }

    unsigned int typeFlags = *((unsigned int*)buf);
    fprintf(f, "typeFlags = %u\n", typeFlags);
    buf += 4;

    unsigned int typeFlags2 = *((unsigned int*)buf);
    fprintf(f, "typeFlags2 = %u\n", typeFlags2);
    buf += 4;

    unsigned int type = *((unsigned int*)buf);
    fprintf(f, "type = %u\n", type);
    buf += 4;

    unsigned int family = *((unsigned int*)buf);
    fprintf(f, "family = %u\n", family);
    buf += 4;

    unsigned int rank = *((unsigned int*)buf);
    fprintf(f, "rank = %u\n", rank);
    buf += 4;

    unsigned int petSpellDataId = *((unsigned int*)buf);
    fprintf(f, "petSpellDataId = %u\n", petSpellDataId);
    buf += 4;

    unsigned int killCredit1 = *((unsigned int*)buf);
    fprintf(f, "killCredit1 = %u\n", killCredit1);
    buf += 4;

    unsigned int killCredit2 = *((unsigned int*)buf);
    fprintf(f, "killCredit2 = %u\n", killCredit2);
    buf += 4;

    unsigned int displayIdCount = *((unsigned int*)buf);
    fprintf(f, "displayIdCount = %u\n", displayIdCount);
    buf += 4;

    float totalProbability = *((float*)buf);
    fprintf(f, "totalProbability = %f\n", totalProbability);
    buf += 4;

    unsigned int displayId = *((unsigned int*)buf);
    fprintf(f, "displayId[0] = %u\n", displayId);
    buf += 4;
    float displayScale = *((float*)buf);
    fprintf(f, "displayScale[0] = %f\n", displayScale);
    buf += 4;
    float displayProbability = *((float*)buf);
    fprintf(f, "displayProbability[0] = %f\n", displayProbability);
    
    for (unsigned int i = 0; i < displayIdCount-1; i++)
    {
        buf += 4;
        unsigned int displayId = *((unsigned int*)buf);
        fprintf(f, "displayId[%u] = %u\n", i+1, displayId);
        buf += 4;
        float displayScale = *((float*)buf);
        fprintf(f, "displayScale[%u] = %f\n", i+1, displayScale);
        buf += 4;
        float displayProbability = *((float*)buf);
        fprintf(f, "displayProbability[%u] = %f\n", i+1, displayProbability);
    }

    buf += 4;
    float healthMultiplier = *((float*)buf);
    fprintf(f, "healthMultiplier = %f\n", healthMultiplier);
    buf += 4;
    float manaMultiplier = *((float*)buf);
    fprintf(f, "manaMultiplier = %f\n", manaMultiplier);
    buf += 4;
    unsigned int questItemsCount = *((unsigned int*)buf);
    fprintf(f, "questItemsCount = %u\n", questItemsCount);
    buf += 4;
    unsigned int movementInfoId = *((unsigned int*)buf);
    fprintf(f, "movementInfoId = %u\n", movementInfoId);
    buf += 4;
    unsigned int healthScalingExpansion = *((unsigned int*)buf);
    fprintf(f, "healthScalingExpansion = %u\n", healthScalingExpansion);
    buf += 4;
    unsigned int requiredExpansion = *((unsigned int*)buf);
    fprintf(f, "requiredExpansion = %u\n", requiredExpansion);
    buf += 4;
    unsigned int vignetteID = *((unsigned int*)buf);
    fprintf(f, "vignetteID = %u\n", vignetteID);
    buf += 4;
    unsigned int unitClass = *((unsigned int*)buf);
    fprintf(f, "unitClass = %u\n", unitClass);
    buf += 4;

    if (titleLength)
    {
        std::string subname;
        while (*buf != 0)
        {
            subname += *buf;
            buf++;
        }
        fprintf(f, "subname = %s\n", subname.c_str());
        buf++;
    }

    if (titleAltLength)
    {
        std::string subnameAlt;
        while (*buf != 0)
        {
            subnameAlt += *buf;
            buf++;
        }
        fprintf(f, "subnameAlt = %s\n", subnameAlt.c_str());
        buf++;
    }

    for (unsigned int i = 0; i < questItemsCount; i++)
    {
        unsigned int questItem = *((unsigned int*)buf);
        fprintf(f, "questItem[%u] = %u\n", i, questItem);
        buf += 4;
    }

    int remainingBytes = recordSize - (buf - buffer);
    fprintf(f, "remaining bytes: %i\n", remainingBytes);
    for (int i = 0; i < remainingBytes; i++)
    {
        fprintf(f, "remainingByte[%i] = %hhu\n", i, *(buf++));
    }
    
    delete[] buffer;
    fclose(f);
}

std::vector<GameObject> vGameObjects;

void ReadGameObjectEntry(FILE*& pFile)
{
    g_counter++;
    GameObject goEntry;
    goEntry.ReadEntry(pFile);
    vGameObjects.push_back(goEntry);
}

void WriteGameObjects()
{
    if (vGameObjects.empty())
        printf("No objects to write!\n");

    printf("\nSelect output method:\n");
    printf("1. Text\n");
    printf("2. SQL\n");
    printf("> ");

    fseek(stdin, 0, SEEK_END);
    char option = getchar() - '0';

    switch (option)
    {
        case WRITE_AS_TEXT:
        {
            for (uint32 i = 0; i < vGameObjects.size(); i++)
            {
                std::string const filename = std::string("gameobject") + std::to_string(i) + std::string(".txt");
                FILE* f = fopen(filename.c_str(), "w");
                if (f == nullptr)
                {
                    printf("Error creating file!\n");
                    exit(1);
                }

                vGameObjects[i].WriteAsText(f);
                fclose(f);
            }
            break;
        }
        case WRITE_AS_SQL:
        {
            FILE* f = fopen("gameobject_template.sql", "w");
            fprintf(f, "REPLACE INTO `gameobject_template` (`entry`, `type`, `display_id`, `scale`, `required_level`, `name1`, `name2`, `name3`, `name4`, `unk_string1`, `unk_string2`, `unk_string3`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `data33`, `quest_item1`, `quest_item2`, `quest_item3`) VALUES \n");
            uint32 count = 0;
            for (auto const& goEntry : vGameObjects)
            {
                count++;
                if (count > 1)
                    fprintf(f, ",\n");

                goEntry.WriteAsSQL(f);
            }
            fprintf(f, ";\n");
            fclose(f);
            break;
        }
    }
}

std::vector<NpcText> vNpcTexts;

void ReadNpcTextEntry(FILE*& pFile)
{
    g_counter++;
    NpcText npcTextEntry;
    npcTextEntry.ReadEntry(pFile);
    vNpcTexts.push_back(npcTextEntry);
}

void WriteNpcTexts()
{
    if (vNpcTexts.empty())
        printf("No npc texts to write!\n");

    printf("\nSelect output method:\n");
    printf("1. Text\n");
    printf("2. SQL\n");
    printf("> ");

    fseek(stdin, 0, SEEK_END);
    char option = getchar() - '0';

    switch (option)
    {
        case WRITE_AS_TEXT:
        {
            for (uint32 i = 0; i < vNpcTexts.size(); i++)
            {
                std::string const filename = std::string("npctext") + std::to_string(i) + std::string(".txt");
                FILE* f = fopen(filename.c_str(), "w");
                if (f == nullptr)
                {
                    printf("Error creating file!\n");
                    exit(1);
                }

                vNpcTexts[i].WriteAsText(f);
                fclose(f);
            }
            break;
        }
        case WRITE_AS_SQL:
        {
            FILE* f = fopen("npc_text.sql", "w");
            fprintf(f, "REPLACE INTO `npc_text` (`ID`, `Probability0`, `Probability1`, `Probability2`, `Probability3`, `Probability4`, `Probability5`, `Probability6`, `Probability7`, `BroadcastTextID0`, `BroadcastTextID1`, `BroadcastTextID2`, `BroadcastTextID3`, `BroadcastTextID4`, `BroadcastTextID5`, `BroadcastTextID6`, `BroadcastTextID7`) VALUES \n");
            uint32 count = 0;
            for (auto const& npcTextEntry : vNpcTexts)
            {
                count++;
                if (count > 1)
                    fprintf(f, ",\n");

                npcTextEntry.WriteAsSQL(f);
            }
            fprintf(f, ";\n");
            fclose(f);
            break;
        }
    }
}

std::vector<PageText> vPageTexts;

void ReadPageTextEntry(FILE*& pFile)
{
    g_counter++;
    PageText pageTextEntry;
    pageTextEntry.ReadEntry(pFile);
    vPageTexts.push_back(pageTextEntry);
}

void WritePageTexts()
{
    if (vPageTexts.empty())
        printf("No page texts to write!\n");

    printf("\nSelect output method:\n");
    printf("1. Text\n");
    printf("2. SQL\n");
    printf("> ");

    fseek(stdin, 0, SEEK_END);
    char option = getchar() - '0';

    switch (option)
    {
        case WRITE_AS_TEXT:
        {
            for (uint32 i = 0; i < vPageTexts.size(); i++)
            {
                std::string const filename = std::string("pagetext") + std::to_string(i) + std::string(".txt");
                FILE* f = fopen(filename.c_str(), "w");
                if (f == nullptr)
                {
                    printf("Error creating file!\n");
                    exit(1);
                }

                vPageTexts[i].WriteAsText(f);
                fclose(f);
            }
            break;
        }
        case WRITE_AS_SQL:
        {
            FILE* f = fopen("page_text.sql", "w");
            fprintf(f, "REPLACE INTO `page_text` (`Entry`, `NextPageId`, `PlayerConditionId`, `Flags`, `Text`) VALUES \n");
            uint32 count = 0;
            for (auto const& pageTextEntry : vPageTexts)
            {
                count++;
                if (count > 1)
                    fprintf(f, ",\n");

                pageTextEntry.WriteAsSQL(f);
            }
            fprintf(f, ";\n");
            fclose(f);
            break;
        }
    }
}

void ReadQuestEntry(FILE*& pFile)
{
    g_counter++;
    std::string const filename = std::string("quest") + std::to_string(g_counter) + std::string(".txt");
    FILE* f = fopen(filename.c_str(), "w");
    if (f == nullptr)
    {
        printf("Error creating file!\n");
        exit(1);
    }

    unsigned int entry = 0;
    fread(&entry, sizeof(unsigned int), 1, pFile);
    fprintf(f, "entry = %u\n", entry);

    unsigned int recordSize = 0;
    fread(&recordSize, sizeof(unsigned int), 1, pFile);
    fprintf(f, "recordSize = %u\n", recordSize);

    char* buffer = new char[recordSize];
    fread(buffer, sizeof(char) * recordSize, 1, pFile);

    char* buf = buffer;

    unsigned int questId = *((unsigned int*)buf);
    fprintf(f, "questId = %u\n", questId);
    buf += 4;

    unsigned int questType = *((unsigned int*)buf);
    fprintf(f, "questType = %u\n", questType);
    buf += 4;

    unsigned int questLevel = *((unsigned int*)buf);
    fprintf(f, "questLevel = %u\n", questLevel);
    buf += 4;

    unsigned int questScalingFactionGroup = *((unsigned int*)buf);
    fprintf(f, "questScalingFactionGroup = %u\n", questScalingFactionGroup);
    buf += 4;

    unsigned int questMaxScalingLevel = *((unsigned int*)buf);
    fprintf(f, "questMaxScalingLevel = %u\n", questMaxScalingLevel);
    buf += 4;

    unsigned int questPackageID = *((unsigned int*)buf);
    fprintf(f, "questPackageID = %u\n", questPackageID);
    buf += 4;

    unsigned int questMinLevel = *((unsigned int*)buf);
    fprintf(f, "questMinLevel = %u\n", questMinLevel);
    buf += 4;

    unsigned int questSortID = *((unsigned int*)buf);
    fprintf(f, "questSortID = %u\n", questSortID);
    buf += 4;

    unsigned int questInfoID = *((unsigned int*)buf);
    fprintf(f, "questInfoID = %u\n", questInfoID);
    buf += 4;

    unsigned int suggestedGroupNum = *((unsigned int*)buf);
    fprintf(f, "suggestedGroupNum = %u\n", suggestedGroupNum);
    buf += 4;

    unsigned int rewardNextQuest = *((unsigned int*)buf);
    fprintf(f, "rewardNextQuest = %u\n", rewardNextQuest);
    buf += 4;

    unsigned int rewardXPDifficulty = *((unsigned int*)buf);
    fprintf(f, "rewardXPDifficulty = %u\n", rewardXPDifficulty);
    buf += 4;

    float rewardXPMultiplier = *((float*)buf);
    fprintf(f, "rewardXPMultiplier = %f\n", rewardXPMultiplier);
    buf += 4;

    unsigned int rewardMoney = *((unsigned int*)buf);
    fprintf(f, "rewardMoney = %u\n", rewardMoney);
    buf += 4;

    unsigned int rewardMoneyDifficulty = *((unsigned int*)buf);
    fprintf(f, "rewardMoneyDifficulty = %u\n", rewardMoneyDifficulty);
    buf += 4;

    float rewardMoneyMultiplier = *((float*)buf);
    fprintf(f, "rewardMoneyMultiplier = %f\n", rewardMoneyMultiplier);
    buf += 4;

    unsigned int rewardBonusMoney = *((unsigned int*)buf);
    fprintf(f, "rewardBonusMoney = %u\n", rewardBonusMoney);
    buf += 4;

    unsigned int rewardDisplaySpell[3] = {};
    for (int i = 0; i < 3; i++)
    {
        rewardDisplaySpell[i] = *((unsigned int*)buf);
        fprintf(f, "rewardDisplaySpell[%i] = %u\n", i, rewardDisplaySpell[i]);
        buf += 4;
    }
    
    unsigned int rewardSpell = *((unsigned int*)buf);
    fprintf(f, "rewardSpell = %u\n", rewardSpell);
    buf += 4;

    unsigned int rewardHonor = *((unsigned int*)buf);
    fprintf(f, "rewardHonor = %u\n", rewardHonor);
    buf += 4;

    float rewardHonorKill = *((float*)buf);
    fprintf(f, "rewardHonorKill = %f\n", rewardHonorKill);
    buf += 4;

    unsigned int rewardArtifactXPDifficulty = *((unsigned int*)buf);
    fprintf(f, "rewardArtifactXPDifficulty = %u\n", rewardArtifactXPDifficulty);
    buf += 4;

    float rewardArtifactXPMultiplier = *((float*)buf);
    fprintf(f, "rewardArtifactXPMultiplier = %f\n", rewardArtifactXPMultiplier);
    buf += 4;

    unsigned int rewardArtifactCategoryID = *((unsigned int*)buf);
    fprintf(f, "rewardArtifactCategoryID = %u\n", rewardArtifactCategoryID);
    buf += 4;

    unsigned int providedItem = *((unsigned int*)buf);
    fprintf(f, "providedItem = %u\n", providedItem);
    buf += 4;

    unsigned int flags1 = *((unsigned int*)buf);
    fprintf(f, "flags1 = %u\n", flags1);
    buf += 4;

    unsigned int flags2 = *((unsigned int*)buf);
    fprintf(f, "flags2 = %u\n", flags2);
    buf += 4;

    unsigned int flags3 = *((unsigned int*)buf);
    fprintf(f, "flags3 = %u\n", flags3);
    buf += 4;

    struct {
        int ItemID;
        int Quantity;
    } rewardFixedItems[4];
    for (int i = 0; i < 4; i++)
    {
        rewardFixedItems[i].ItemID = *((unsigned int*)buf);
        fprintf(f, "rewardFixedItems[%i].ItemID = %i\n", i, rewardFixedItems[i].ItemID);
        buf += 4;
        rewardFixedItems[i].Quantity = *((unsigned int*)buf);
        fprintf(f, "rewardFixedItems[%i].Quantity = %i\n", i, rewardFixedItems[i].Quantity);
        buf += 4;
    }

    struct {
        int ItemID;
        int Quantity;
    } itemDrop[4];
    for (int i = 0; i < 4; i++)
    {
        itemDrop[i].ItemID = *((unsigned int*)buf);
        fprintf(f, "itemDrop[%i].ItemID = %i\n", i, itemDrop[i].ItemID);
        buf += 4;
        itemDrop[i].Quantity = *((unsigned int*)buf);
        fprintf(f, "itemDrop[%i].Quantity = %i\n", i, itemDrop[i].Quantity);
        buf += 4;
    }

    struct {
        int ItemID;
        int Quantity;
        int DisplayID;
    } rewardChoiceItems[6];
    for (int i = 0; i < 6; i++)
    {
        rewardChoiceItems[i].ItemID = *((unsigned int*)buf);
        fprintf(f, "rewardChoiceItems[%i].ItemID = %i\n", i, rewardChoiceItems[i].ItemID);
        buf += 4;
        rewardChoiceItems[i].Quantity = *((unsigned int*)buf);
        fprintf(f, "rewardChoiceItems[%i].Quantity = %i\n", i, rewardChoiceItems[i].Quantity);
        buf += 4;
        rewardChoiceItems[i].DisplayID = *((unsigned int*)buf);
        fprintf(f, "rewardChoiceItems[%i].DisplayID = %i\n", i, rewardChoiceItems[i].DisplayID);
        buf += 4;
    }

    unsigned int poiContinent = *((unsigned int*)buf);
    fprintf(f, "poiContinent = %u\n", poiContinent);
    buf += 4;

    float poiX = *((float*)buf);
    fprintf(f, "poiX = %f\n", poiX);
    buf += 4;

    float poiY = *((float*)buf);
    fprintf(f, "poiY = %f\n", poiY);
    buf += 4;

    unsigned int poiPriority = *((unsigned int*)buf);
    fprintf(f, "poiPriority = %u\n", poiPriority);
    buf += 4;

    unsigned int rewardTitle = *((unsigned int*)buf);
    fprintf(f, "rewardTitle = %u\n", rewardTitle);
    buf += 4;

    unsigned int rewardArenaPoints = *((unsigned int*)buf);
    fprintf(f, "rewardArenaPoints = %u\n", rewardArenaPoints);
    buf += 4;

    unsigned int rewardSkillLineID = *((unsigned int*)buf);
    fprintf(f, "rewardSkillLineID = %u\n", rewardSkillLineID);
    buf += 4;

    unsigned int rewardNumSkillUps = *((unsigned int*)buf);
    fprintf(f, "rewardNumSkillUps = %u\n", rewardNumSkillUps);
    buf += 4;

    unsigned int portraitGiverDisplayID = *((unsigned int*)buf);
    fprintf(f, "portraitGiverDisplayID = %u\n", portraitGiverDisplayID);
    buf += 4;

    unsigned int portraitGiverMount = *((unsigned int*)buf);
    fprintf(f, "portraitGiverMount  = %u\n", portraitGiverMount);
    buf += 4;

    unsigned int portraitTurnInDisplayID = *((unsigned int*)buf);
    fprintf(f, "portraitTurnInDisplayID  = %u\n", portraitTurnInDisplayID);
    buf += 4;

    struct {
        int FactionID;
        int FactionValue;
        int FactionOverride;
        int FactionGainMaxRank;
    } rewardFaction[5];
    for (int i = 0; i < 5; i++)
    {
        rewardFaction[i].FactionID = *((unsigned int*)buf);
        fprintf(f, "rewardFaction[%i].FactionID = %i\n", i, rewardFaction[i].FactionID);
        buf += 4;
        rewardFaction[i].FactionValue = *((unsigned int*)buf);
        fprintf(f, "rewardFaction[%i].FactionValue = %i\n", i, rewardFaction[i].FactionValue);
        buf += 4;
        rewardFaction[i].FactionOverride = *((unsigned int*)buf);
        fprintf(f, "rewardFaction[%i].FactionOverride = %i\n", i, rewardFaction[i].FactionOverride);
        buf += 4;
        rewardFaction[i].FactionGainMaxRank = *((unsigned int*)buf);
        fprintf(f, "rewardFaction[%i].FactionGainMaxRank = %i\n", i, rewardFaction[i].FactionGainMaxRank);
        buf += 4;
    }

    unsigned int rewardFactionFlags = *((unsigned int*)buf);
    fprintf(f, "rewardFactionFlags  = %u\n", rewardFactionFlags);
    buf += 4;

    struct {
        int CurrencyID;
        int Quantity;
    } rewardCurrency[4];
    for (int i = 0; i < 4; i++)
    {
        rewardCurrency[i].CurrencyID = *((unsigned int*)buf);
        fprintf(f, "rewardCurrency[%i].CurrencyID = %i\n", i, rewardCurrency[i].CurrencyID);
        buf += 4;
        rewardCurrency[i].Quantity = *((unsigned int*)buf);
        fprintf(f, "rewardCurrency[%i].Quantity = %i\n", i, rewardCurrency[i].Quantity);
        buf += 4;
    }

    unsigned int acceptedSoundKitID = *((unsigned int*)buf);
    fprintf(f, "acceptedSoundKitID  = %u\n", acceptedSoundKitID);
    buf += 4;

    unsigned int completeSoundKitID = *((unsigned int*)buf);
    fprintf(f, "completeSoundKitID  = %u\n", completeSoundKitID);
    buf += 4;

    unsigned int areaGroupID = *((unsigned int*)buf);
    fprintf(f, "areaGroupID  = %u\n", areaGroupID);
    buf += 4;

    unsigned int timeAllowed = *((unsigned int*)buf);
    fprintf(f, "timeAllowed  = %u\n", timeAllowed);
    buf += 4;

    unsigned int objectivesCount = *((unsigned int*)buf);
    fprintf(f, "objectivesCount  = %u\n", objectivesCount);
    buf += 4;

    unsigned long long allowableRaces = *((unsigned long long*)buf);
    fprintf(f, "allowableRaces  = %llu\n", allowableRaces);
    buf += 8;

    unsigned int questRewardID = *((unsigned int*)buf);
    fprintf(f, "questRewardID  = %u\n", questRewardID);
    buf += 4;

    unsigned int expansionId = *((unsigned int*)buf);
    fprintf(f, "expansionId  = %u\n", expansionId);
    buf += 4;
    
    // 9 bits
    char logTitleLengthBytes[2];
    logTitleLengthBytes[1] = *buf;
    buf++;
    logTitleLengthBytes[0] = *buf;
    unsigned short logTitleLength = (*((unsigned short*)logTitleLengthBytes) >> 7) & 0b111111111;
    fprintf(f, "logTitleLength = %hu\n", logTitleLength);
    
    // 12 bits
    char logDescriptionLengthBytes[2];
    logDescriptionLengthBytes[1] = *buf;
    buf++;
    logDescriptionLengthBytes[0] = *buf;
    unsigned short logDescriptionLength = (*((unsigned short*)logDescriptionLengthBytes) >> 3) & 0b111111111111;
    fprintf(f, "logDescriptionLength = %hu\n", logDescriptionLength);
    
    // 12 bits
    char questDescriptionLengthBytes[4];
    questDescriptionLengthBytes[3] = 0;
    questDescriptionLengthBytes[2] = *buf;
    buf++;
    questDescriptionLengthBytes[1] = *buf;
    buf++;
    questDescriptionLengthBytes[0] = *buf;
    unsigned int questDescriptionLength = (*((unsigned int*)questDescriptionLengthBytes) >> 7) & 0b111111111111;
    fprintf(f, "questDescriptionLength = %u\n", questDescriptionLength);
    
    // 9 bits
    char areaDescriptionLengthBytes[2];
    areaDescriptionLengthBytes[1] = *buf;
    buf++;
    areaDescriptionLengthBytes[0] = *buf;
    unsigned short areaDescriptionLength = (*((unsigned short*)areaDescriptionLengthBytes) >> 6) & 0b111111111;
    fprintf(f, "areaDescriptionLength = %hu\n", areaDescriptionLength);
    
    // 10 bits
    char portraitGiverTextLengthBytes[2];
    portraitGiverTextLengthBytes[1] = *buf;
    buf++;
    portraitGiverTextLengthBytes[0] = *buf;
    unsigned short portraitGiverTextLength = (*((unsigned short*)portraitGiverTextLengthBytes) >> 4) & 0b1111111111;
    fprintf(f, "portraitGiverTextLength = %hu\n", portraitGiverTextLength);
    
    // 8 bits
    char portraitGiverNameLengthBytes[2];
    portraitGiverNameLengthBytes[1] = *buf;
    buf++;
    portraitGiverNameLengthBytes[0] = *buf;
    unsigned short portraitGiverNameLength = (*((unsigned short*)portraitGiverNameLengthBytes) >> 4) & 0b11111111;
    fprintf(f, "portraitGiverNameLength = %hu\n", portraitGiverNameLength);
    
    // 10 bits
    char portraitTurnInTextLengthBytes[2];
    portraitTurnInTextLengthBytes[1] = *buf;
    buf++;
    portraitTurnInTextLengthBytes[0] = *buf;
    unsigned short portraitTurnInTextLength = (*((unsigned short*)portraitTurnInTextLengthBytes) >> 2) & 0b1111111111;
    fprintf(f, "portraitTurnInTextLength = %hu\n", portraitTurnInTextLength);
    
    // 8 bits
    char portraitTurnInNameLengthBytes[2];
    portraitTurnInNameLengthBytes[1] = *buf;
    buf++;
    portraitTurnInNameLengthBytes[0] = *buf;
    unsigned short portraitTurnInNameLength = (*((unsigned short*)portraitTurnInNameLengthBytes) >> 2) & 0b11111111;
    fprintf(f, "portraitTurnInNameLength = %hu\n", portraitTurnInNameLength);
    
    // 11 bits
    char questCompletionLogLengthBytes[4];
    questCompletionLogLengthBytes[3] = 0;
    questCompletionLogLengthBytes[2] = *buf;
    buf++;
    questCompletionLogLengthBytes[1] = *buf;
    buf++;
    questCompletionLogLengthBytes[0] = *buf;
    unsigned int questCompletionLogLength = (*((unsigned int*)questCompletionLogLengthBytes) >> 7) & 0b11111111111;
    fprintf(f, "questCompletionLogLength = %u\n", questCompletionLogLength);
    buf++;

    for (unsigned int i = 0; i < objectivesCount; ++i)
    {
        unsigned int objectiveId = *((unsigned int*)buf);
        fprintf(f, "objectives[%u].Id  = %u\n", i, objectiveId);
        buf += 4;
        unsigned char objectiveType = *((unsigned char*)buf);
        fprintf(f, "objectives[%u].Type  = %hhu\n", i, objectiveType);
        buf += 1;
        char storageIndex = *buf;
        fprintf(f, "objectives[%u].StorageIndex  = %hhi\n", i, storageIndex);
        buf += 1;
        unsigned int objectId = *((unsigned int*)buf);
        fprintf(f, "objectives[%u].ObjectId  = %u\n", i, objectId);
        buf += 4;
        unsigned int amount = *((unsigned int*)buf);
        fprintf(f, "objectives[%u].Amount  = %u\n", i, amount);
        buf += 4;
        unsigned int objectiveFlags1 = *((unsigned int*)buf);
        fprintf(f, "objectives[%u].Flags  = %u\n", i, objectiveFlags1);
        buf += 4;
        unsigned int objectiveFlags2 = *((unsigned int*)buf);
        fprintf(f, "objectives[%u].Flags2  = %u\n", i, objectiveFlags2);
        buf += 4;
        float progressBarWeight = *((float*)buf);
        fprintf(f, "objectives[%u].ProgressBarWeight  = %f\n", i, progressBarWeight);
        buf += 4;
        unsigned int visualEffectsCount = *((unsigned int*)buf);
        fprintf(f, "objectives[%u].VisualEffectsCount = %u\n", i, visualEffectsCount);
        buf += 4;
        for (unsigned int j = 0; j < visualEffectsCount; ++j)
        {
            unsigned int visualEffectId = *((unsigned int*)buf);
            fprintf(f, "objectives[%u].visualEffectId[%u] = %u\n", i, j, visualEffectId);
            buf += 4;
        }
        unsigned char descriptionLength = *((unsigned char*)buf);
        fprintf(f, "objectives[%u].descriptionLength  = %hhu\n", i, descriptionLength);
        buf += 1;
        if (descriptionLength)
        {
            std::string objectiveDescription;
            for (unsigned int i = 0; i < descriptionLength; i++)
            {
                objectiveDescription += *buf;
                buf++;
            }
            fprintf(f, "objectives[%u].description = %s\n", i, objectiveDescription.c_str());
        }
    }

    if (logTitleLength)
    {
        std::string logTitle;
        for (int i = 0; i < logTitleLength; i++)
        {
            logTitle += *buf;
            buf++;
        }
        fprintf(f, "logTitle = %s\n", logTitle.c_str());
    }

    if (logDescriptionLength)
    {
        std::string logDescription;
        for (int i = 0; i < logDescriptionLength; i++)
        {
            logDescription += *buf;
            buf++;
        }
        fprintf(f, "logDescription = %s\n", logDescription.c_str());
    }

    if (questDescriptionLength)
    {
        std::string questDescription;
        for (unsigned int i = 0; i < questDescriptionLength; i++)
        {
            questDescription += *buf;
            buf++;
        }
        fprintf(f, "questDescription = %s\n", questDescription.c_str());
    }

    if (areaDescriptionLength)
    {
        std::string areaDescription;
        for (unsigned int i = 0; i < areaDescriptionLength; i++)
        {
            areaDescription += *buf;
            buf++;
        }
        fprintf(f, "areaDescription = %s\n", areaDescription.c_str());
    }

    if (portraitGiverTextLength)
    {
        std::string portraitGiverText;
        for (unsigned int i = 0; i < portraitGiverTextLength; i++)
        {
            portraitGiverText += *buf;
            buf++;
        }
        fprintf(f, "portraitGiverText = %s\n", portraitGiverText.c_str());
    }

    if (portraitGiverNameLength)
    {
        std::string portraitGiverName;
        for (unsigned int i = 0; i < portraitGiverNameLength; i++)
        {
            portraitGiverName += *buf;
            buf++;
        }
        fprintf(f, "portraitGiverName = %s\n", portraitGiverName.c_str());
    }

    if (portraitTurnInTextLength)
    {
        std::string portraitTurnInText;
        for (unsigned int i = 0; i < portraitTurnInTextLength; i++)
        {
            portraitTurnInText += *buf;
            buf++;
        }
        fprintf(f, "portraitTurnInText = %s\n", portraitTurnInText.c_str());
    }

    if (portraitTurnInNameLength)
    {
        std::string portraitTurnInName;
        for (unsigned int i = 0; i < portraitTurnInNameLength; i++)
        {
            portraitTurnInName += *buf;
            buf++;
        }
        fprintf(f, "portraitTurnInName = %s\n", portraitTurnInName.c_str());
    }

    if (questCompletionLogLength)
    {
        std::string questCompletionLog;
        for (unsigned int i = 0; i < questCompletionLogLength; i++)
        {
            questCompletionLog += *buf;
            buf++;
        }
        fprintf(f, "questCompletionLog = %s\n", questCompletionLog.c_str());
    }

    int remainingBytes = recordSize - (buf - buffer);
    fprintf(f, "remaining bytes: %i\n", remainingBytes);
    for (int i = 0; i < remainingBytes; i++)
    {
        fprintf(f, "remainingByte[%i] = %hhu\n", i, *(buf++));
    }

    delete[] buffer;
    fclose(f);
}

template <typename T>
void ReverseArray(T arr[], size_t size)
{
    for (size_t i = 0; i < size / 2; ++i)
    {
        T temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

enum
{
    CREATURE_CACHE,
    GAMEOBJECT_CACHE,
    QUEST_CACHE,
    PAGETEXT_CACHE,
    NPCTEXT_CACHE,
};

int main()
{
    printf("Choose file to read:\n");
    printf("1. creaturecache.wdb\n");
    printf("2. gameobjectcache.wdb\n");
    printf("3. questcache.wdb\n");
    printf("4. pagetextcache.wdb\n");
    printf("5. npccache.wdb\n");
    printf("> ");

    unsigned int option = 0;
    char const* fileName = nullptr;

    switch (getchar())
    {
        case '1':
        {
            option = CREATURE_CACHE;
            fileName = "creaturecache.wdb";
            break;
        }
        case '2':
        {
            option = GAMEOBJECT_CACHE;
            fileName = "gameobjectcache.wdb";
            break;
        }
        case '3':
        {
            option = QUEST_CACHE;
            fileName = "questcache.wdb";
            break;
        }
        case '4':
        {
            option = PAGETEXT_CACHE;
            fileName = "pagetextcache.wdb";
            break;
        }
        case '5':
        {
            option = NPCTEXT_CACHE;
            fileName = "npccache.wdb";
            break;
        }
        default:
        {
            printf("Invalid option.");
            exit(0);
        }
    }

    printf("\n");

    FILE* pFile = fopen(fileName, "rb");
    if (pFile == nullptr)
    {
        printf("Failed to read file.\n");
        return false;
    }

    fseek(pFile, 0L, SEEK_END);
    long size = ftell(pFile);
    fseek(pFile, 0L, SEEK_SET);

    char signature[5] = {};
    fread(signature, 4, 1, pFile);
    ReverseArray(signature, 4);
    printf("Signature: %s\n", signature);

    unsigned int clientBuild = 0;
    fread(&clientBuild, sizeof(unsigned int), 1, pFile);
    printf("Client Build: %u\n", clientBuild);

    char clientLocale[5] = {};
    fread(clientLocale, 4, 1, pFile);
    ReverseArray(clientLocale, 4);
    printf("Client Locale: %s\n", clientLocale);

    unsigned int recordSize = 0;
    fread(&recordSize, sizeof(unsigned int), 1, pFile);
    printf("Record Size: %u\n", recordSize);

    unsigned int recordVersion = 0;
    fread(&recordVersion, sizeof(unsigned int), 1, pFile);
    printf("Record Version: %u\n", recordVersion);

    unsigned int cacheVersion = 0;
    fread(&cacheVersion, sizeof(unsigned int), 1, pFile);
    printf("Cache Version: %u\n", cacheVersion);

    while (ftell(pFile) < size - 20)
    {
        switch (option)
        {
            case CREATURE_CACHE:
                ReadCreatureEntry(pFile);
                break;
            case GAMEOBJECT_CACHE:
                ReadGameObjectEntry(pFile);
                break;
            case QUEST_CACHE:
                ReadQuestEntry(pFile);
                break;
            case PAGETEXT_CACHE:
                ReadPageTextEntry(pFile);
                break;
            case NPCTEXT_CACHE:
                ReadNpcTextEntry(pFile);
                break;
        }
    }

    printf("\nDone!\n");
    printf("Records read: %u\n", g_counter);

    switch (option)
    {
        case CREATURE_CACHE:
            //ReadCreatureEntry(pFile);
            break;
        case GAMEOBJECT_CACHE:
            WriteGameObjects();
            break;
        case QUEST_CACHE:
            //ReadQuestEntry(pFile);
            break;
        case PAGETEXT_CACHE:
            WritePageTexts();
            break;
        case NPCTEXT_CACHE:
            WriteNpcTexts();
            break;
    }
    
    printf("\nData has been exported!\n");

    fseek(stdin, 0, SEEK_END);
    getchar();
    return 0;

}