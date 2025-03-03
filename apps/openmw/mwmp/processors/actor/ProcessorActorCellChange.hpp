#ifndef OPENMW_PROCESSORACTORCELLCHANGE_HPP
#define OPENMW_PROCESSORACTORCELLCHANGE_HPP


#include "../ActorProcessor.hpp"
#include "apps/openmw/mwmp/Main.hpp"
#include "apps/openmw/mwmp/CellController.hpp"

namespace mwmp
{
    class ProcessorActorCellChange final: public ActorProcessor
    {
    public:
        ProcessorActorCellChange()
        {
            BPP_INIT(ID_ACTOR_CELL_CHANGE);
        }

        virtual void Do(ActorPacket &packet, ActorList &actorList)
        {
            // Process actor cell changes no matter who sent them
            // The server now handles all decision-making for actor behavior
            LOG_MESSAGE_SIMPLE(TimedLog::LOG_INFO, "Received %s about %s", strPacketID.c_str(), actorList.cell.getShortDescription().c_str());
            
            Main::get().getCellController()->readCellChange(actorList);
        }
    };
}

#endif //OPENMW_PROCESSORACTORCELLCHANGE_HPP
