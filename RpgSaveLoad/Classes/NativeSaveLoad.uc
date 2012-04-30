class AOPNativeSaveLoad extends Object DLLBind(RpgSaveLoadImpl);

///////////////////////////////////////////////////////////////////////////////
// = Structs =
///////////////////////////////////////////////////////////////////////////////

struct SlotStruct
{
	var string Id;
	var int RawTime;
	var string GregorianDate;
	var string JalaliDate;
	var string Time;
};

struct SlotListStruct
{
	var array < SlotStruct > Slots;
};

struct QuestStruct
{
	var int Id;
	var int Status;
};

struct InventoryStruct
{
	var int Index;
	var int Id;
	var string CharacterId;
	var int Durability;
	var int CurrentDurability;
};

struct BuffStruct
{
	var int Index;
	var int Id;
	var string CharacterId;
	var float Time;
};

struct AbilityStruct
{
	var int Index;
	var string Id;
	var string CharacterId;
	var int Points;
};

struct CharacterStruct
{
	var string Id;

	var int Level;
	var int XP;
	var int HP;
	var int MP;

	var int Money;
};

struct LootStruct
{
	var string Id;
	var float LastLootTime;
	var bool IsLooted;
};

struct SimpleDialogStruct
{
	var int Id;
	var int BatchIndex;
	var bool IsPlayed;
};

struct AccompanyingCharacterStruct
{
	var int Index;
	var string Id;
};

struct PlayerCharacterStruct
{
	var array< AccompanyingCharacterStruct > AccompanyingCharacters;
	var int RemainingAbilityPoints;
	var string LevelMapName;
	var Vector Location;
};

struct MapLevelStruct
{
	var string LevelId;
	var bool   IsUnlocked;
};

struct MapStruct
{
	var string QuestLevelId;
	var string QuestLevelMapName;
	var array < MapLevelStruct > MapLevels;
};

struct SlotDataStruct
{
	var array < QuestStruct > Quests;
	var array < CharacterStruct > Characters;
	var array < InventoryStruct > Inventory;
	var array < BuffStruct > Buffs;
	var array < AbilityStruct > Abilities;
	var array < LootStruct > Loots;
	var array < SimpleDialogStruct > SimpleDialogs;
	var PlayerCharacterStruct PlayerCharacter;
	var MapStruct Map;
};


///////////////////////////////////////////////////////////////////////////////
// = Fields =
///////////////////////////////////////////////////////////////////////////////

var private bool m_initialized;


///////////////////////////////////////////////////////////////////////////////
// = Native Functions =
///////////////////////////////////////////////////////////////////////////////

dllimport final function InitializeSaveLoad();
dllimport final function GetAllSlots(out SlotListStruct out_slotsList);
dllimport final function CommitSlot(string id, SlotDataStruct data);
dllimport final function FetchSlot(string id, out SlotDataStruct out_data);
dllimport final function RemoveSlot(string id);


///////////////////////////////////////////////////////////////////////////////
// = Public Methods =
///////////////////////////////////////////////////////////////////////////////

public function Initialize()
{
	if(m_initialized)
		return;
	m_initialized = true;
	InitializeSaveLoad();
}


public function bool HasASlotWithId(string slotId)
{
	local SlotListStruct slotList;
	local int i;

	Initialize();
	GetAllSlots(slotList);
	for (i = 0; i < slotList.Slots.Length; i++)
	{
		if(slotList.Slots[i].Id ~= slotId)
			return true;
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// = Private Methods =
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// = Default Properties =
///////////////////////////////////////////////////////////////////////////////
DefaultProperties
{
}

