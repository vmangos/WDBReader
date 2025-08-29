#ifndef H_ITEM
#define H_ITEM

#include "Defines.h"
#include "EscapeString.h"
#include "ByteBuffer.h"
#include "ClientBuilds.h"

#define MAX_ITEM_PROTO_DAMAGES 5
#define MAX_ITEM_PROTO_SPELLS  5
#define MAX_ITEM_PROTO_STATS  10

struct Item
{
    uint32 entry = 0;
    uint32 itemClass = 0;
    uint32 subClass = 0;
    std::string name[4];
    uint32 displayId = 0;
    uint32 quality = 0;
    uint32 flags = 0;
    uint32 buyPrice = 0;
    uint32 sellPrice = 0;
    uint32 inventoryType = 0;
    int32 allowableClass = 0;
    int32 allowableRace = 0;
    uint32 itemLevel = 0;
    uint32 requiredLevel = 0;
    uint32 requiredSkill = 0;
    uint32 requiredSkillRank = 0;
    uint32 requiredSpell = 0;
    uint32 requiredHonorRank = 0;
    uint32 requiredCityRank = 0;
    uint32 requiredReputationFaction = 0;
    uint32 requiredReputationRank = 0;
    uint32 maxCount = 0;
    uint32 stackable = 0;
    uint32 containerSlots = 0;

    struct ItemStat
    {
        int32 statType = 0;
        int32 statValue = 0;
    };
    ItemStat itemStats[MAX_ITEM_PROTO_STATS] = {};

    struct ItemDamage
    {
        float damageMin = 0;
        float damageMax = 0;
        uint32 damageType = 0;
    };
    ItemDamage itemDamages[MAX_ITEM_PROTO_DAMAGES] = {};

    int32 armor = 0;
    int32 holyRes = 0;
    int32 fireRes = 0;
    int32 natureRes = 0;
    int32 frostRes = 0;
    int32 shadowRes = 0;
    int32 arcaneRes = 0;

    uint32 delay = 0;
    uint32 ammoType = 0;
    float rangedModRange;

    struct ItemSpell
    {
        uint32 spellId = 0;
        uint32 spellTrigger = 0;
        int32  spellCharges = 0;
        int32  spellCooldown = 0;
        uint32 spellCategory = 0;
        int32  spellCategoryCooldown = 0;
    };
    ItemSpell itemSpells[MAX_ITEM_PROTO_SPELLS] = {};

    uint32 bonding = 0;
    std::string description;
    uint32 pageText = 0;
    uint32 pageLanguage = 0;
    uint32 pageMaterial = 0;
    uint32 startQuest = 0;
    uint32 lockId = 0;
    int32 material = 0;
    uint32 sheath = 0;
    uint32 randomProperty = 0;
    uint32 block = 0;
    uint32 itemSet = 0;
    uint32 maxDurability = 0;
    uint32 areaBound = 0;
    uint32 mapBound = 0;
    uint32 bagFamily = 0;

    bool ReadEntry(ByteBuffer& buffer)
    {
        buffer >> entry;
        if (!entry)
            return false;

        uint32 entrySize;
        buffer >> entrySize;

        if (!entrySize)
            return false;

        buffer >> itemClass;
        buffer >> subClass;

        for (uint32 i = 0; i < 4; ++i)
            buffer >> name[i];

        buffer >> displayId;
        buffer >> quality;
        buffer >> flags;
        buffer >> buyPrice;
        buffer >> sellPrice;
        buffer >> inventoryType;
        buffer >> allowableClass;
        buffer >> allowableRace;
        buffer >> itemLevel;
        buffer >> requiredLevel;
        buffer >> requiredSkill;
        buffer >> requiredSkillRank;
        buffer >> requiredSpell;
        buffer >> requiredHonorRank;
        buffer >> requiredCityRank;
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            buffer >> requiredReputationFaction;
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            buffer >> requiredReputationRank;
        buffer >> maxCount;
        buffer >> stackable;
        buffer >> containerSlots;
        for (auto& i : itemStats)
        {
            buffer >> i.statType;
            buffer >> i.statValue;
        }
        for (auto& i : itemDamages)
        {
            buffer >> i.damageMin;
            buffer >> i.damageMax;
            buffer >> i.damageType;
        }
        buffer >> armor;
        buffer >> holyRes;
        buffer >> fireRes;
        buffer >> natureRes;
        buffer >> frostRes;
        buffer >> shadowRes;
        buffer >> arcaneRes;

        buffer >> delay;
        buffer >> ammoType;
        if (g_clientBuild >= CLIENT_BUILD_1_10_0)
            buffer >> rangedModRange;

        for (auto& i : itemSpells)
        {
            buffer >> i.spellId;
            buffer >> i.spellTrigger;
            buffer >> i.spellCharges;
            buffer >> i.spellCooldown;
            buffer >> i.spellCategory;
            buffer >> i.spellCategoryCooldown;
        }

        buffer >> bonding;
        buffer >> description;
        buffer >> pageText;
        buffer >> pageLanguage;
        buffer >> pageMaterial;
        buffer >> startQuest;
        buffer >> lockId;
        buffer >> material;
        buffer >> sheath;
        buffer >> randomProperty;
        buffer >> block;
        buffer >> itemSet;
        buffer >> maxDurability;
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            buffer >> areaBound;
        if (g_clientBuild >= CLIENT_BUILD_1_11_0)
            buffer >> mapBound;
        if (g_clientBuild >= CLIENT_BUILD_1_9_0)
            buffer >> bagFamily;

        return true;
    }

    void WriteSQLRow(FILE*& f) const
    {
        fprintf(f, "(%u", entry);
        fprintf(f, ", %u", itemClass);
        fprintf(f, ", %u", subClass);

        for (uint32 i = 0; i < 4; ++i)
            fprintf(f, ", '%s'", EscapeString(name[i]).c_str());

        fprintf(f, ", %u", displayId);
        fprintf(f, ", %u", quality);
        fprintf(f, ", %u", flags);
        fprintf(f, ", %u", buyPrice);
        fprintf(f, ", %u", sellPrice);
        fprintf(f, ", %u", inventoryType);
        fprintf(f, ", %i", allowableClass);
        fprintf(f, ", %i", allowableRace);
        fprintf(f, ", %u", itemLevel);
        fprintf(f, ", %u", requiredLevel);
        fprintf(f, ", %u", requiredSkill);
        fprintf(f, ", %u", requiredSkillRank);
        fprintf(f, ", %u", requiredSpell);
        fprintf(f, ", %u", requiredHonorRank);
        fprintf(f, ", %u", requiredCityRank);
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", %u", requiredReputationFaction);
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", %u", requiredReputationRank);
        fprintf(f, ", %u", maxCount);
        fprintf(f, ", %u", stackable);
        fprintf(f, ", %u", containerSlots);

        for (auto& i : itemStats)
        {
            fprintf(f, ", %i", i.statType);
            fprintf(f, ", %i", i.statValue);
        }
        for (auto& i : itemDamages)
        {
            fprintf(f, ", %g", i.damageMin);
            fprintf(f, ", %g", i.damageMax);
            fprintf(f, ", %u", i.damageType);
        }

        fprintf(f, ", %i", armor);
        fprintf(f, ", %i", holyRes);
        fprintf(f, ", %i", fireRes);
        fprintf(f, ", %i", natureRes);
        fprintf(f, ", %i", frostRes);
        fprintf(f, ", %i", shadowRes);
        fprintf(f, ", %i", arcaneRes);

        fprintf(f, ", %u", delay);
        fprintf(f, ", %u", ammoType);
        if (g_clientBuild >= CLIENT_BUILD_1_10_0)
            fprintf(f, ", %g", rangedModRange);

        for (auto& i : itemSpells)
        {
            fprintf(f, ", %u", i.spellId);
            fprintf(f, ", %u", i.spellTrigger);
            fprintf(f, ", %i", i.spellCharges);
            fprintf(f, ", %i", i.spellCooldown);
            fprintf(f, ", %u", i.spellCategory);
            fprintf(f, ", %i", i.spellCategoryCooldown);
        }

        fprintf(f, ", %u", bonding);
        fprintf(f, ", '%s'", EscapeString(description).c_str());
        fprintf(f, ", %u", pageText);
        fprintf(f, ", %u", pageLanguage);
        fprintf(f, ", %u", pageMaterial);
        fprintf(f, ", %u", startQuest);
        fprintf(f, ", %u", lockId);
        fprintf(f, ", %i", material);
        fprintf(f, ", %u", sheath);
        fprintf(f, ", %u", randomProperty);
        fprintf(f, ", %u", block);
        fprintf(f, ", %u", itemSet);
        fprintf(f, ", %u", maxDurability);
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", %u", areaBound);
        if (g_clientBuild >= CLIENT_BUILD_1_11_0)
            fprintf(f, ", %u", mapBound);
        if (g_clientBuild >= CLIENT_BUILD_1_9_0)
            fprintf(f, ", %u", bagFamily);

        fprintf(f, ")");
    }

    static void WriteToSQL(std::vector<Item> const& vItems)
    {
        FILE* f = fopen("wdb_item_template.sql", "w");
        fprintf(f, "REPLACE INTO `wdb_item_template` (`entry`, `class`, `subclass`, `name`, `name2`, `name3`, `name4`, `subname`");

        fprintf(f, ", `class`");
        fprintf(f, ", `subclass`");
        fprintf(f, ", `name`");
        fprintf(f, ", `name2`");
        fprintf(f, ", `name3`");
        fprintf(f, ", `name4`");
        fprintf(f, ", `display_id`");
        fprintf(f, ", `quality`");
        fprintf(f, ", `flags`");
        fprintf(f, ", `buy_price`");
        fprintf(f, ", `sell_price`");
        fprintf(f, ", `inventory_type`");
        fprintf(f, ", `allowable_class`");
        fprintf(f, ", `allowable_race`");
        fprintf(f, ", `item_level`");
        fprintf(f, ", `required_level`");
        fprintf(f, ", `required_skill`");
        fprintf(f, ", `required_skill_rank`");
        fprintf(f, ", `required_spell`");
        fprintf(f, ", `required_honor_rank`");
        fprintf(f, ", `required_city_rank`");

        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", `required_reputation_faction`");
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", `required_reputation_rank`");

        fprintf(f, ", `max_count`");
        fprintf(f, ", `stackable`");
        fprintf(f, ", `container_slots`");

        for (uint32 i = 1; i <= MAX_ITEM_PROTO_STATS; ++i)
            fprintf(f, ", `stat_type%u`, `stat_value%u`", i, i);

        for (uint32 i = 1; i <= MAX_ITEM_PROTO_DAMAGES; ++i)
            fprintf(f, ", `dmg_min%u`, `dmg_max%u`, `dmg_type%u`", i, i, i);

        fprintf(f, ", `armor`");
        fprintf(f, ", `holy_res`");
        fprintf(f, ", `fire_res`");
        fprintf(f, ", `nature_res`");
        fprintf(f, ", `frost_res`");
        fprintf(f, ", `shadow_res`");
        fprintf(f, ", `arcane_res`");

        fprintf(f, ", `delay`");
        fprintf(f, ", `ammo_type`");
        if (g_clientBuild >= CLIENT_BUILD_1_10_0)
            fprintf(f, ", `range_mod`");

        for (uint32 i = 1; i <= MAX_ITEM_PROTO_SPELLS; ++i)
            fprintf(f, ", `spellid_%u`, `spelltrigger_%u`, `spellcharges_%u`, `spellcooldown_%u`, `spellcategory_%u`, `spellcategorycooldown_%u`", i, i, i, i, i, i);

        fprintf(f, ", `bonding`");
        fprintf(f, ", `description`");
        fprintf(f, ", `page_text`");
        fprintf(f, ", `page_language`");
        fprintf(f, ", `page_material`");
        fprintf(f, ", `start_quest`");
        fprintf(f, ", `lock_id`");
        fprintf(f, ", `material`");
        fprintf(f, ", `sheath`");
        fprintf(f, ", `random_property`");
        fprintf(f, ", `block`");
        fprintf(f, ", `set_id`");
        fprintf(f, ", `max_durability`");
        if (g_clientBuild >= CLIENT_BUILD_1_7_0)
            fprintf(f, ", `area_bound`");
        if (g_clientBuild >= CLIENT_BUILD_1_11_0)
            fprintf(f, ", `map_bound`");
        if (g_clientBuild >= CLIENT_BUILD_1_9_0)
            fprintf(f, ", `bag_family`");

        fprintf(f, ") VALUES\n");

        uint32 count = 0;
        for (auto const& itemEntry : vItems)
        {
            count++;
            if (count > 1)
                fprintf(f, ",\n");

            itemEntry.WriteSQLRow(f);
        }

        fprintf(f, ";\n");
        fclose(f);
    }
};

#endif