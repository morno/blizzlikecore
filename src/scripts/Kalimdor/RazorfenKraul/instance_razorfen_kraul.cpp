/*
 * Copyright (C) 2013  BlizzLikeGroup
 * BlizzLikeCore integrates as part of this file: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Instance_Razorfen_Kraul
Complete(%):
Comment:
Category: Razorfen Kraul
EndScriptData */

#include "ScriptPCH.h"
#include "razorfen_kraul.h"

#define WARD_KEEPERS_NR 2

struct instance_razorfen_kraul : public ScriptedInstance
{
    instance_razorfen_kraul(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint64 DoorWardGUID;
    uint32 WardCheck_Timer;
    int WardKeeperAlive;

    void Initialize()
    {
        WardKeeperAlive = 1;
        WardCheck_Timer = 4000;
        DoorWardGUID = 0;
    }

    Player* GetPlayerInMap()
    {
        Map::PlayerList const& players = instance->GetPlayers();

        if (!players.isEmpty())
        {
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* plr = itr->getSource())
                    return plr;
            }
        }
        debug_log("BSCR: Instance Razorfen Kraul: GetPlayerInMap, but PlayerList is empty!");
        return NULL;
    }

    void OnGameObjectCreate(GameObject* pGo, bool /*apply*/)
    {
        switch (pGo->GetEntry())
        {
        case 21099: DoorWardGUID = pGo->GetGUID(); break;
        }
    }

    void Update(uint32 diff)
    {
        if (WardCheck_Timer <= diff)
        {
            HandleGameObject(DoorWardGUID, WardKeeperAlive);
            WardKeeperAlive = 0;
            WardCheck_Timer = 4000;
        } else
            WardCheck_Timer -= diff;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch (type)
        {
            case TYPE_WARD_KEEPERS:
                if (data == NOT_STARTED)
                    WardKeeperAlive = 1;
                break;
        }
    }

};

InstanceData* GetInstanceData_instance_razorfen_kraul(Map* pMap)
{
    return new instance_razorfen_kraul(pMap);
}

void AddSC_instance_razorfen_kraul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_razorfen_kraul";
    newscript->GetInstanceData = &GetInstanceData_instance_razorfen_kraul;
    newscript->RegisterSelf();
}
