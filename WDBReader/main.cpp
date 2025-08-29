#include "Defines.h"
#include "Creature.h"
#include "GameObject.h"
#include "Item.h"
#include "Quest.h"
#include "NpcText.h"
#include "PageText.h"
#include "ClientBuilds.h"
#include "stdio.h"
#include <string>
#include <vector>

uint32 g_clientBuild = 0;
unsigned int g_counter = 0;

std::vector<Creature> g_vCreatures;
std::vector<GameObject> g_vGameObjects;
std::vector<Item> g_vItems;
std::vector<Quest> g_vQuests;
std::vector<NpcText> g_vNpcTexts;
std::vector<PageText> g_vPageTexts;

template <class T>
void ReadAllRecords(ByteBuffer& buffer, std::vector<T>& records)
{
    bool readMore;
    do
    {
        T wdbEntry;
        if (readMore = wdbEntry.ReadEntry(buffer))
        {
            ++g_counter;
            records.push_back(wdbEntry);
        }
    } while (readMore);
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
    ITEM_CACHE,
    QUEST_CACHE,
    PAGETEXT_CACHE,
    NPCTEXT_CACHE,
};

int main()
{
    printf("Choose file to read:\n");
    printf("1. creaturecache.wdb\n");
    printf("2. gameobjectcache.wdb\n");
    printf("3. itemcache.wdb\n");
    printf("4. questcache.wdb\n");
    printf("5. pagetextcache.wdb\n");
    printf("6. npccache.wdb\n");
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
            option = ITEM_CACHE;
            fileName = "itemcache.wdb";
            break;
        }
        case '4':
        {
            option = QUEST_CACHE;
            fileName = "questcache.wdb";
            break;
        }
        case '5':
        {
            option = PAGETEXT_CACHE;
            fileName = "pagetextcache.wdb";
            break;
        }
        case '6':
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

    fread(&g_clientBuild, sizeof(unsigned int), 1, pFile);
    printf("Client Build: %u\n", g_clientBuild);

    if (g_clientBuild >= CLIENT_BUILD_1_6_0)
    {
        char clientLocale[5] = {};
        fread(clientLocale, 4, 1, pFile);
        ReverseArray(clientLocale, 4);
        printf("Client Locale: %s\n", clientLocale);
    }

    unsigned int recordSize = 0;
    fread(&recordSize, sizeof(unsigned int), 1, pFile);
    printf("Record Size: %u\n", recordSize);

    unsigned int recordVersion = 0;
    fread(&recordVersion, sizeof(unsigned int), 1, pFile);
    printf("Record Version: %u\n", recordVersion);

    if (g_clientBuild >= CLIENT_BUILD_3_0_8)
    {
        unsigned int cacheVersion = 0;
        fread(&cacheVersion, sizeof(unsigned int), 1, pFile);
        printf("Cache Version: %u\n", cacheVersion);
    }
    
    size_t dataSize = size - ftell(pFile);
    printf("Data Size: %llu", dataSize);

    ByteBuffer buffer;
    buffer.resize(dataSize);
    fread((void*)buffer.contents(), 1, dataSize, pFile);

    switch (option)
    {
        case CREATURE_CACHE:
            ReadAllRecords<Creature>(buffer, g_vCreatures);
            break;
        case GAMEOBJECT_CACHE:
            ReadAllRecords<GameObject>(buffer, g_vGameObjects);
            break;
        case ITEM_CACHE:
            ReadAllRecords<Item>(buffer, g_vItems);
            break;
        case QUEST_CACHE:
            ReadAllRecords<Quest>(buffer, g_vQuests);
            break;
        case PAGETEXT_CACHE:
            ReadAllRecords<PageText>(buffer, g_vPageTexts);
            break;
        case NPCTEXT_CACHE:
            ReadAllRecords<NpcText>(buffer, g_vNpcTexts);
            break;
    }

    printf("\nDone!\n");
    printf("Records read: %u\n", g_counter);

    switch (option)
    {
        case CREATURE_CACHE:
            Creature::WriteToSQL(g_vCreatures);
            break;
        case GAMEOBJECT_CACHE:
            GameObject::WriteToSQL(g_vGameObjects);
            break;
        case ITEM_CACHE:
            Item::WriteToSQL(g_vItems);
            break;
        case QUEST_CACHE:
            Quest::WriteToSQL(g_vQuests);
            break;
        case PAGETEXT_CACHE:
            PageText::WriteToSQL(g_vPageTexts);
            break;
        case NPCTEXT_CACHE:
            NpcText::WriteToSQL(g_vNpcTexts);
            break;
    }
    
    printf("\nData has been exported!\n");

    fseek(stdin, 0, SEEK_END);
    getchar();
    return 0;

}