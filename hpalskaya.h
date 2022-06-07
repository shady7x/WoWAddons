#pragma once

#include "auto_common.h"

#define HOLY_LIGHT 48782
#define FLASH_LIGHT 48785
#define HOLY_SHOCK 48825
#define CHASTICA 53563

short henabled;

int GetSpellHealing(int sp_id, int spell_power, float total_mult)
{
	int base_heal = 0;
	float sp_mult = 0.0f;
	if (sp_id == HOLY_LIGHT) base_heal = 4888, sp_mult = 1.67857f;
	else if (sp_id == HOLY_SHOCK) base_heal = 2401, sp_mult = 0.807f;
	else if (sp_id == FLASH_LIGHT) base_heal = 785, sp_mult = 1.009f;

	return int(((float)base_heal + sp_mult * (float)spell_power) * total_mult);
}


unsigned int effective_heal(unsigned int missing_hp, bool main_target, int spd, float t_mult)
{
	int heal = GetSpellHealing(HOLY_LIGHT, spd, t_mult);
	if (main_target)
	{
		if (missing_hp - heal > 5000)
		{
			heal += 4000;          // average heal 15k crit 23k 23-15 = 8 / 2 = 4
		}
	}
	else
	{
		float symbol_heal = (float)heal * 0.1f;
		heal = (int)symbol_heal;
	}
	return min(heal, (int)missing_hp);
}


unsigned long long best_hl_target(std::deque < WoWObject > &raid)
{
	unsigned int max_hl = 0;
	unsigned long long btarget = 0;

	int spd = me->SpellPower();
	unsigned int total = raid.size();
	for (unsigned int i = 0; i < total; ++i)
	{
		Unit* raid_i = new Unit(raid[i].BaseAddress);
		if (raid_i && raid[i].CurrentHealth > 1 && f.xcastrange(Spell(HOLY_LIGHT), raid[i].guid))
		{
			std::vector < unsigned int > ex_heal;
			ex_heal.clear();
			float t_mult = 1.176f; // TODO add some common buffs like AvWrath, ICC, T10 bonus
			unsigned int main_ex_heal = effective_heal(raid[i].MissingHealth, true, spd, t_mult);
			for (unsigned int j = 0; j < total; ++j)
			{
				if (i != j)
				{
					Unit* raid_j = new Unit(raid[j].BaseAddress);
					if (raid_j && raid[j].CurrentHealth > 1)
					{
						if (raid_i->DistanceTo(raid_j) < 8.0f) // TODO check
						{
							ex_heal.push_back(effective_heal(raid[j].MissingHealth, false, spd, t_mult));
						}
					}
					delete raid_j;
				}
			}

			raid[i].hrating = 0;
			for (unsigned int j = 0; j < ex_heal.size(); ++j)
				raid[i].hrating += ex_heal[j];

			if (ex_heal.size() > 0)
			{
				raid[i].hrating = int(float(raid[i].hrating) / ex_heal.size());
			}
			raid[i].hrating += main_ex_heal;

			if (raid[i].hrating > max_hl)
			{
				max_hl = raid[i].hrating;
				btarget = raid[i].guid;
			}
		}
		delete raid_i;
	}

	return btarget;
}

unsigned int TargetAmount(unsigned long long g, std::deque < WoWObject > &raid)
{
	unsigned int res = 0;
	unsigned int total = raid.size();
	for (unsigned int i = 0; i < total; ++i)
	{
		Unit* raid_i = new Unit(raid[i].BaseAddress);
		if (raid_i && raid[i].CurrentHealth > 1)
		{
			if (raid_i->TargetGuid() == g)
				++res;
		}
		delete raid_i;
	}
	return res;
}

void rotation_holypal()
{
	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		if (henabled != 0)
		{
			henabled = 0;
			chat("0 [  ]");
		}
	}
	else if (GetAsyncKeyState(VK_NUMPAD3))
	{
		if (henabled != 2)
		{
			henabled = 2;
			chat("2  [ -cd ]");
		}
	}

	if (!henabled) return;
	if (me->Mounted() || f.ObjectHasAura(me->addr, 43183) || f.ObjectHasAura(me->addr, 45548)) return;

	std::deque < WoWObject > raid;
	raid.clear();
	for (int i = 0x0; i <= 0xA0; i += 0x4) // = 40 ppl * 4 size
	{
		unsigned long long pGuid = readx<unsigned long long>(readx<unsigned int>(0x00BEB568 + i));
		if (pGuid > 0)
		{
			Unit* eu = new Unit(GetObjectByGuidTypeCheck(pGuid));
			if (eu->addr > 0 && eu->addr != me->addr)
			{
				WoWObject e;
				e.guid = pGuid;
				e.BaseAddress = eu->addr;
				e.Type = readx<short>(e.BaseAddress + 0x14);
				if (e.Type == 0x4)
				{
					e.UnitFieldsAddress = readx<unsigned int>(e.BaseAddress + 0x8);
					e.CurrentHealth = readx<unsigned int>(e.UnitFieldsAddress + 0x18 * 4);
					e.MaxHealth = readx<unsigned int>(e.UnitFieldsAddress + 0x20 * 4);
					e.MissingHealth = e.MaxHealth - e.CurrentHealth;
					//e.Name = eu->Name();
					raid.push_back(e);
				}
			}
			delete eu;
		}
	}

	if (raid.size())
	{
		WoWObject eMe, maintank, offtank, focus;
		eMe.guid = me->Guid();
		eMe.BaseAddress = me->addr;
		eMe.Type = 0x4;
		eMe.UnitFieldsAddress = readx<unsigned int>(eMe.BaseAddress + 0x8);
		eMe.CurrentHealth = readx<unsigned int>(eMe.UnitFieldsAddress + 0x18 * 4);
		eMe.MaxHealth = readx<unsigned int>(eMe.UnitFieldsAddress + 0x20 * 4);
		eMe.MissingHealth = eMe.MaxHealth - eMe.CurrentHealth;
		//eMe.Name = me->Name();
		raid.push_back(eMe);

		std::sort(raid.begin(), raid.end(), cmp_raid_max);

		unsigned int total = raid.size();

		maintank = raid[--total];
		offtank = raid[--total];
		raid.pop_back();
		raid.pop_back();

		std::sort(raid.begin(), raid.end(), cmp_raid);

		raid.push_front(offtank);
		raid.push_front(maintank);
		total += 2;

		// st
		int mana = me->Power();
		if (!f.ObjectHasAura(eMe.BaseAddress, 20166) && mana > 552)
		{
			f.xcast(20166, 0);
			return;
		}

		Unit* focusx = new Unit(GetObjectByGuidTypeCheck(GetFocusGuid()));
		if (focusx && focusx->addr > 0)
		{
			focus.guid = focusx->Guid();
			focus.BaseAddress = focusx->addr;
			focus.Type = 0x4;
			focus.UnitFieldsAddress = readx<unsigned int>(focus.BaseAddress + 0x8);
			focus.CurrentHealth = readx<unsigned int>(focus.UnitFieldsAddress + 0x18 * 4);
			focus.MaxHealth = readx<unsigned int>(focus.UnitFieldsAddress + 0x20 * 4);
			focus.MissingHealth = focus.MaxHealth - focus.CurrentHealth;

			unsigned int tl_ch = 0;
			bool has_ch = f.ObjectHasAuraMyAura(focus.BaseAddress, CHASTICA, eMe.guid, tl_ch);
			if (!has_ch || tl_ch < 1000)
			{
				if (mana > 1382 && !f.xcd(CHASTICA) && f.xcastrange(Spell(CHASTICA), focus.guid))
				{
					f.xcast(CHASTICA, focus.guid);
					delete focusx;
					return;
				}
			}

			if (focus.guid == maintank.guid)
				offtank.MissingHealth += 5000;
			else if (focus.guid == offtank.guid)
				maintank.MissingHealth += 5000;
		}


		// try find boss
		unsigned int bossAddr = 0;
		uint64 bossGuid = 0;
		unsigned int cPplTargeting = 0;
		for (unsigned int i = 0; i < total; ++i)
		{
			Unit* raid_i = new Unit(raid[i].BaseAddress);
			if (raid_i && raid[i].CurrentHealth > 1)
			{
				unsigned long long cguid = raid_i->TargetGuid();
				unsigned int ctar = TargetAmount(cguid, raid);
				if (ctar > cPplTargeting)
				{
					cPplTargeting = ctar;
					bossGuid = cguid;
					bossAddr = raid[i].BaseAddress;
					if (cPplTargeting > total / 2)
					{
						delete raid_i;
						break;
					}
				}
			}
			delete raid_i;
		}
		// if we found boss and he doesnt have Heal judgement just use it (normally would just refresh aura)
		if (bossGuid > 0)
		{
			unsigned int tl_j = 0;
			bool has_j = f.ObjectHasAuraMyAura(bossAddr, 20185, me->Guid(), tl_j);
			Unit* uBoss = new Unit(bossAddr);
			if (uBoss->IsInCombat() && (!has_j || tl_j < 1500) && f.xcastrange(Spell(20271), bossGuid))
			{
				if (!f.xcd(20271) && mana > 196)
				{
					f.xcast(20271, bossGuid);
					delete uBoss;
					delete focusx;
					return;
				}
			}
			delete uBoss;
		}
		// if we dont have buff somehow (boss not in range, i.e. syndragosa phase) try to apply it to boss(?)
		// or to random fighting mobs around
		if (!f.ObjectHasAura(eMe.BaseAddress, 54153))
		{
			if (bossGuid > 0 && f.xcastrange(Spell(20271), bossGuid))
			{
				Unit* uBoss = new Unit(bossAddr);
				if (uBoss->IsInCombat() && !f.xcd(20271) && mana > 196)
				{
					f.xcast(20271, bossGuid);
					delete uBoss;
					delete focusx;
					return;
				}
				delete uBoss;
			}
			else for (unsigned int i = 0; i < reckon_guids.size(); ++i)
			{
				Unit* u = new Unit(GetObjectByGuidTypeCheck(reckon_guids[i]));
				if (u != nullptr && u->addr > 0)
				{
					uint64 uGuid = u->Guid();
					int uHealth = u->Health();
					if (uHealth > 1 && !u->IsDead() && u->CanAttack() && u->IsInCombat() && f.xcastrange(Spell(20271), uGuid) && mana > 196)
					{
						f.xcast(20271, uGuid);
						delete focusx;
						delete u;
						return;
					}
				}
				delete u;
			}
		}

		unsigned long long hl_target = best_hl_target(raid);
		if (hl_target > 0)
		{
			if (!f.xcd(HOLY_LIGHT) && mana > 1101)
			{
				f.xcast(HOLY_LIGHT, hl_target);
				delete focusx;
				return;
			}
		}

		delete focusx;
	}
}




/*
bool raid_hp_decent = true;
unsigned int mtot = min(3, total); // no need to check more than 3 targets (maintank, offtank, lowesthp)
for (unsigned int i = 0; i < mtot; ++i)
{
Unit* raid_i = new Unit(raid[i].BaseAddress);
if (raid_i && raid[i].CurrentHealth > 1)
{
if (raid_i->IsInCombat() && raid_i->HealthPercent() < 70 && f.xcastrange(Spell(HOLY_LIGHT), raid[i].guid))
{
raid_hp_decent = false;
break;
}
}
delete raid_i;
}

if (raid_hp_decent)
{
*/