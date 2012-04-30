#include <shlobj.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include "saveload.hpp"
#include "base.hpp"
#include "cdate.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "debug.hpp"
#include "system.hpp"
#include "util.hpp"

using namespace std;
using namespace boost;
using namespace cppdb;
using namespace CDate;
using namespace RpgSaveLoadImpl;

namespace fs = boost::filesystem;

const string SaveLoad::M_TABLE_SLOTS = "slots";
const string SaveLoad::M_TABLE_QUESTS = "quests";
const string SaveLoad::M_TABLE_INVENTORY = "inventory";
const string SaveLoad::M_TABLE_BUFFS = "buffs";
const string SaveLoad::M_TABLE_ABILITIES = "abilities";
const string SaveLoad::M_TABLE_CHARACTERS = "characters";
const string SaveLoad::M_TABLE_LOOTS = "loots";
const string SaveLoad::M_TABLE_SIMPLEDIALOGS = "simpledialogs";
const string SaveLoad::M_TABLE_ACCOMPANYINGCHARACTERS = "accompanyingcharacters";
const string SaveLoad::M_TABLE_PLAYERCHARACTER = "playercharacter";
const string SaveLoad::M_TABLE_MAP = "map";
const string SaveLoad::M_TABLE_MAPLEVELS = "maplevels";

SaveLoad::SaveLoad()
{
	try {
		DB::LoadSQLite3Driver();
	
		string curPath(System::GetFolderPath(CSIDL_PERSONAL));
		if (!algorithm::ends_with(curPath, System::DirectorySeparatorChar()))
			curPath += System::DirectorySeparatorChar();

		string vendorPath(curPath + Base::SAVELOAD_CACHE_VENDOR_PATH);
		string titlePath(curPath + Base::SAVELOAD_CACHE_TITLE_PATH);
		string dbDirPath(curPath + Base::SAVELOAD_CACHE_DIR_PATH);
		string dbFilePath(curPath + Base::SAVELOAD_CACHE_FILE_PATH);
	
		try {
			if (fs::exists(vendorPath)) {
				if (!fs::is_directory(vendorPath)) {
					fs::remove(vendorPath);
				}
			}
			if (fs::exists(titlePath)) {
				if (!fs::is_directory(titlePath)) {
					fs::remove(titlePath);
				}
			}
			if (!fs::exists(dbDirPath)) {
				fs::create_directories(dbDirPath);
			}
		}
		catch(const fs::filesystem_error &ex) {
			LOG("IO Error", ex.what());
		}
		catch(...) {
			LOG("Unknown IO Error!!");
		}

		DB::Vacuum(dbFilePath.c_str());
		m_db = new DB(dbFilePath);
	}
    catch (std::exception &ex) {
		LOG(ex.what());
	}
	catch (...) {
		LOG("Unknown Error!!");
	}
}

SaveLoad::~SaveLoad()
{
	delete m_db;
}

void SaveLoad::Initialize()
{
	try {
		m_db->CreateTable(M_TABLE_SLOTS,
						  " id TEXT PRIMARY KEY NOT NULL, "
						  " rawtime TEXT, "
						  " gdate TEXT, "
						  " jdate TEXT, "
						  " time TEXT, "
						  " UNIQUE (id) "
						  );

		m_db->CreateTable(M_TABLE_QUESTS,
						  " slotid TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " status TEXT, "
						  " PRIMARY KEY (slotid, id) "
						  );

		m_db->CreateTable(M_TABLE_INVENTORY,
						  " slotid TEXT NOT NULL, "
						  " itemindex TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " characterid TEXT NOT NULL, "
						  " durability TEXT, "
						  " currentdurability TEXT, "
						  " PRIMARY KEY (slotid, itemindex, characterid) "
						  );

		m_db->CreateTable(M_TABLE_BUFFS,
						  " slotid TEXT NOT NULL, "
						  " itemindex TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " characterid TEXT NOT NULL, "
						  " time TEXT, "
						  " PRIMARY KEY (slotid, itemindex, characterid) "
						  );

		m_db->CreateTable(M_TABLE_ABILITIES,
						  " slotid TEXT NOT NULL, "
						  " itemindex TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " characterid TEXT NOT NULL, "
						  " points TEXT, "
						  " PRIMARY KEY (slotid, id, characterid) "
						  );

		m_db->CreateTable(M_TABLE_CHARACTERS,
						  " slotid TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " level TEXT, "
						  " xp TEXT, "
						  " hp TEXT, "
						  " mp TEXT, "
						  " money TEXT, "
						  " PRIMARY KEY (slotid, id) "
						  );

		m_db->CreateTable(M_TABLE_LOOTS,
						  " slotid TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " timesincelastloot TEXT, "
						  " islooted TEXT, "
						  " PRIMARY KEY (slotid, id) "
						  );

		m_db->CreateTable(M_TABLE_SIMPLEDIALOGS,
						  " slotid TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " batchindex TEXT NOT NULL, "
						  " isplayed TEXT, "
						  " PRIMARY KEY (slotid, id, batchindex) "
						  );

		m_db->CreateTable(M_TABLE_ACCOMPANYINGCHARACTERS,
						  " slotid TEXT NOT NULL, "
						  " itemindex TEXT NOT NULL, "
						  " id TEXT NOT NULL, "
						  " PRIMARY KEY (slotid, itemindex) "
						  );

		m_db->CreateTable(M_TABLE_PLAYERCHARACTER,
						  " slotid TEXT NOT NULL, "
						  " remainingabilitypoints TEXT, "
						  " levelid TEXT, "
						  " locationx TEXT, "
						  " locationy TEXT, "
						  " locationz TEXT, "
						  " PRIMARY KEY (slotid) "
						  );

		m_db->CreateTable(M_TABLE_MAP,
						  " slotid TEXT NOT NULL, "
						  " questlevelid TEXT, "
						  " questLevelmapname TEXT, "
						  " PRIMARY KEY (slotid) "
						  );

		m_db->CreateTable(M_TABLE_MAPLEVELS,
						  " slotid TEXT NOT NULL, "
						  " levelid TEXT, "
						  " isunlocked TEXT, "
						  " PRIMARY KEY (slotid, levelid) "
						  );
	}
    catch (std::exception &ex) {
		LOG(ex.what());
	}
	catch (...) {
		LOG("Unknown Error!!");
	}
}

void SaveLoad::GetAllSlots(struct SlotListStruct *out_slotsList)
{
	try {
		result r = m_db->Sql() << "SELECT id, rawtime, gdate, jdate, time FROM ["
			+ M_TABLE_SLOTS + "];";

		out_slotsList->Slots.Clear();

		while(r.next()) {
			string id;
			string rawTime;
			string gDate;
			string jDate;
			string time;

			r >> id >> rawTime >> gDate >> jDate >> time;

			SlotStruct *slot = new SlotStruct();
			slot->Id = Crypto::Decrypt(id);
			slot->RawTime = lexical_cast<int>(Crypto::Decrypt(rawTime));
			slot->GregorianDate = Crypto::Decrypt(gDate);
			slot->JalaliDate = Crypto::Decrypt(jDate);
			slot->Time = Crypto::Decrypt(time);
			out_slotsList->Slots.AddItem(slot);
		}
	}
    catch (std::exception &ex) {
		LOG(ex.what());
	}
	catch (...) {
		LOG("Unknown Error!!");
	}
}

void SaveLoad::CommitSlot(const string &id, struct SlotDataStruct *data)
{
	try {
		const string encId = Crypto::Encrypt(id);

		result r = m_db->Sql() << "SELECT id FROM ["
			+ M_TABLE_SLOTS + "] "
			"WHERE id=?;" << encId << row;

		Now n;

		cppdb::transaction guard(m_db->Sql());

		m_db->Delete(M_TABLE_QUESTS, "slotid", encId);
		m_db->Delete(M_TABLE_CHARACTERS, "slotid", encId);
		m_db->Delete(M_TABLE_INVENTORY, "slotid", encId);
		m_db->Delete(M_TABLE_BUFFS, "slotid", encId);
		m_db->Delete(M_TABLE_ABILITIES, "slotid", encId);
		m_db->Delete(M_TABLE_LOOTS, "slotid", encId);
		m_db->Delete(M_TABLE_SIMPLEDIALOGS, "slotid", encId);
		m_db->Delete(M_TABLE_PLAYERCHARACTER, "slotid", encId);
		m_db->Delete(M_TABLE_ACCOMPANYINGCHARACTERS, "slotid", encId);
		m_db->Delete(M_TABLE_MAP, "slotid", encId);
		m_db->Delete(M_TABLE_MAPLEVELS, "slotid", encId);

		if (r.empty()) {
			m_db->Insert(M_TABLE_SLOTS, "id, rawtime, gdate, jdate, time", 5,
				encId.c_str(),
				Crypto::Encrypt(lexical_cast<std::string>(n.RawTime)).c_str(),
				Crypto::Encrypt(DateConv::ToGregorian(n)).c_str(),
				Crypto::Encrypt(DateConv::ToJalali(n)).c_str(),
				Crypto::Encrypt(DateConv::Time(n)).c_str()
			);
		} else {
			m_db->Update(M_TABLE_SLOTS, "id", encId, "rawtime=?, gdate=?, jdate=?, time=?", 4,
				Crypto::Encrypt(lexical_cast<std::string>(n.RawTime)).c_str(),
				Crypto::Encrypt(DateConv::ToGregorian(n)).c_str(),
				Crypto::Encrypt(DateConv::ToJalali(n)).c_str(),
				Crypto::Encrypt(DateConv::Time(n)).c_str()
			);
		}

		for (int i = 0; i < data->Quests.Num(); ++i) {
			QuestStruct quest = data->Quests[i];
			m_db->Insert(M_TABLE_QUESTS, "slotid, id, status", 3, encId.c_str(),
				Crypto::Encrypt(lexical_cast<string>(quest.Id)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(quest.Status)).c_str()
				);
		}

		for (int i = 0; i < data->Characters.Num(); ++i) {
			CharacterStruct character = data->Characters[i];
			m_db->Insert(M_TABLE_CHARACTERS, "slotid, id, level, xp, hp, mp, money", 7, encId.c_str(),
				Crypto::Encrypt(Util::WStrToStr(character.Id.GetText())).c_str(),
				Crypto::Encrypt(lexical_cast<string>(character.Level)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(character.XP)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(character.HP)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(character.MP)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(character.Money)).c_str()
				);
		}

		for (int i = 0; i < data->Inventory.Num(); ++i) {
			InventoryStruct inventory = data->Inventory[i];
			m_db->Insert(M_TABLE_INVENTORY, "slotid, itemindex, id, characterid, durability, currentdurability", 6, encId.c_str(),
				Crypto::Encrypt(lexical_cast<string>(inventory.Index)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(inventory.Id)).c_str(),
				Crypto::Encrypt(Util::WStrToStr(inventory.CharacterId.GetText())).c_str(),
				Crypto::Encrypt(lexical_cast<string>(inventory.Durability)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(inventory.CurrentDurability)).c_str()
				);
		}

		for (int i = 0; i < data->Buffs.Num(); ++i) {
			BuffStruct buff = data->Buffs[i];
			m_db->Insert(M_TABLE_BUFFS, "slotid, itemindex, id, characterid, time", 5, encId.c_str(),
				Crypto::Encrypt(lexical_cast<string>(buff.Index)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(buff.Id)).c_str(),
				Crypto::Encrypt(Util::WStrToStr(buff.CharacterId.GetText())).c_str(),
				Crypto::Encrypt(lexical_cast<string>(buff.Time)).c_str()
				);
		}

		for (int i = 0; i < data->Abilities.Num(); ++i) {
			AbilityStruct ability = data->Abilities[i];
			m_db->Insert(M_TABLE_ABILITIES, "slotid, itemindex, id, characterid, points", 5, encId.c_str(),
				Crypto::Encrypt(lexical_cast<string>(ability.Index)).c_str(),
				Crypto::Encrypt(Util::WStrToStr(ability.Id.GetText())).c_str(),
				Crypto::Encrypt(Util::WStrToStr(ability.CharacterId.GetText())).c_str(),
				Crypto::Encrypt(lexical_cast<string>(ability.Points)).c_str()
				);
		}

		for (int i = 0; i < data->Loots.Num(); ++i) {
			LootStruct loot = data->Loots[i];
			m_db->Insert(M_TABLE_LOOTS, "slotid, id, timesincelastloot, islooted", 4, encId.c_str(),
				Crypto::Encrypt(Util::WStrToStr(loot.Id.GetText())).c_str(),
				Crypto::Encrypt(lexical_cast<string>(loot.IsLooted)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(loot.LastLootTime)).c_str()
				);
		}

		for (int i = 0; i < data->SimpleDialogs.Num(); ++i) {
			SimpleDialogStruct simpleDialog = data->SimpleDialogs[i];
			m_db->Insert(M_TABLE_SIMPLEDIALOGS, "slotid, id, batchindex, isplayed", 4, encId.c_str(),
				Crypto::Encrypt(lexical_cast<string>(simpleDialog.Id)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(simpleDialog.BatchIndex)).c_str(),
				Crypto::Encrypt(lexical_cast<string>(simpleDialog.IsPlayed)).c_str()
				);
		}

		PlayerCharacterStruct playerCharacter = data->PlayerCharacter;
		m_db->Insert(M_TABLE_PLAYERCHARACTER, "slotid, remainingabilitypoints, levelid, locationx, locationy, locationz", 6, encId.c_str(),
			Crypto::Encrypt(lexical_cast<string>(playerCharacter.RemainingAbilityPoints)).c_str(),
			Crypto::Encrypt(Util::WStrToStr(playerCharacter.LevelMapName.GetText())).c_str(),
			Crypto::Encrypt(lexical_cast<string>(playerCharacter.Location.x)).c_str(),
			Crypto::Encrypt(lexical_cast<string>(playerCharacter.Location.y)).c_str(),
			Crypto::Encrypt(lexical_cast<string>(playerCharacter.Location.z)).c_str()
			);

		for (int i = 0; i < playerCharacter.AccompanyingCharacters.Num(); ++i) {
			AccompanyingCharacterStruct accompanyingCharacter = playerCharacter.AccompanyingCharacters[i];
			m_db->Insert(M_TABLE_ACCOMPANYINGCHARACTERS, "slotid, itemindex, id", 3, encId.c_str(),
				Crypto::Encrypt(lexical_cast<string>(accompanyingCharacter.Index)).c_str(),
				Crypto::Encrypt(Util::WStrToStr(accompanyingCharacter.Id.GetText())).c_str()
				);
		}

		MapStruct map = data->Map;
		m_db->Insert(M_TABLE_MAP, "slotid, questlevelid, questlevelmapname", 3, encId.c_str(),
			Crypto::Encrypt(Util::WStrToStr(map.QuestLevelId.GetText())).c_str(),
			Crypto::Encrypt(Util::WStrToStr(map.QuestLevelMapName.GetText())).c_str()
			);

		for (int i = 0; i < map.MapLevels.Num(); ++i) {
			MapLevelStruct mapLevel = map.MapLevels[i];
			m_db->Insert(M_TABLE_MAPLEVELS, "slotid, levelid, isunlocked", 3, encId.c_str(),
				Crypto::Encrypt(Util::WStrToStr(mapLevel.LevelId.GetText())).c_str(),
				Crypto::Encrypt(lexical_cast<string>(mapLevel.IsUnlocked)).c_str()
				);
		}

		guard.commit();
	}
    catch (std::exception &ex) {
		LOG(ex.what());
	}
	catch (...) {
		LOG("Unknown Error!!");
	}
}

void SaveLoad::FetchSlot(const std::string &id, struct SlotDataStruct *out_data)
{
	try {
		const string encId = Crypto::Encrypt(id);

		result r = m_db->Sql() << "SELECT id FROM ["
			+ M_TABLE_SLOTS + "] "
			"WHERE id=?;" << encId << row;

		if (!r.empty())
		{
			// quests
			result questResult = m_db->Sql() << "SELECT id, status FROM [" + M_TABLE_QUESTS + "] "
				"WHERE slotid=?;" << encId;
			while(questResult.next()) {
				string questId;
				string status;
				questResult >> questId >> status;
				QuestStruct *quest = new QuestStruct();
				quest->Id = lexical_cast<int>(Crypto::Decrypt(questId));
				quest->Status = lexical_cast<int>(Crypto::Decrypt(status));

				out_data->Quests.AddItem(quest);
			}

			// characters
			result characterResult = m_db->Sql() << "SELECT id, level, xp, hp, mp, money FROM [" + M_TABLE_CHARACTERS + "] "
				"WHERE slotid=?;" << encId;
			while(characterResult.next()) {
				string characterId;
				string level;
				string xp;
				string hp;
				string mp;
				string money;
				characterResult >> characterId >> level >> xp >> hp >> mp >> money;
				CharacterStruct *character = new CharacterStruct();
				character->Id = Crypto::Decrypt(characterId);
				character->Level = lexical_cast<int>(Crypto::Decrypt(level));
				character->XP = lexical_cast<int>(Crypto::Decrypt(xp));
				character->HP = lexical_cast<int>(Crypto::Decrypt(hp));
				character->MP = lexical_cast<int>(Crypto::Decrypt(mp));
				character->Money = lexical_cast<int>(Crypto::Decrypt(money));

				out_data->Characters.AddItem(character);
			}

			// inventory
			result inventoryResult = m_db->Sql() << "SELECT itemindex, id, characterid, durability, currentdurability FROM [" + M_TABLE_INVENTORY + "] "
				"WHERE slotid=?;" << encId;
			while(inventoryResult.next()) {
				string index;
				string inventoryId;
				string characterId;
				string durability;
				string currentDurability;
				inventoryResult >> index >> inventoryId >> characterId >> durability >> currentDurability;
				InventoryStruct *inventory = new InventoryStruct();
				inventory->Index = lexical_cast<int>(Crypto::Decrypt(index));
				inventory->Id = lexical_cast<int>(Crypto::Decrypt(inventoryId));
				inventory->CharacterId = Crypto::Decrypt(characterId);
				inventory->Durability = lexical_cast<int>(Crypto::Decrypt(durability));
				inventory->CurrentDurability = lexical_cast<int>(Crypto::Decrypt(currentDurability));

				out_data->Inventory.AddItem(inventory);
			}

			// buffs
			result buffResult = m_db->Sql() << "SELECT itemindex, id, characterid, time FROM [" + M_TABLE_BUFFS + "] "
				"WHERE slotid=?;" << encId;
			while(buffResult.next()) {
				string index;
				string buffId;
				string characterId;
				string time;
				buffResult >> index >> buffId >> characterId >> time;
				BuffStruct *buff = new BuffStruct();
				buff->Index = lexical_cast<int>(Crypto::Decrypt(index));
				buff->Id = lexical_cast<int>(Crypto::Decrypt(buffId));
				buff->CharacterId = Crypto::Decrypt(characterId);
				buff->Time = lexical_cast<float>(Crypto::Decrypt(time));

				out_data->Buffs.AddItem(buff);
			}

			// abilities
			result abilityResult = m_db->Sql() << "SELECT itemindex, id, characterid, points FROM [" + M_TABLE_ABILITIES + "] "
				"WHERE slotid=?;" << encId;
			while(abilityResult.next()) {
				string index;
				string abilityId;
				string characterId;
				string points;
				abilityResult >> index >> abilityId >> characterId >> points;
				AbilityStruct *ability = new AbilityStruct();
				ability->Index = lexical_cast<int>(Crypto::Decrypt(index));
				ability->Id =Crypto::Decrypt(abilityId);
				ability->CharacterId = Crypto::Decrypt(characterId);
				ability->Points = lexical_cast<int>(Crypto::Decrypt(points));

				out_data->Abilities.AddItem(ability);
			}

			// loots
			result lootResult = m_db->Sql() << "SELECT id, timesincelastloot, islooted FROM [" + M_TABLE_LOOTS + "] "
				"WHERE slotid=?;" << encId;
			while(lootResult.next()) {
				string lootId;
				string timeSinceLastLoot;
				string isLooted;
				lootResult >> lootId >> timeSinceLastLoot >> isLooted;
				LootStruct *loot = new LootStruct();
				loot->Id = Crypto::Decrypt(lootId);
				loot->LastLootTime = lexical_cast<float>(Crypto::Decrypt(timeSinceLastLoot));
				loot->IsLooted = lexical_cast<bool>(Crypto::Decrypt(isLooted));

				out_data->Loots.AddItem(loot);
			}

			// simple dialogs
			result simpledialogResult = m_db->Sql() << "SELECT id, batchindex, isplayed FROM [" + M_TABLE_SIMPLEDIALOGS + "] "
				"WHERE slotid=?;" << encId;
			while(simpledialogResult.next()) {
				string simpledialogId;
				string batchIndex;
				string isPlayed;
				simpledialogResult >> simpledialogId >> batchIndex >> isPlayed;
				SimpleDialogStruct *simpleDialog = new SimpleDialogStruct();
				simpleDialog->Id = lexical_cast<int>(Crypto::Decrypt(simpledialogId));
				simpleDialog->BatchIndex = lexical_cast<int>(Crypto::Decrypt(batchIndex));
				simpleDialog->IsPlayed = lexical_cast<bool>(Crypto::Decrypt(isPlayed));

				out_data->SimpleDialogs.AddItem(simpleDialog);
			}

			// player character
			result pcResult = m_db->Sql() << "SELECT remainingabilitypoints, levelid, locationx, locationy, locationz FROM [" + M_TABLE_PLAYERCHARACTER + "] "
				"WHERE slotid=?;" << encId;
			while(pcResult.next()) {

				string remainingAbilityPoints;
				string levelId;
				string locationX;
				string locationY;
				string locationZ;
				pcResult >> remainingAbilityPoints >> levelId >> locationX >> locationY >> locationZ;
				out_data->PlayerCharacter.RemainingAbilityPoints = lexical_cast<int>(Crypto::Decrypt(remainingAbilityPoints));
				out_data->PlayerCharacter.LevelMapName = Crypto::Decrypt(levelId);
				out_data->PlayerCharacter.Location.x = lexical_cast<float>(Crypto::Decrypt(locationX));
				out_data->PlayerCharacter.Location.y = lexical_cast<float>(Crypto::Decrypt(locationY));
				out_data->PlayerCharacter.Location.z = lexical_cast<float>(Crypto::Decrypt(locationZ));

				result acResult =  m_db->Sql() << "SELECT itemindex, id FROM [" + M_TABLE_ACCOMPANYINGCHARACTERS + "] "
					"WHERE slotid=?;" << encId;
				while(acResult.next()) {
					string index;
					string accompanyingCharacterId;
					acResult >> index >> accompanyingCharacterId;
					AccompanyingCharacterStruct *accompanyingCharacter = new AccompanyingCharacterStruct();
					accompanyingCharacter->Index = lexical_cast<int>(Crypto::Decrypt(index));
					accompanyingCharacter->Id = Crypto::Decrypt(accompanyingCharacterId);

					out_data->PlayerCharacter.AccompanyingCharacters.AddItem(accompanyingCharacter);
				}

				break;
			}

			// map
			result mapResult = m_db->Sql() << "SELECT questlevelid, questlevelmapname FROM [" + M_TABLE_MAP + "] "
				"WHERE slotid=?;" << encId;
			while(mapResult.next()) {
				string questlevelid;
				string questlevelmapname;
				mapResult >> questlevelid >> questlevelmapname;
				out_data->Map.QuestLevelId = Crypto::Decrypt(questlevelid);
				out_data->Map.QuestLevelMapName = Crypto::Decrypt(questlevelmapname);

				// map levels
				result mapLevelsResult =  m_db->Sql() << "SELECT levelid, isunlocked FROM [" + M_TABLE_MAPLEVELS + "] "
					"WHERE slotid=?;" << encId;
				while(mapLevelsResult.next()) {
					string levelid;
					string isunlocked;
					mapLevelsResult >> levelid >> isunlocked;
					MapLevelStruct *mapLevel = new MapLevelStruct();
					mapLevel->LevelId = Crypto::Decrypt(levelid);
					mapLevel->IsUnlocked = lexical_cast<bool>(Crypto::Decrypt(isunlocked));
					out_data->Map.MapLevels.AddItem(mapLevel);
				}

				break;
			}

		}
	}
    catch (std::exception &ex) {
		LOG(ex.what());
	}
	catch (...) {
		LOG("Unknown Error!!");
	}
}


void SaveLoad::RemoveSlot(const string &id)
{
		const string encId = Crypto::Encrypt(id);

		cppdb::transaction guard(m_db->Sql());

		m_db->Delete(M_TABLE_QUESTS, "slotid", encId);
		m_db->Delete(M_TABLE_INVENTORY, "slotid", encId);
		m_db->Delete(M_TABLE_BUFFS, "slotid", encId);
		m_db->Delete(M_TABLE_ABILITIES, "slotid", encId);
		m_db->Delete(M_TABLE_CHARACTERS, "slotid", encId);
		m_db->Delete(M_TABLE_LOOTS, "slotid", encId);
		m_db->Delete(M_TABLE_SIMPLEDIALOGS, "slotid", encId);
		m_db->Delete(M_TABLE_ACCOMPANYINGCHARACTERS, "slotid", encId);
		m_db->Delete(M_TABLE_PLAYERCHARACTER, "slotid", encId);
		m_db->Delete(M_TABLE_MAP, "slotid", encId);
		m_db->Delete(M_TABLE_MAPLEVELS, "slotid", encId);

		m_db->Delete(M_TABLE_SLOTS, "id", encId);

		guard.commit();
}

