#ifndef SAVELOAD_HPP
#define SAVELOAD_HPP


#include "slotdata.hpp"
#include <string>

namespace RpgSaveLoadImpl {
	class DB;
    class SaveLoad;
}

class RpgSaveLoadImpl::SaveLoad
{
private:
	static const std::string M_TABLE_SLOTS;
	static const std::string M_TABLE_QUESTS;
	static const std::string M_TABLE_INVENTORY;
	static const std::string M_TABLE_BUFFS;
	static const std::string M_TABLE_ABILITIES;
	static const std::string M_TABLE_CHARACTERS;
	static const std::string M_TABLE_LOOTS;
	static const std::string M_TABLE_SIMPLEDIALOGS;
	static const std::string M_TABLE_ACCOMPANYINGCHARACTERS;
	static const std::string M_TABLE_PLAYERCHARACTER;
	static const std::string M_TABLE_MAP;
	static const std::string M_TABLE_MAPLEVELS;

private:
	DB *m_db;

public:
	SaveLoad();
    ~SaveLoad();

	void Initialize();
	void GetAllSlots(struct SlotListStruct *out_slotsList);
	void CommitSlot(const std::string &id, struct SlotDataStruct *data);
	void FetchSlot(const std::string &id, struct SlotDataStruct *out_data);
	void RemoveSlot(const std::string &id);
};


#endif /* SAVELOAD_HPP */

