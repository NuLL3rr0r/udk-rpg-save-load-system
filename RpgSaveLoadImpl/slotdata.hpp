#ifndef SLOTDATA_HPP
#define SLOTDATA_HPP


#include "udktypes.hpp"

struct SlotStruct
{
	FString Id;
	int RawTime;
	FString GregorianDate;
	FString JalaliDate;
	FString Time;
};

struct SlotListStruct
{
	TArray<struct SlotStruct> Slots;
};

struct QuestStruct
{
	int Id;
	int Status;
};

struct InventoryStruct
{
	int Index;
	int Id;
	FString CharacterId;
	int Durability;
	int CurrentDurability;
};

struct BuffStruct
{
	int Index;
	int Id;
	FString CharacterId;
	float Time;
};

struct AbilityStruct
{
	int Index;
	FString Id;
	FString CharacterId;
	int Points;
};

struct CharacterStruct
{
	FString Id;

	int Level;
	int XP;
	int HP;
	int MP;

	int Money;
};

struct LootStruct
{
	FString Id;
	float LastLootTime;
	bool IsLooted;
};

struct SimpleDialogStruct
{
	int Id;
	int BatchIndex;
	bool IsPlayed;
};

struct AccompanyingCharacterStruct
{
	int Index;
	FString Id;
};

struct PlayerCharacterStruct
{
	TArray<struct AccompanyingCharacterStruct> AccompanyingCharacters;
	int RemainingAbilityPoints;
	FString LevelMapName;
	FVector Location;
};

struct MapLevelStruct
{
	FString LevelId;
	bool    IsUnlocked;
};

struct MapStruct
{
	FString QuestLevelId;
	FString QuestLevelMapName;
	TArray < MapLevelStruct > MapLevels;
};

struct SlotDataStruct
{
	TArray<struct QuestStruct> Quests;
	TArray<struct CharacterStruct> Characters;
	TArray<struct InventoryStruct> Inventory;
	TArray<struct BuffStruct> Buffs;
	TArray<struct AbilityStruct> Abilities;
	TArray<struct LootStruct> Loots;
	TArray<struct SimpleDialogStruct> SimpleDialogs;
	struct PlayerCharacterStruct PlayerCharacter;
	struct MapStruct Map;
};


#endif /* SLOTDATA_HPP */

