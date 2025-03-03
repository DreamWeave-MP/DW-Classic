#ifndef OPENMW_PROCESSORACTORAI_HPP
#define OPENMW_PROCESSORACTORAI_HPP

#include "../ActorProcessor.hpp"

namespace mwmp
{
    class ProcessorActorAI : public ActorProcessor
    {
    public:
        ProcessorActorAI()
        {
            BPP_INIT(ID_ACTOR_AI)
        }

        void Do(ActorPacket &packet, Player &player, BaseActorList &actorList) override
        {
            // Instead of checking cell authority, now the server handles all AI requests
            Cell *serverCell = CellController::get()->getCell(&actorList.cell);

            if (serverCell != nullptr)
            {
                // Process the AI packet and apply logic server-side
                
                // Always call the script hook for AI actions, so scripts can intervene if needed
                Script::Call<Script::CallbackIdentity("OnActorAI")>(player.getId(), actorList.cell.getShortDescription().c_str());
                
                // After processing, send the packet to all players with the cell loaded
                // This ensures AI behavior is consistent for all clients
                serverCell->sendToLoaded(&packet, &actorList);
            }
            else
            {
                // For cells that aren't loaded on the server, just forward the packet
                // This is typically for script-initiated AI
                packet.Send(true);
            }
        }
    };
}

#endif //OPENMW_PROCESSORACTORAI_HPP
