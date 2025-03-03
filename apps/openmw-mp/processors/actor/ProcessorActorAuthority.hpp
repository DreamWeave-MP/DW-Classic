#ifndef OPENMW_PROCESSORACTORAUTHORITY_HPP
#define OPENMW_PROCESSORACTORAUTHORITY_HPP

#include "../ActorProcessor.hpp"

namespace mwmp
{
    class ProcessorActorAuthority : public ActorProcessor
    {
    public:
        ProcessorActorAuthority()
        {
            BPP_INIT(ID_ACTOR_AUTHORITY)
        }

        void Do(ActorPacket &packet, Player &player, BaseActorList &actorList) override
        {
            Cell *cell = CellController::get()->getCell(&actorList.cell);

            if (cell == nullptr)
            {
                cell = new Cell(actorList.cell);
                CellController::get()->addCell(cell);
            }

            LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "Received %s about %s from %s", strPacketID.c_str(),
                actorList.cell.getShortDescription().c_str(), player.getName().c_str());

            // Only set authority for world loading/unloading purposes
            // AI behavior is managed centrally by the server
            cell->setAuthority(player.guid);

            // Authority information is now only used for cell loading state,
            // not for determining who controls AI
            
            Script::Call<Script::CallbackIdentity("OnCellLoad")>(player.getId(),
                actorList.cell.getShortDescription().c_str());

            packet.Send(true);
        }
    };
}

#endif //OPENMW_PROCESSORACTORAUTHORITY_HPP 