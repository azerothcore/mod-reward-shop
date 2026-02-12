/*

Database Actions:

1 = item
2 = gold
3 = name change
4 = faction change
5 = race change
6 = level up to 80

script made by talamortis

*/

#include "AccountMgr.h"
#include "Chat.h"
#include "Configuration/Config.h"
#include "Creature.h"
#include "Define.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"

class reward_shop : public CreatureScript
{
public:
    reward_shop() : CreatureScript("reward_shop") {}

    bool failedcode;

    bool OnGossipHello(Player *player, Creature *creature) override
    {
        if (player->IsInCombat())
            return false;

        if (!sConfigMgr->GetOption<bool>("RewardShopEnable", 0))
            return false;

        std::string text = "Enter code and press accept";
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I'd like to redeem my code.", GOSSIP_SENDER_MAIN, 1, text, 0, true);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "How do i get a code?", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I dont have a code.", GOSSIP_SENDER_MAIN, 3);

        if (sConfigMgr->GetOption<bool>("AllowGM", 1) && player->IsGameMaster())
            AddGossipItemFor(player, GOSSIP_ICON_BATTLE, "[GM] I would like generate a code.", GOSSIP_SENDER_MAIN, 4);

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player *player, Creature *creature, uint32 /* sender */, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        std::string info = sConfigMgr->GetOption<std::string>("WebsiteAddress", "You can get codes by visiting the online store at (website address)");
        uint32 rnd1 = urand(10000, 90000);
        uint32 rnd2 = urand(10000, 90000);
        uint32 rnd3 = urand(10000, 90000);
        uint32 rnd4 = urand(10000, 90000);
        uint32 rnd5 = urand(10000, 90000);

        std::string CreatedBy = player->GetName();
        std::ostringstream randomcode;
        randomcode << "GM-" << rnd1 << "-" << rnd2 << "-" << rnd3 << "-" << rnd4 << "-" << rnd5;

        switch (action)
        {
        case 2:
            creature->Whisper(info.c_str(), LANG_UNIVERSAL, player);
            CloseGossipMenuFor(player);
            break;
        case 3:
            CloseGossipMenuFor(player);
            break;
        case 4:
            player->PlayerTalkClass->ClearMenus();
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I'd like to generate a name change code.", GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I'd like to generate a faction change code.", GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "I'd like to generate a race change code.", GOSSIP_SENDER_MAIN, 8);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            break;
        case 6:
            CharacterDatabase.Query("INSERT INTO `reward_shop` (`action`, `action_data`, `quantity`, `code`, `status`, `PlayerGUID`, `PlayerIP`, `CreatedBy`) VALUES(3, 0, 0, '{}', 0, 0, '0', '{}')", randomcode.str().c_str(), CreatedBy.c_str());
            ChatHandler(player->GetSession()).PSendSysMessage("Code was successfully created your code is {}", randomcode.str());
            break;
        case 7:
            CharacterDatabase.Query("INSERT INTO `reward_shop` (`action`, `action_data`, `quantity`, `code`, `status`, `PlayerGUID`, `PlayerIP`,`CreatedBy`) VALUES(4, 0, 0, '{}', 0, 0, '0', '{}')", randomcode.str().c_str(), CreatedBy.c_str());
            ChatHandler(player->GetSession()).PSendSysMessage("Code was successfully created your code is {}", randomcode.str());
            break;
        case 8:
            CharacterDatabase.Query("INSERT INTO `reward_shop` (`action`, `action_data`, `quantity`, `code`, `status`, `PlayerGUID`, `PlayerIP`, `CreatedBy`) VALUES(5, 0, 0, '{}', 0, 0, '0', '{}')", randomcode.str().c_str(), CreatedBy.c_str());
            ChatHandler(player->GetSession()).PSendSysMessage("Code was successfully created your code is {}", randomcode.str());
            break;
        }
        return true;
    }

    bool OnGossipSelectCode(Player *player, Creature *creature, uint32 /* sender */, uint32, const char *code) override
    {
        ObjectGuid playerguid = player->GetGUID();
        std::string playerIP = player->GetSession()->GetRemoteAddress();
        std::string rewardcode = code;
        std::ostringstream messageCode;
        messageCode << "Sorry " << player->GetName() << ", that is not a valid code or has already been redeemed.";

        std::size_t found = rewardcode.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-");

        if (found != std::string::npos)
            return false;

        // check for code
        QueryResult result = CharacterDatabase.Query("SELECT id, action, action_data, quantity, status FROM reward_shop WHERE code = '{}'", rewardcode.c_str());

        if (!result)
        {
            player->PlayDirectSound(9638); // No
            creature->Whisper(messageCode.str().c_str(), LANG_UNIVERSAL, player);
            creature->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            return false;
        }

        std::transform(rewardcode.begin(), rewardcode.end(), rewardcode.begin(), ::toupper);

        do
        {
            Field *fields = result->Fetch();
            uint32 action = fields[1].Get<uint32>();
            uint32 action_data = fields[2].Get<uint32>();
            uint32 quantity = fields[3].Get<uint32>();
            uint32 status = fields[4].Get<int32>();
            int count = 1;
            uint32 noSpaceForCount = 0;
            ItemPosCountVec dest;
            InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, action_data, quantity, &noSpaceForCount);

            if (status == 1)
            {
                player->PlayDirectSound(9638); // No
                creature->Whisper(messageCode.str().c_str(), LANG_UNIVERSAL, player);
                creature->HandleEmoteCommand(EMOTE_ONESHOT_QUESTION);
                SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                return false;
            }
            switch (action)
            {

            case 1: /* Item */
                if (msg != EQUIP_ERR_OK)
                    count -= noSpaceForCount;

                if (count == 0 || dest.empty())
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("Can not create item either item is unique or you do not have any space");
                    ChatHandler(player->GetSession()).SetSentErrorMessage(true);
                    return false;
                }

                if (count > 0 && action_data)
                    player->AddItem(action_data, quantity);
                break;
            case 2: /* Gold */
                player->ModifyMoney(action_data * 10000);
                ChatHandler(player->GetSession()).PSendSysMessage("CHAT OUTPUT: Successfully added [{} G]", action_data);
                break;
            case 3: /* Name Change */
                player->SetAtLoginFlag(AT_LOGIN_RENAME);
                ChatHandler(player->GetSession()).PSendSysMessage("CHAT OUTPUT: Please log out for name change.");
                break;
            case 4: /* Faction Change */
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                ChatHandler(player->GetSession()).PSendSysMessage("CHAT OUTPUT: Please log out for faction change.");
                break;
            case 5: /* Race Change */
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                ChatHandler(player->GetSession()).PSendSysMessage("CHAT OUTPUT: Please log out for race change.");
                break;
            case 6: /* Level Up */
                if (player->GetLevel() < 80) {
                    player->SetLevel(80);
                    player->SetUInt32Value(PLAYER_XP, 0);
                    player->SetUInt32Value(PLAYER_NEXT_LEVEL_XP, 0);
                    player->SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, 80);
                    player->UpdateSkillsToMaxSkillsForLevel();
                    ChatHandler(player->GetSession()).PSendSysMessage("CHAT OUTPUT: You´ve reached level 80.");
                }
                else
                    ChatHandler(player->GetSession()).PSendSysMessage("CHAT OUTPUT: You´re already level 80.");
                break;
            }

        } while (result->NextRow());

        CharacterDatabase.Query("UPDATE reward_shop SET status = 1, PlayerGUID = '{}', PlayerIP = '{}' WHERE code = '{}'", playerguid.GetCounter(), playerIP.c_str(), rewardcode.c_str());
        return true;
    }

    struct npc_reward_shopAI : public ScriptedAI
    {
        npc_reward_shopAI(Creature *creature) : ScriptedAI(creature) {}
        uint32 say_timer;
        bool canSay;

        void Reset() override
        {
            say_timer = 1000;
            canSay = false;
        }

        void MoveInLineOfSight(Unit *who) override
        {
            if (me->IsWithinDist(who, 5.0f) && who->GetTypeId() == TYPEID_PLAYER)
                canSay = true;
            else
                canSay = false;
        }

        void UpdateAI(uint32 diff) override
        {
            if (say_timer <= diff)
            {
                if (canSay)
                {
                    me->Say("Do you have a code to redeem? Step right up!", LANG_UNIVERSAL);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                    say_timer = 61000;
                }
            }
            else
                say_timer -= diff;
        }
    };
    CreatureAI *GetAI(Creature *creature) const override
    {
        return new npc_reward_shopAI(creature);
    }
};

void AddRewardShopScripts()
{
    new reward_shop();
}
