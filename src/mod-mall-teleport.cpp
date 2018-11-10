#include "Configuration/Config.h"
#include "Player.h"
#include "Creature.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Define.h"
#include "GossipDef.h"
#include "DataMap.h"

bool enabled;

class MallTeleportPlayer : public PlayerScript
{
public:
    MallTeleportPlayer() : PlayerScript("MallTeleportPlayer") { }

    void OnLogin(Player* p) override
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT AccountId FROM premium WHERE active = 1 AND AccountId = %u", p->GetSession()->GetAccountId());

        if (result)
        {
            Field* fields = result->Fetch();
            enabled = true;
        }
        ChatHandler(p->GetSession()).SendSysMessage("This server is running the |cff4CFF00MallTeleportModule |rmodule");
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

    static bool HandleMallTeleportCommand(ChatHandler* handler, char const* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
        QueryResult result = WorldDatabase.PQuery("SELECT `map`, `position_x`, `position_y`, `position_z`, `orientation` FROM game_tele WHERE name = 'PlayerMall'");

        if (!me)
            return false;

        if (me->IsInCombat())
            return false;

        if (!result)
            return false;

        do
        {
            Field* fields = result->Fetch();
            uint32 map = fields[0].GetUInt32();
            float position_x = fields[1].GetFloat();
            float position_y = fields[2].GetFloat();
            float position_z = fields[3].GetFloat();
            float orientation = fields[4].GetFloat();

            me->TeleportTo(map, position_x, position_y, position_z, orientation);
        } while (result->NextRow());
    }

    static bool HandleVIPMallTeleportCommand(ChatHandler* handler, char const* args)
    {
        QueryResult result = WorldDatabase.PQuery("SELECT `map`, `position_x`, `position_y`, `position_z`, `orientation` FROM game_tele WHERE name = 'VIPMall'");
        Player* p = handler->GetSession()->GetPlayer();

        if (!p)
            return false;

        if (p->IsInCombat())
            return false;

        if (!enabled)
            p->GetSession()->SendNotification("You do not have access to this command");
            return false;
            
        do
        {
            Field* fields = result->Fetch();
            uint32 map = fields[0].GetUInt32();
            float position_x = fields[1].GetFloat();
            float position_y = fields[2].GetFloat();
            float position_z = fields[3].GetFloat();
            float orientation = fields[4].GetFloat();

            p->TeleportTo(map, position_x, position_y, position_z, orientation);
        } while (result->NextRow());

    }
};

class MallTeleport_conf : public WorldScript
{
public:
    MallTeleport_conf() : WorldScript("MallTeleport_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string cfg_path = _CONF_DIR;
            std::string cfg_file = cfg_path + "/PlayerProgrssion.conf";

#ifdef WIN32
            cfg_file = "PlayerProgrssion.conf";
#endif
            std::string cfg_def_file = cfg_file + ".dist";
            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());
        }
    }
};

void AddMallTeleportScripts()
{
    new MallTeleportPlayer();
    new MallTeleport();
    new MallTeleport_conf();
}
