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

/* ScriptData
SDName: Boss_Pandemonius
SD%Complete: 75
SDComment: Not known how void blast is done (amount of rapid cast seems to be related to players in party). All mobs remaining in surrounding area should aggro when engaged.
SDCategory: Auchindoun, Mana Tombs
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Pandemonius
{
    SAY_AGGRO                       = 0,
    SAY_KILL                        = 1,
    SAY_DEATH                       = 2,
    EMOTE_DARK_SHELL                = 3,

    SPELL_VOID_BLAST                = 32325,
    H_SPELL_VOID_BLAST              = 38760,
    SPELL_DARK_SHELL                = 32358,
    H_SPELL_DARK_SHELL              = 38759
};

class boss_pandemonius : public CreatureScript
{
public:
    boss_pandemonius() : CreatureScript("boss_pandemonius") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_pandemoniusAI(creature);
    }

    struct boss_pandemoniusAI : public ScriptedAI
    {
        boss_pandemoniusAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 VoidBlast_Timer;
        uint32 DarkShell_Timer;
        uint32 VoidBlast_Counter;

        void Reset() override
        {
            VoidBlast_Timer = 8000+rand()%15000;
            DarkShell_Timer = 20000;
            VoidBlast_Counter = 0;
        }

        void JustDied(Unit* /*killer*/) override
        {
            Talk(SAY_DEATH);
        }

        void KilledUnit(Unit* /*victim*/) override
        {
            Talk(SAY_KILL);
        }

        void JustEngagedWith(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (VoidBlast_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_VOID_BLAST);
                    VoidBlast_Timer = 500;
                    ++VoidBlast_Counter;
                }

                if (VoidBlast_Counter == 5)
                {
                    VoidBlast_Timer = 15000+rand()%10000;
                    VoidBlast_Counter = 0;
                }
            } else VoidBlast_Timer -= diff;

            if (!VoidBlast_Counter)
            {
                if (DarkShell_Timer <= diff)
                {
                    if (me->IsNonMeleeSpellCasted(false))
                        me->InterruptNonMeleeSpells(true);

                    Talk(EMOTE_DARK_SHELL);

                    DoCast(me, SPELL_DARK_SHELL);
                    DarkShell_Timer = 20000;
                } else DarkShell_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_pandemonius()
{
    new boss_pandemonius();
}
