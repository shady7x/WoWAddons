#pragma once

#include "auto_common.h"

#define DIVINE_STORM 53385
#define JUDGEMENT_OFW 53408
#define JUDGEMENT_OFL 20271
#define CRUSADER_STRIKE 35395
#define HAMMER_OF_WRATH 48806
#define EXORCISM 48801
#define CONSECRATION 48819
#define RECKONING 62124

short enabled;

bool IgnoreHealthTarget(int npc, double hp_pr);

void rotation_paladin()
{
	state = 0;
	Unit* targetx = new Unit(GetObjectByGuidTypeCheck(me->TargetGuid()));
	if (!GetAsyncKeyState(0x5))
	{
		if (enabled && targetx && targetx->addr > 0 && targetx->CanAttack() && targetx->Health())
		{
			state = 1;
			tloc = targetx->Coords();
			uint64 tGuid = targetx->Guid();

			std::to_string(1);

			if (targetx->Distance() < 4.0f || f.xcastrange(Spell(CRUSADER_STRIKE), tGuid))
			{
				state = 2;

				if (pala_canceller && f.ObjectHasAura(me->addr, 73422))
				{
					FramescriptExecute("CancelUnitBuff(\"player\", GetSpellInfo(73422))");
				}

				if (!me->IsAutoAttacking())
				{
					f.xcast(6603, tGuid);
				}

				int ps2 = JUDGEMENT_OFW;
				if (enabled == 2)
					ps2 = JUDGEMENT_OFL;
				int ps3 = CRUSADER_STRIKE;
				if (f.ObjectHasAura(me->addr, 20375)) // aoe seal
					std::swap(ps2, ps3);

				if (f.xcd(DIVINE_STORM) < 50)
				{
					bool cast = f.xcast(DIVINE_STORM, 0);
				}
				else if (f.xcd(ps2) < 50)
				{
					bool cast = f.xcast(ps2, tGuid);
				}
				else if (f.xcd(ps3) < 50)
				{
					bool cast = f.xcast(ps3, tGuid);
				}
				else if (f.xcd(HAMMER_OF_WRATH) < 50 && targetx->HealthPercent() < 20.0)
				{
					bool cast = f.xcast(HAMMER_OF_WRATH, tGuid);
				}
				else if (f.xcd(EXORCISM) < 50 && f.ObjectHasAura(me->addr, 59578)) // ideally should also check that at least 200 ms for buff left 
				{
					bool cast = f.xcast(EXORCISM, tGuid);
				}
				else if (f.xcd(CONSECRATION) == 0 && me->PowerPercent() > 30.0 && targetx->HealthPercent() > 20.0)
				{
					bool cast = f.xcast(CONSECRATION, 0);
				}

				if (pala_killer && f.xcd(RECKONING) == 0)
				{
					uint64 mGuid = me->Guid();
					unsigned int bStacks = f.ObjectGetAuraStacks(me->addr, 20053, -1);

					float rDamage = 0.5f * (float)me->AttackPower() * 1.03f * 1.03f * (1.0f + 0.03f * (float)bStacks); // +3% passive +3% my auras +0.3.6.9% from crit stacks 
					if (f.ObjectHasAura(me->addr, 73828) || f.ObjectHasAura(me->addr, 73822))
					{
						rDamage *= 1.30f; // +30% icc
					}
					else if (f.ObjectHasAura(me->addr, 72221))
					{
						rDamage *= 1.05f; // +5% heroic inst
					}
					if (f.ObjectHasAura(me->addr, 31884))
					{
						rDamage *= 1.20f; // +20% avengers wrath
					}

					for (unsigned int i = 0; i < reckon_guids.size(); ++i)
					{
						Unit* u = new Unit(GetObjectByGuidTypeCheck(reckon_guids[i]));
						if (u == nullptr || u->addr < 1)
							continue;
						uint64 uGuid = u->Guid();
						int uHealth = u->Health();
						bool ignoringHp = IgnoreHealthTarget(u->npc_id(), u->HealthPercent());
						if (pala_killer == 2 && !ignoringHp)
							continue;
						if (uHealth > 1 && (uHealth < 15000 || ignoringHp) && !u->IsDead() && u->CanAttack() && u->TargetGuid() != mGuid && f.xcastrange(Spell(RECKONING), uGuid))
						{
							float tDamage = rDamage;
							CreatureType uType = u->GetCreatureType();
							if (uType == CreatureType::CREATURE_TYPE_DEMON || uType == CreatureType::CREATURE_TYPE_ELEMENTAL || uType == CreatureType::CREATURE_TYPE_UNDEAD || uType == CreatureType::CREATURE_TYPE_HUMANOID)
							{
								tDamage *= 1.03f; // +3% passive to demons / elementals / undeads / humanoids 
								if (uType == CreatureType::CREATURE_TYPE_UNDEAD)
								{
									tDamage *= 1.01f; // +1% undead tracking 
								}
							}
							if (f.ObjectHasAura(u->addr, 51735) /*|| f.ObjectHasAura(u->addr, 60433)*/)
							{
								tDamage *= 1.13f; // +13% unholy plague or moonkin buff
							}

							if (ignoringHp || uHealth < (int)tDamage)
							{
								//uint64 bGuid = *reinterpret_cast<uint64*>(0xBD07B0);
								//*reinterpret_cast<uint64*>(0xBD07B0) = uGuid;
								bool cast = f.xcast(RECKONING, uGuid);
								//*reinterpret_cast<uint64*>(0xBD07B0) = bGuid;
								delete u;
								break;
							}
						}
						delete u;
					}
				}
			}
		}
	}
	delete targetx;
}

bool IgnoreHealthTarget(int npc, double hp_pr)
{
	if (npc == 38508 && (hp_pr < 16.0 || hp_pr > 90.0))
		return true;
	if (npc == 36609) 
		return true;
	if (npc == 36619)
		return true;
	if (npc == 36633)
		return true;
	//if (npc == 33) 
		//return true;
	if (npc == 37695 && hp_pr < 10.0) 
		return true;
	if (npc == 34800)
		return true;
	return false;
}