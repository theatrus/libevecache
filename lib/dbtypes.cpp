#include "evecache/dbtypes.hpp"

namespace EveCache {

    unsigned long windows_to_unix_time(unsigned long long time)
    {
        return ((time / 10000000LL)-11644473600LL);
    }

    std::map<std::string, unsigned char> ColumnLookup::_idLookup;
    std::map<unsigned char, std::string> ColumnLookup::_nameLookup;
    bool ColumnLookup::isInit = false;

    void ColumnLookup::insert(const char* name, unsigned char id)
    {
        _idLookup[name] = id;
        _nameLookup[id] = name;
    }

    void ColumnLookup::init()
    {
        if (ColumnLookup::isInit == true)
            return;
        insert("*corpid",  1 );
        insert("*locationid",  2 );
        insert("age",  3 );
        insert("Asteroid",  4 );
        insert("authentication",  5 );
        insert("ballID",  6 );
        insert("beyonce",  7 );
        insert("bloodlineID",  8 );
        insert("capacity",  9 );
        insert("categoryID",  10 );
        insert("character",  11 );
        insert("characterID",  12 );
        insert("characterName",  13 );
        insert("characterType",  14 );
        insert("charID",  15 );
        insert("chatx",  16 );
        insert("clientID",  17 );
        insert("config",  18 );
        insert("contraband",  19 );
        insert("corporationDateTime",  20 );
        insert("corporationID",  21 );
        insert("createDateTime",  22 );
        insert("customInfo",  23 );
        insert("description",  24 );
        insert("divisionID",  25 );
        insert("DoDestinyUpdate",  26 );
        insert("dogmaIM",  27 );
        insert("EVE System",  28 );
        insert("flag",  29 );
        insert("foo.SlimItem",  30 );
        insert("gangID",  31 );
        insert("Gemini",  32 );
        insert("gender",  33 );
        insert("graphicID",  34 );
        insert("groupID",  35 );
        insert("header",  36 );
        insert("idName",  37 );
        insert("invbroker",  38 );
        insert("itemID",  39 );
        insert("items",  40 );
        insert("jumps",  41 );
        insert("line",  42 );
        insert("lines",  43 );
        insert("locationID",  44 );
        insert("locationName",  45 );
        insert("macho.CallReq",  46 );
        insert("macho.CallRsp",  47 );
        insert("macho.MachoAddress",  48 );
        insert("macho.Notification",  49 );
        insert("macho.SessionChangeNotification",  50 );
        insert("modules",  51 );
        insert("name",  52 );
        insert("objectCaching",  53 );
        insert("objectCaching.CachedObject",  54 );
        insert("OnChatJoin",  55 );
        insert("OnChatLeave",  56 );
        insert("OnChatSpeak",  57 );
        insert("OnGodmaShipEffect",  58 );
        insert("OnItemChange",  59 );
        insert("OnModuleAttributeChange",  60 );
        insert("OnMultiEvent",  61 );
        insert("orbitID",  62 );
        insert("ownerID",  63 );
        insert("ownerName",  64 );
        insert("quantity",  65 );
        insert("raceID",  66 );
        insert("RowClass",  67 );
        insert("securityStatus",  68 );
        insert("Sentry Gun",  69 );
        insert("sessionchange",  70 );
        insert("singleton",  71 );
        insert("skillEffect",  72 );
        insert("squadronID",  73 );
        insert("typeID",  74 );
        insert("used",  75 );
        insert("userID",  76 );
        insert("util.CachedObject",  77 );
        insert("util.IndexRowset",  78 );
        insert("util.Moniker",  79 );
        insert("util.Row",  80 );
        insert("util.Rowset",  81 );
        insert("*multicastID",  82 );
        insert("AddBalls",  83 );
        insert("AttackHit3",  84 );
        insert("AttackHit3R",  85 );
        insert("AttackHit4R",  86 );
        insert("DoDestinyUpdates",  87 );
        insert("GetLocationsEx",  88 );
        insert("InvalidateCachedObjects",  89 );
        insert("JoinChannel",  90 );
        insert("LSC",  91 );
        insert("LaunchMissile",  92 );
        insert("LeaveChannel",  93 );
        insert("OID+",  94 );
        insert("OID-",  95 );
        insert("OnAggressionChange",  96 );
        insert("OnCharGangChange",  97 );
        insert("OnCharNoLongerInStation",  98 );
        insert("OnCharNowInStation",  99 );
        insert("OnDamageMessage",  100 );
        insert("OnDamageStateChange",  101 );
        insert("OnEffectHit",  102 );
        insert("OnGangDamageStateChange",  103 );
        insert("OnLSC",  104 );
        insert("OnSpecialFX",  105 );
        insert("OnTarget",  106 );
        insert("RemoveBalls",  107 );
        insert("SendMessage",  108 );
        insert("SetMaxSpeed",  109 );
        insert("SetSpeedFraction",  110 );
        insert("TerminalExplosion",  111 );
        insert("address",  112 );
        insert("alert",  113 );
        insert("allianceID",  114 );
        insert("allianceid",  115 );
        insert("bid",  116 );
        insert("bookmark",  117 );
        insert("bounty",  118 );
        insert("channel",  119 );
        insert("charid",  120 );
        insert("constellationid",  121 );
        insert("corpID",  122 );
        insert("corpid",  123 );
        insert("corprole",  124 );
        insert("damage",  125 );
        insert("duration",  126 );
        insert("effects.Laser",  127 );
        insert("gangid",  128 );
        insert("gangrole",  129 );
        insert("hqID",  130 );
        insert("issued",  131 );
        insert("jit",  132 );
        insert("languageID",  133 );
        insert("locationid",  134 );
        insert("machoVersion",  135 );
        insert("marketProxy",  136 );
        insert("minVolume",  137 );
        insert("orderID",  138 );
        insert("price",  139 );
        insert("range",  140 );
        insert("regionID",  141 );
        insert("regionid",  142 );
        insert("role",  143 );
        insert("rolesAtAll",  144 );
        insert("rolesAtBase",  145 );
        insert("rolesAtHQ",  146 );
        insert("rolesAtOther",  147 );
        insert("shipid",  148 );
        insert("sn",  149 );
        insert("solarSystemID",  150 );
        insert("solarsystemid",  151 );
        insert("solarsystemid2",  152 );
        insert("source",  153 );
        insert("splash",  154 );
        insert("stationID",  155 );
        insert("stationid",  156 );
        insert("target",  157 );
        insert("userType",  158 );
        insert("userid",  159 );
        insert("volEntered",  160 );
        insert("volRemaining",  161 );
        insert("weapon",  162 );
        insert("agent.missionTemplatizedContent_BasicKillMission",  163 );
        insert("agent.missionTemplatizedContent_ResearchKillMission",  164 );
        insert("agent.missionTemplatizedContent_StorylineKillMission",  165 );
        insert("agent.missionTemplatizedContent_GenericStorylineKillMission",  166 );
        insert("agent.missionTemplatizedContent_BasicCourierMission",  167 );
        insert("agent.missionTemplatizedContent_ResearchCourierMission",  168 );
        insert("agent.missionTemplatizedContent_StorylineCourierMission",  169 );
        insert("agent.missionTemplatizedContent_GenericStorylineCourierMission",  170 );
        insert("agent.missionTemplatizedContent_BasicTradeMission",  171 );
        insert("agent.missionTemplatizedContent_ResearchTradeMission",  172 );
        insert("agent.missionTemplatizedContent_StorylineTradeMission",  173 );
        insert("agent.missionTemplatizedContent_GenericStorylineTradeMission",  174 );
        insert("agent.offerTemplatizedContent_BasicExchangeOffer",  175 );
        insert("agent.offerTemplatizedContent_BasicExchangeOffer_ContrabandDemand",  176 );
        insert("agent.offerTemplatizedContent_BasicExchangeOffer_Crafting",  177 );
        insert("agent.LoyaltyPoints",  178 );
        insert("agent.ResearchPoints",  179 );
        insert("agent.Credits",  180 );
        insert("agent.Item",  181 );
        insert("agent.Entity",  182 );
        insert("agent.Objective",  183 );
        insert("agent.FetchObjective",  184 );
        insert("agent.EncounterObjective",  185 );
        insert("agent.DungeonObjective",  186 );
        insert("agent.TransportObjective",  187 );
        insert("agent.Reward",  188 );
        insert("agent.TimeBonusReward",  189 );
        insert("agent.MissionReferral",  190 );
        insert("agent.Location",  191 );
        insert("agent.StandardMissionDetails",  192 );
        insert("agent.OfferDetails",  193 );
        insert("agent.ResearchMissionDetails",  194 );
        insert("agent.StorylineMissionDetails",  195 );
        insert("bad allocation",  196 );

        ColumnLookup::isInit = true;
    }


    unsigned char ColumnLookup::lookupId(std::string& name)
    {
        init();
        return _idLookup[name];
    }

    std::string ColumnLookup::lookupName(unsigned char id)
    {
        init();
        return _nameLookup[id];
    }

};
