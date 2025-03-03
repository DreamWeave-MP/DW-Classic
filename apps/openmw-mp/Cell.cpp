#include "Cell.hpp"

#include <components/openmw-mp/NetworkMessages.hpp>

#include <iostream>
#include "Player.hpp"
#include "Script/Script.hpp"

Cell::Cell(ESM::Cell cell) : cell(cell)
{
    cellActorList.count = 0;
}

Cell::Iterator Cell::begin() const
{
    return players.begin();
}

Cell::Iterator Cell::end() const
{
    return players.end();
}

void Cell::addPlayer(Player *player)
{
    // Ensure the player hasn't already been added
    auto it = find(begin(), end(), player);

    if (it != end())
    {
        LOG_APPEND(TimedLog::LOG_INFO, "- Attempt to add %s to Cell %s again was ignored", player->npc.mName.c_str(), getShortDescription().c_str());
        return;
    }

    auto it2 = find(player->cells.begin(), player->cells.end(), this);
    if (it2 == player->cells.end())
    {
        LOG_APPEND(TimedLog::LOG_INFO, "- Adding %s to Player %s", getShortDescription().c_str(), player->npc.mName.c_str());

        player->cells.push_back(this);
    }

    LOG_APPEND(TimedLog::LOG_INFO, "- Adding %s to Cell %s", player->npc.mName.c_str(), getShortDescription().c_str());

    Script::Call<Script::CallbackIdentity("OnCellLoad")>(player->getId(), getShortDescription().c_str());

    players.push_back(player);
}

void Cell::removePlayer(Player *player, bool cleanPlayer)
{
    for (Iterator it = begin(); it != end(); it++)
    {
        if (*it == player)
        {
            if (cleanPlayer)
            {
                auto it2 = find(player->cells.begin(), player->cells.end(), this);
                if (it2 != player->cells.end())
                {
                    LOG_APPEND(TimedLog::LOG_INFO, "- Removing %s from Player %s", getShortDescription().c_str(), player->npc.mName.c_str());

                    player->cells.erase(it2);
                }
            }

            LOG_APPEND(TimedLog::LOG_INFO, "- Removing %s from Cell %s", player->npc.mName.c_str(), getShortDescription().c_str());

            Script::Call<Script::CallbackIdentity("OnCellUnload")>(player->getId(), getShortDescription().c_str());

            players.erase(it);
            return;
        }
    }
}

void Cell::readActorList(unsigned char packetID, const mwmp::BaseActorList *newActorList)
{
    for (const auto &newActor : newActorList->baseActors)
    {
        bool foundActor = false;

        // If there's already an Actor with this refNum and mpNum in this cell, update it
        for (auto &actor : cellActorList.baseActors)
        {
            if (actor.refNum == newActor.refNum && actor.mpNum == newActor.mpNum)
            {
                if (packetID == ID_ACTOR_POSITION)
                {
                    actor.position = newActor.position;
                    actor.direction = newActor.direction;
                }
                else if (packetID == ID_ACTOR_STATS_DYNAMIC)
                {
                    actor.creatureStats.mDynamic[0] = newActor.creatureStats.mDynamic[0];
                    actor.creatureStats.mDynamic[1] = newActor.creatureStats.mDynamic[1];
                    actor.creatureStats.mDynamic[2] = newActor.creatureStats.mDynamic[2];
                }
                else if (packetID == ID_ACTOR_EQUIPMENT)
                {
                    for (int i = 0; i < 19; ++i)
                        actor.equipmentItems[i] = newActor.equipmentItems[i];
                }
                else if (packetID == ID_ACTOR_AI)
                {
                    // Store AI data on the server
                    actor.aiAction = newActor.aiAction;
                    actor.aiDistance = newActor.aiDistance;
                    actor.aiDuration = newActor.aiDuration;
                    actor.aiShouldRepeat = newActor.aiShouldRepeat;
                    actor.aiCoordinates = newActor.aiCoordinates;
                    actor.hasAiTarget = newActor.hasAiTarget;
                    
                    if (actor.hasAiTarget)
                        actor.aiTarget = newActor.aiTarget;
                }

                foundActor = true;
                break;
            }
        }

        if (!foundActor)
            cellActorList.baseActors.push_back(newActor);
    }

    cellActorList.count = cellActorList.baseActors.size();
}

bool Cell::containsActor(int refNum, int mpNum)
{
    for (unsigned int i = 0; i < cellActorList.baseActors.size(); i++)
    {
        mwmp::BaseActor actor = cellActorList.baseActors.at(i);

        if (actor.refNum == refNum && actor.mpNum == mpNum)
            return true;
    }
    return false;
}

mwmp::BaseActor *Cell::getActor(int refNum, int mpNum)
{
    for (auto &actor : cellActorList.baseActors)
    {
        if (actor.refNum == refNum && actor.mpNum == mpNum)
        {
            return &actor;
        }
    }

    return nullptr;
}

void Cell::removeActors(const mwmp::BaseActorList *newActorList)
{
    for (std::vector<mwmp::BaseActor>::iterator it = cellActorList.baseActors.begin(); it != cellActorList.baseActors.end();)
    {
        int refNum = (*it).refNum;
        int mpNum = (*it).mpNum;

        bool foundActor = false;

        for (unsigned int i = 0; i < newActorList->count; i++)
        {
            mwmp::BaseActor newActor = newActorList->baseActors.at(i);

            if (newActor.refNum == refNum && newActor.mpNum == mpNum)
            {
                it = cellActorList.baseActors.erase(it);
                foundActor = true;
                break;
            }
        }

        if (!foundActor)
            it++;
    }

    cellActorList.count = cellActorList.baseActors.size();
}

RakNet::RakNetGUID *Cell::getAuthority()
{
    return &authorityGuid;
}

void Cell::setAuthority(const RakNet::RakNetGUID& guid)
{
    authorityGuid = guid;
}

mwmp::BaseActorList *Cell::getActorList()
{
    return &cellActorList;
}

Cell::TPlayers Cell::getPlayers() const
{
    return players;
}

void Cell::sendToLoaded(mwmp::ActorPacket *actorPacket, mwmp::BaseActorList *baseActorList) const
{
    if (players.empty())
        return;

    std::list <Player*> plList;

    for (auto pl : players)
    {
        if (pl != nullptr && !pl->npc.mName.empty())
            plList.push_back(pl);
    }

    plList.sort();
    plList.unique();

    for (auto pl : plList)
    {
        if (pl->guid == baseActorList->guid) continue;

        actorPacket->setActorList(baseActorList);

        // Send the packet to this eligible guid
        actorPacket->Send(pl->guid);
    }
}

void Cell::sendToLoaded(mwmp::ObjectPacket *objectPacket, mwmp::BaseObjectList *baseObjectList) const
{
    if (players.empty())
        return;

    std::list <Player*> plList;

    for (auto pl : players)
    {
        if (pl != nullptr && !pl->npc.mName.empty())
            plList.push_back(pl);
    }

    plList.sort();
    plList.unique();

    for (auto pl : plList)
    {
        if (pl->guid == baseObjectList->guid) continue;

        objectPacket->setObjectList(baseObjectList);

        // Send the packet to this eligible guid
        objectPacket->Send(pl->guid);
    }
}

std::string Cell::getShortDescription() const
{
    return cell.getShortDescription();
}

// New method to check if an actor is currently executing an AI package
bool Cell::isActorAiActive(int refNum, int mpNum)
{
    mwmp::BaseActor *actor = getActor(refNum, mpNum);
    
    if (actor == nullptr)
        return false;
        
    // Actions other than CANCEL are considered active
    return actor->aiAction != mwmp::BaseActorList::CANCEL;
}
