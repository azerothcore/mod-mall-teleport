#include "Configuration/Config.h"
#include "Player.h"
#include "Creature.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Define.h"
#include "GossipDef.h"
#include "DataMap.h"
#include "Chat.h"

bool enabled;

class MallTeleportPlayer : public PlayerScript
{
public:
    MallTeleportPlayer() : PlayerScript("MallTeleportPlayer") { }

    void OnLogin(Player* player) override
    {
        QueryResult result = CharacterDatabase.Query("SELECT `AccountId` FROM `premium` WHERE `active`=1 AND `AccountId`={}", player->GetSession()->GetAccountId());

        if (result)
        {
            enabled = true;
        }

        ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00MallTeleportModule |rmodule");
    }
};

class MallTeleport : public CommandScript
{
public:
    MallTeleport() : CommandScript("MallTeleport") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> MallTeleportTable =
        {
            { "Mall", SEC_PLAYER, false, &HandleMallTeleportCommand, "" },
            {"vipMall", SEC_PLAYER, false, &HandleVIPMallTeleportCommand, ""}

        };
        return MallTeleportTable;
    }

    static bool HandleMallTeleportCommand(ChatHandler* handler, char const* /* args */)
    {
        Player* player = handler->GetSession()->GetPlayer();

        QueryResult result = WorldDatabase.Query("SELECT `map`, `position_x`, `position_y`, `position_z`, `orientation` FROM `game_tele` WHERE `name`='PlayerMall'");

        if (!player)
            return false;

        if (player->IsInCombat())
            return false;

        if (!result)
            return false;

        do
        {
            Field* fields = result->Fetch();
            uint32 map = fields[0].Get<uint32>();
            float position_x = fields[1].Get<float>();
            float position_y = fields[2].Get<float>();
            float position_z = fields[3].Get<float>();
            float orientation = fields[4].Get<float>();

            player->TeleportTo(map, position_x, position_y, position_z, orientation);
        } while (result->NextRow());

        return true;
    }

    static bool HandleVIPMallTeleportCommand(ChatHandler* handler, char const* /* args */)
    {
        QueryResult result = WorldDatabase.Query("SELECT `map`, `position_x`, `position_y`, `position_z`, `orientation` FROM `game_tele` WHERE `name`='VIPMall'");

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
        {
            return false;
        }

        if (player->IsInCombat())
        {
            return false;
        }

        if (!enabled)
        {
            player->GetSession()->SendNotification("You do not have access to this command");
            return false;
        }

        do
        {
            Field* fields = result->Fetch();
            uint32 map = fields[0].Get<uint32>();
            float position_x = fields[1].Get<float>();
            float position_y = fields[2].Get<float>();
            float position_z = fields[3].Get<float>();
            float orientation = fields[4].Get<float>();

            player->TeleportTo(map, position_x, position_y, position_z, orientation);
        } while (result->NextRow());

        return true;
    }
};

void AddMallTeleportScripts()
{
    new MallTeleportPlayer();
    new MallTeleport();
}
