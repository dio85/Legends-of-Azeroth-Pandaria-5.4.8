/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"
#include "CreatureTextMgr.h"
#include "Containers.h"
#include "MoveSplineInit.h"
#include "dragon_soul.h"

enum ScriptedTexts
{
    SAY_AGGRO       = 0,
    SAY_DEATH       = 1,
    SAY_INTRO       = 2,
    SAY_KILL        = 3,
    SAY_SHADOWS     = 4,
    SAY_BLOOD       = 5,
    SAY_VOID        = 6,
    SAY_AGGRO_1     = 7,
    SAY_DEATH_1     = 8,
    SAY_INTRO_1     = 9,
    SAY_KILL_1      = 10,
    SAY_SHADOWS_1   = 11,
    SAY_BLOOD_1     = 12,
    SAY_VOID_1      = 13,
};

enum Spells
{
    SPELL_BERSERK                           = 26662,
    SPELL_FOCUSED_ANGER                     = 104543,
    SPELL_PSYCHIC_DRAIN                     = 104323,
    SPELL_PSYCHIC_DRAIN_DMG                 = 104322,
    SPELL_DISRUPTING_SHADOWS                = 103434,
    SPELL_DISRUPTING_SHADOWS_DMG            = 103948, // on dispel
    SPELL_VOID_OF_THE_UNMAKING_1            = 103521, // on void
    SPELL_VOID_OF_THE_UNMAKING_VISUAL       = 109187, // visual
    SPELL_VOID_OF_THE_UNMAKING_SUMMON_1     = 103571,
    SPELL_VOID_OF_THE_UNMAKING_SUMMON_2     = 110780, // ?
    SPELL_VOID_OF_THE_UNMAKING_PREVENT      = 103627,
    SPELL_VOID_OF_THE_UNMAKING_DUMMY_1      = 103946, // boss casts beam on void
    SPELL_VOID_DIFFUSION_DMG                = 103527,
    SPELL_VOID_DIFFUSION_BUFF               = 106836,
    SPELL_VOID_DIFFUSION_DEBUFF             = 104031, // debuf on boss
    SPELL_BLACK_BLOOD_ERUPTION              = 108799,
    SPELL_BLACK_BLOOD_ERUPTION_DMG          = 108794,
    SPELL_TANTRUM                           = 103953,
    SPELL_DARKNESS                          = 109413,
    SPELL_EYE_OF_GORATH                     = 109190,
    SPELL_CLAW_OF_GORATH                    = 109191,
    SPELL_FLAIL_OF_GORATH                   = 109193,

    SPELL_ZONOZZ_WHISPER_AGGRO              = 109874,
    SPELL_ZONOZZ_WHISPER_INTRO              = 109875,
    SPELL_ZONOZZ_WHISPER_DEATH              = 109876,
    SPELL_ZONOZZ_WHISPER_KILL               = 109877,
    SPELL_ZONOZZ_WHISPER_BLOOD              = 109878,
    SPELL_ZONOZZ_WHISPER_SHADOWS            = 109879,
    SPELL_ZONOZZ_WHISPER_VOID               = 109880,

    SPELL_BLOOD_OF_GORATH_DUMMY             = 103932,
    SPELL_BLACK_BLOOD_OF_GORATH             = 104377, // by tentacles
    SPELL_BLACK_BLOOD_OF_GORATH_SELF        = 104378, // by boss

    SPELL_SLUDGE_SPEW                       = 110297,
    SPELL_WILD_FLAIL                        = 109199,

    SPELL_OOZE_SPIT                         = 109396,

    SPELL_SHADOW_GAZE                       = 104347,
};

enum Adds
{
    NPC_VOID_OF_THE_UNMAKING_1  = 55334,
    NPC_VOID_OF_THE_UNMAKING_2  = 58473, // ? second summon spell
    NPC_CLAW_OF_GORATH          = 55418,
    NPC_EYE_OF_GORATH           = 55416,
    NPC_FLAIL_OF_GORATH         = 55417,
};

enum Events
{
    EVENT_BERSERK               = 1,
    EVENT_FOCUSED_ANGER         = 2,
    EVENT_PSYCHIC_DRAIN         = 3,
    EVENT_DISRUPTING_SHADOWS    = 4,
    EVENT_VOID_OF_THE_UNMAKING  = 5,
    EVENT_CHECK_DISTANCE        = 6,
    EVENT_CONTINUE              = 7,
    EVENT_UPDATE_AURA           = 8,
    EVENT_TANTRUM_1             = 9,
    EVENT_TANTRUM_2             = 10,
    EVENT_TANTRUM_3             = 11,
    EVENT_END_TANTRUM_1         = 12,
    EVENT_END_TANTRUM_2         = 13,
    EVENT_SLUDGE_SPEW           = 14,
    EVENT_WILD_FLAIL            = 15,
    EVENT_OOZE_SPIT             = 16,
    EVENT_SHADOW_GAZE           = 17,
};

enum MiscData
{
    POINT_VOID          = 1,
    DATA_ACHIEVE        = 2,
    DATA_PHASE_COUNT    = 3,
    DATA_VOID           = 4,
};

const Position centerPos = { -1769.329956f, -1916.869995f, -226.28f, 0.0f };
const Position tentaclePos[14] = 
{
    { -1702.57f,     -1884.71f,     -221.513f, 3.63029f  },
    { -1801.84f,     -1851.69f,     -221.436f, 5.2709f   },
    { -1792.2f,      -1988.63f,     -221.373f, 1.41372f  },
    { -1834.55f,     -1952.28f,     -221.38f,  0.628318f },
    { -1734.35f,     -1983.18f,     -221.445f, 2.14675f  },
    { -1745.46f,     -1847.31f,     -221.437f, 4.43314f  },
    { -1839.37f,     -1895.09f,     -221.381f, 5.98648f  },
    { -1696.95f,     -1941.09f,     -221.292f, 1.90241f  },

    { -1739.246826f, -1885.623047f, -226.28f,  4.44f     },
    { -1791.315552f, -1885.340820f, -226.06f,  4.94f     },
    { -1801.412476f, -1939.772217f, -226.13f,  0.84f     },
    { -1759.518921f, -1957.948853f, -226.00f,  1.67f     },

    { -1774.998413f, -1937.956299f, -226.35f,  1.30f     },
    { -1748.312256f, -1901.348877f, -226.17f,  3.87f     }
};

class boss_warlord_zonozz: public CreatureScript
{
    public:
        boss_warlord_zonozz() : CreatureScript("boss_warlord_zonozz") { }

        struct boss_warlord_zonozzAI : public BossAI
        {
            boss_warlord_zonozzAI(Creature* creature) : BossAI(creature, DATA_ZONOZZ)
            {             
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
                me->setActive(true);
                bIntro = false;
                phaseCount = 0;
            }

            void Reset() override
            {
                _Reset();

                me->SetReactState(REACT_AGGRESSIVE);

                me->GetMap()->SetWorldState(WORLDSTATE_PING_PONG_CHAMPION, 0);
                phaseCount = 0;

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLACK_BLOOD_OF_GORATH);
            }

            void MoveInLineOfSight(Unit* who) override
            {
                if (bIntro)
                    return;

                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (!me->IsWithinDistInMap(who, 100.0f, false))
                    return;

                Talk(SAY_INTRO);
                DoCastAOE(SPELL_ZONOZZ_WHISPER_INTRO, true);
                bIntro = true;
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                if (instance->GetBossState(DATA_MORCHOK) != DONE)
                {
                    EnterEvadeMode();
                    instance->DoNearTeleportPlayers(portalsPos[0]);
                    return;
                }

                me->GetMap()->SetWorldState(WORLDSTATE_PING_PONG_CHAMPION, 0);
                phaseCount = 0;

                me->SetReactState(REACT_AGGRESSIVE);

                Talk(SAY_AGGRO);
                DoCastAOE(SPELL_ZONOZZ_WHISPER_AGGRO, true);

                events.ScheduleEvent(EVENT_CHECK_DISTANCE, 5000);
                events.ScheduleEvent(EVENT_BERSERK, 6 * MINUTE * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_PSYCHIC_DRAIN, 13000);
                events.ScheduleEvent(EVENT_FOCUSED_ANGER, 10500);
                events.ScheduleEvent(EVENT_DISRUPTING_SHADOWS, urand(25000, 30000));
                events.ScheduleEvent(EVENT_VOID_OF_THE_UNMAKING, 5500);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLACK_BLOOD_OF_GORATH);

                instance->SetBossState(DATA_ZONOZZ, IN_PROGRESS);

                std::list<Creature*> trashmobs;
                GetCreatureListWithEntryInGrid(trashmobs, me, NPC_EYE_OF_GORATH_TRASH, 150);
                GetCreatureListWithEntryInGrid(trashmobs, me, NPC_FLAIL_OF_GORATH_TRASH, 150);
                GetCreatureListWithEntryInGrid(trashmobs, me, NPC_CLAW_OF_GORATH_TRASH, 150);
                for (std::list<Creature*>::const_iterator itr = trashmobs.begin(); itr != trashmobs.end(); ++itr)
                    if (Creature* trash = *itr)
                        if (trash->IsAlive())
                            trash->SetInCombatWithZone();
            }

            void JustDied(Unit* /*killer*/) override
            {
                _JustDied();

                Talk(SAY_DEATH);
                DoCastAOE(SPELL_ZONOZZ_WHISPER_DEATH, true);

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLACK_BLOOD_OF_GORATH);
            }

            void JustSummoned(Creature* summon) override
            {
                BossAI::JustSummoned(summon);

                switch (summon->GetEntry())
                {
                    case NPC_VOID_OF_THE_UNMAKING_1:
                        summon->SetOrientation(me->GetOrientation());
                        DoCast(summon, SPELL_VOID_OF_THE_UNMAKING_DUMMY_1);
                        break;
                    case NPC_EYE_OF_GORATH:
                        if (!IsHeroic())
                            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        if (IsHeroic())
                            DoCastAOE(SPELL_BLACK_BLOOD_OF_GORATH, true);
                        break;
                    case NPC_CLAW_OF_GORATH:
                    case NPC_FLAIL_OF_GORATH:
                        if (IsHeroic())
                            DoCastAOE(SPELL_BLACK_BLOOD_OF_GORATH, true);
                        break;
                    default:
                        break;                            
                }
            }

            void SummonedCreatureDies(Creature* summon, Unit* killer) override
            {
                BossAI::SummonedCreatureDies(summon, killer);

                switch (summon->GetEntry())
                {
                    case NPC_VOID_OF_THE_UNMAKING_1:
                    case NPC_EYE_OF_GORATH:
                    case NPC_CLAW_OF_GORATH:
                    case NPC_FLAIL_OF_GORATH:
                        instance->DoRemoveAuraFromStackOnPlayers(SPELL_BLACK_BLOOD_OF_GORATH);
                        break;
                    default:
                        break;                            
                }
            }

            void SetData(uint32 type, uint32 data) override
            {
                if (type == DATA_ACHIEVE)
                    me->GetMap()->SetWorldState(WORLDSTATE_PING_PONG_CHAMPION, 1);
                else if (type == DATA_VOID)
                {   
                    me->CastCustomSpell(SPELL_VOID_DIFFUSION_DEBUFF, SPELLVALUE_AURA_STACK, data, me, true);
                    me->RemoveAura(SPELL_FOCUSED_ANGER);
                    events.CancelEvent(EVENT_DISRUPTING_SHADOWS);
                    events.CancelEvent(EVENT_PSYCHIC_DRAIN);
                    events.CancelEvent(EVENT_FOCUSED_ANGER);
                    events.CancelEvent(EVENT_VOID_OF_THE_UNMAKING);
                    events.ScheduleEvent(EVENT_TANTRUM_1, 1500);
                    phaseCount++;
                }
            }

            uint32 GetData(uint32 type) const override
            {
                if (type == DATA_PHASE_COUNT)
                    return phaseCount;

                return 0;
            }

            void KilledUnit(Unit* victim) override
            {
                if (victim && victim->GetTypeId() == TYPEID_PLAYER)
                {
                    Talk(SAY_KILL);
                    DoCastAOE(SPELL_ZONOZZ_WHISPER_KILL, true);
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->GetDistance(me->GetHomePosition()) > 150.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHECK_DISTANCE:
                            if (me->GetDistance(me->GetHomePosition()) > 150.0f)
                            {
                                events.Reset();
                                EnterEvadeMode();
                                return;
                            }
                            events.ScheduleEvent(EVENT_CHECK_DISTANCE, 5000);
                            break;
                        case EVENT_BERSERK:
                            DoCast(me, SPELL_BERSERK);
                            break;
                        case EVENT_FOCUSED_ANGER:
                            DoCast(me, SPELL_FOCUSED_ANGER);
                            events.ScheduleEvent(EVENT_FOCUSED_ANGER, 6500);
                            break;
                        case EVENT_PSYCHIC_DRAIN:
                            DoCastVictim(SPELL_PSYCHIC_DRAIN);
                            events.ScheduleEvent(EVENT_PSYCHIC_DRAIN, urand(20000, 25000));
                            break;
                        case EVENT_DISRUPTING_SHADOWS:
                            Talk(SAY_SHADOWS);
                            me->CastCustomSpell(SPELL_DISRUPTING_SHADOWS, SPELLVALUE_MAX_TARGETS, RAID_MODE(3, 8, 3, 8), me);
                            events.ScheduleEvent(EVENT_DISRUPTING_SHADOWS, urand(25000, 30000));
                            break;
                        case EVENT_VOID_OF_THE_UNMAKING:
                            summons.DespawnEntry(NPC_VOID_OF_THE_UNMAKING_1);
                            Talk(SAY_VOID);
                            DoCast(me, SPELL_VOID_OF_THE_UNMAKING_SUMMON_1);
                            events.ScheduleEvent(EVENT_VOID_OF_THE_UNMAKING, 90300);
                            break;
                        case EVENT_TANTRUM_1:
                            me->SetReactState(REACT_PASSIVE);
                            me->AttackStop();
                            me->NearTeleportTo(centerPos.GetPositionX(), centerPos.GetPositionY(), centerPos.GetPositionZ(), centerPos.GetOrientation());
                            events.ScheduleEvent(EVENT_TANTRUM_2, 3000);
                            break;
                        case EVENT_TANTRUM_2:
                            Talk(SAY_BLOOD);
                            DoCast(me, SPELL_DARKNESS, true);
                            if (!IsHeroic())
                                DoCast(me, SPELL_BLACK_BLOOD_OF_GORATH_SELF, true);
                            DoCast(me, SPELL_TANTRUM);
                            switch (GetDifficulty())
                            {
                                case RAID_DIFFICULTY_10MAN_NORMAL:
                                    SpawnRandomTentacles(4, 0, 0);
                                    break;                                    
                                case RAID_DIFFICULTY_25MAN_NORMAL:
                                    SpawnRandomTentacles(8, 0, 0);
                                    break;
                                case RAID_DIFFICULTY_10MAN_HEROIC:
                                    SpawnRandomTentacles(4, 2, 1);
                                    break;
                                case RAID_DIFFICULTY_25MAN_HEROIC:
                                    SpawnRandomTentacles(8, 4, 2);
                                    break;
                                default:
                                    break;
                            }
                            events.ScheduleEvent(EVENT_END_TANTRUM_1, 11000);
                            events.ScheduleEvent(EVENT_END_TANTRUM_2, 30000);
                            break;
                        case EVENT_END_TANTRUM_1:
                            me->SetReactState(REACT_AGGRESSIVE);
                            AttackStart(me->GetVictim());
                            break;
                        case EVENT_END_TANTRUM_2:
                            if (!IsHeroic())
                                summons.DespawnEntry(NPC_EYE_OF_GORATH);
                            me->RemoveAura(SPELL_VOID_OF_THE_UNMAKING_PREVENT);
                            events.ScheduleEvent(EVENT_VOID_OF_THE_UNMAKING, urand(13000, 14000));
                            events.ScheduleEvent(EVENT_FOCUSED_ANGER, 6000);
                            events.ScheduleEvent(EVENT_DISRUPTING_SHADOWS, 6000);
                            events.ScheduleEvent(EVENT_PSYCHIC_DRAIN, 21000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool bIntro;
            uint32 phaseCount;

            void SpawnRandomTentacles(uint32 max_eyes, uint32 max_flails, uint32 max_claws)
            {
                if (max_eyes > 8)
                    max_eyes = 8;
                if (max_flails > 4)
                    max_flails = 4;
                if (max_claws > 2)
                    max_claws = 2;

                for (uint8 i = 0; i < max_eyes; ++i)
                    me->SummonCreature(NPC_EYE_OF_GORATH, tentaclePos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                
                for (uint8 i = 8; i < (8 + max_flails); ++i)
                    me->SummonCreature(NPC_FLAIL_OF_GORATH, tentaclePos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                
                for (uint8 i = 12; i < (12 + max_claws); ++i)
                    me->SummonCreature(NPC_CLAW_OF_GORATH, tentaclePos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<boss_warlord_zonozzAI>(creature);
        }
};

class npc_warlord_zonozz_void_of_the_unmaking : public CreatureScript
{
    public:
        npc_warlord_zonozz_void_of_the_unmaking() : CreatureScript("npc_warlord_zonozz_void_of_the_unmaking") { }

        struct npc_warlord_zonozz_void_of_the_unmakingAI : public ScriptedAI
        {
            npc_warlord_zonozz_void_of_the_unmakingAI(Creature* creature) : ScriptedAI(creature)
            {
                //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                bAura = false;
                bExplode = false;
                SetCombatMovement(false);
            }

            void IsSummonedBy(Unit* /*summoner*/) override
            {
                me->CastSpell(me, 62371, true); // Handy aura for total immunity and hidden clientside
                //me->SetSpeed(MOVE_RUN, 0.428571f, true);
                //me->SetSpeed(MOVE_WALK, 0.428571f, true);
                //me->SetSpeed(MOVE_FLIGHT, 0.428571f, true);
                me->SetSpeed(MOVE_RUN, 0.6f, true);
                me->SetSpeed(MOVE_WALK, 0.6f, true);
                me->SetSpeed(MOVE_FLIGHT, 0.6f, true);
                events.ScheduleEvent(EVENT_CHECK_DISTANCE, 5000);
                events.ScheduleEvent(EVENT_CONTINUE, 5000);
            }
 
            void DamageTaken(Unit* /*attacker*/, uint32& damage) override
            {
                damage = 0;
            }

            SpellMissInfo SpellHitResult(Unit* attacker, SpellInfo const* /*spell*/, Spell const* /*spellInstance*/) override
            {
                return attacker != me && attacker->GetEntry() != NPC_ZONOZZ ? SPELL_MISS_IMMUNE : SPELL_MISS_NONE;
            }

            void UpdateAI(uint32 diff) override
            {
                if (bExplode)
                    return;

                if (!UpdateVictim())
                    return;

                if (centerPos.GetExactDist2d(me->GetPositionX(), me->GetPositionY()) > 95.0f)
                {
                    bExplode = true;
                    events.Reset();
                    me->StopMoving();
                    DoCastAOE(SPELL_BLACK_BLOOD_ERUPTION);
                    me->DespawnOrUnsummon(5000);
                    return;
                }

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CONTINUE:
                        {
                            DoCast(me, SPELL_VOID_OF_THE_UNMAKING_VISUAL, true);
                            bAura = true;
                            _MovePosition(200.0f, me->GetOrientation());
                            //me->GetMotionMaster()->MovePoint(POINT_VOID, pos);
                            break;
                        }
                        case EVENT_CHECK_DISTANCE:
                        {
                            if (!bAura)
                            {
                                events.ScheduleEvent(EVENT_CHECK_DISTANCE, 500);
                                break;
                            }

                            if (Player* player = me->FindNearestPlayer(5.0f))
                            {
                                if (Aura const* aur = me->GetAura(SPELL_VOID_DIFFUSION_BUFF))
                                {
                                    if (aur->GetStackAmount() >= 9)
                                        if (InstanceScript* instance = me->GetInstanceScript())
                                            if (Creature* pZonozz = ObjectAccessor::GetCreature(*me, instance->GetGuidData(DATA_ZONOZZ)))
                                                pZonozz->AI()->SetData(DATA_ACHIEVE, 1);
                                }
                                
                                me->RemoveAura(SPELL_VOID_OF_THE_UNMAKING_VISUAL);
                                bAura = false;
                                DoCastAOE(SPELL_VOID_DIFFUSION_DMG);
                                me->StopMoving();
                                float ang = me->GetAngle(player->GetPositionX(), player->GetPositionY());

                                if (me->NormalizeOrientation(me->GetOrientation() - ang) < (M_PI / 4.0f))
                                    ang = me->GetOrientation();

                                _MovePosition(200.0f, ang + M_PI);
                                
                                //me->GetMotionMaster()->MovePoint(POINT_VOID, pos);
                                events.ScheduleEvent(EVENT_UPDATE_AURA, 4000);
                                events.ScheduleEvent(EVENT_CHECK_DISTANCE, 4000);
                            }
                            else if (Creature* pZonozz = me->FindNearestCreature(NPC_ZONOZZ, 5.0f))
                            {
                                uint8 stacks = 1;
                                if (Aura const* aur = me->GetAura(SPELL_VOID_DIFFUSION_BUFF))
                                    stacks = aur->GetStackAmount();

                                pZonozz->AI()->SetData(DATA_VOID, stacks);
                                events.Reset();
                                me->StopMoving();
                                me->DespawnOrUnsummon(2000);
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_DISTANCE, 200);
                            break;
                        }
                        case EVENT_UPDATE_AURA:
                            DoCast(me, SPELL_VOID_OF_THE_UNMAKING_VISUAL, true);
                            bAura = true;
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap events;
            bool bAura;
            bool bExplode;

            void _MovePosition(float dist, float angle)
            {
                angle = me->NormalizeOrientation(angle);

                float cur_dist = 5.0f;
                Movement::MoveSplineInit init(me);
                bool bPassed = false;

                while (!bPassed)
                {
                    float x = me->GetPositionX() + (cur_dist * std::cos(angle));
                    float y = me->GetPositionY() + (cur_dist * std::sin(angle));
                    float z = me->GetPositionZ();
                    float center_dist = centerPos.GetExactDist2d(x, y);
                    if (center_dist > 100.0f || cur_dist > dist)
                        bPassed = true;
                    else
                    {
                        G3D::Vector3 point;
                        point.x = x;
                        point.y = y;
                        if (center_dist > 40.0f)
                            z = -225.0f + ((center_dist - 40.0f) * 0.1333f);
                        else
                            z = -225.0f;

                        point.z =  z;
                        init.Path().push_back(point);
                        cur_dist += 5.0f;
                    }
                }

                if (!init.Path().empty())
                {
                    init.SetWalk(false);
                    init.Launch();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_warlord_zonozz_void_of_the_unmakingAI>(creature);
        }
};

class npc_warlord_zonozz_tentacle : public CreatureScript
{
    public:
        npc_warlord_zonozz_tentacle() : CreatureScript("npc_warlord_zonozz_tentacle") { }

        struct npc_warlord_zonozz_tentacleAI : public ScriptedAI
        {
            npc_warlord_zonozz_tentacleAI(Creature* creature) : ScriptedAI(creature)
            {
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                SetCombatMovement(false);
            }

            void Reset() override
            {
                events.Reset();
            }

            void JustEngagedWith(Unit* /*who*/) override
            {
                switch (me->GetEntry())
                {
                    case NPC_FLAIL_OF_GORATH:
                        events.ScheduleEvent(EVENT_SLUDGE_SPEW, urand(10000, 15000));
                        events.ScheduleEvent(EVENT_WILD_FLAIL, 15000);
                        break;
                    case NPC_CLAW_OF_GORATH:
                        events.ScheduleEvent(EVENT_OOZE_SPIT, 8000);
                        break;
                    case NPC_EYE_OF_GORATH:
                        events.ScheduleEvent(EVENT_SHADOW_GAZE, urand(3000, 15000));
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SLUDGE_SPEW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                DoCast(target, SPELL_SLUDGE_SPEW);
                            events.ScheduleEvent(EVENT_SLUDGE_SPEW, urand(12000, 20000));
                            break;
                        case EVENT_WILD_FLAIL:
                            DoCastAOE(SPELL_WILD_FLAIL);
                            events.ScheduleEvent(EVENT_WILD_FLAIL, urand(7000, 10000));
                            break;
                        case EVENT_OOZE_SPIT:
                            if (!me->IsWithinMeleeRange(me->GetVictim()))
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                                    DoCast(target, SPELL_OOZE_SPIT);
                            events.ScheduleEvent(EVENT_OOZE_SPIT, 6000);
                            break;
                        case EVENT_SHADOW_GAZE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -int32(SPELL_SHADOW_GAZE)))
                                DoCast(target, SPELL_SHADOW_GAZE);
                            events.ScheduleEvent(EVENT_SHADOW_GAZE, urand(8000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                if (me->GetEntry() != NPC_EYE_OF_GORATH)
                    DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetInstanceAI<npc_warlord_zonozz_tentacleAI>(creature);
        }
};

class spell_warlord_zonozz_whisper : public SpellScriptLoader
{
    public:
        spell_warlord_zonozz_whisper() : SpellScriptLoader("spell_warlord_zonozz_whisper") { }

        class spell_warlord_zonozz_whisper_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warlord_zonozz_whisper_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                if (!GetCaster()->ToCreature() || !GetHitUnit()->ToPlayer())
                    return;

                uint32 textId = 0;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_ZONOZZ_WHISPER_AGGRO: textId = SAY_AGGRO_1;  break;
                    case SPELL_ZONOZZ_WHISPER_DEATH: textId = SAY_DEATH_1; break;
                    case SPELL_ZONOZZ_WHISPER_INTRO: textId = SAY_INTRO_1; break;
                    case SPELL_ZONOZZ_WHISPER_KILL: textId = SAY_KILL_1; break;
                    case SPELL_ZONOZZ_WHISPER_BLOOD: textId = SAY_BLOOD_1; break;
                    case SPELL_ZONOZZ_WHISPER_SHADOWS: textId = SAY_SHADOWS_1; break;
                    case SPELL_ZONOZZ_WHISPER_VOID: textId = SAY_VOID_1; break;
                    default: return;
                }

                sCreatureTextMgr->SendChat(GetCaster()->ToCreature(), textId, GetHitUnit(), CHAT_MSG_MONSTER_WHISPER, LANG_ADDON, TEXT_RANGE_AREA);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_warlord_zonozz_whisper_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warlord_zonozz_whisper_SpellScript();
        }
};

class spell_warlord_zonozz_disrupting_shadows : public SpellScriptLoader
{
    public:
        spell_warlord_zonozz_disrupting_shadows() : SpellScriptLoader("spell_warlord_zonozz_disrupting_shadows") { }

        class spell_warlord_zonozz_disrupting_shadows_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warlord_zonozz_disrupting_shadows_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                if (targets.size() <= 1)
                    return;

                if (Creature* pZonozz = GetCaster()->ToCreature())
                    if (Unit* pTank = pZonozz->GetVictim())
                        targets.remove(pTank);

                uint32 max_targets = (GetCaster()->GetMap()->Is25ManRaid() ? 5 : 2);
                Trinity::Containers::RandomResizeList(targets, max_targets);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warlord_zonozz_disrupting_shadows_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warlord_zonozz_disrupting_shadows_SpellScript();
        }
};

void AddSC_boss_warlord_zonozz()
{
    new boss_warlord_zonozz();
    new npc_warlord_zonozz_void_of_the_unmaking();
    new npc_warlord_zonozz_tentacle();
    new spell_warlord_zonozz_whisper();
    new spell_warlord_zonozz_disrupting_shadows();
}
