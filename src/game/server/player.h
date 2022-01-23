/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_PLAYER_H
#define GAME_SERVER_PLAYER_H

// this include should perhaps be removed
#include "entities/character.h"
#include "gamecontext.h"

// player object
class CPlayer : protected CLuaClass
{
	MACRO_ALLOC_POOL_ID()
	friend class CPlayerDummy;

public:
	CPlayer(CGameContext *pGameServer, int ClientID, int Team, bool IsBot = false);
	~CPlayer();

	void Init(int CID);

	int m_MapMenuItem;
	int GetClass();
	void SetClassSkin(int newClass, int State = 0);
	void SetClass(int newClass);

	int MapMenu() { return (m_Team != TEAM_SPECTATORS) ? m_MapMenu : 0; };
	void OpenMapMenu(int Menu);
	void CloseMapMenu();
	bool MapMenuClickable();

	void TryRespawn();
	void Respawn();
	void SetTeam(int Team, bool DoChatMsg=true);
	int GetTeam() const { return m_Team; };
	int GetCID() const { return m_ClientID; };

	void Tick();
	void PostTick();
	void Snap(int SnappingClient);
	void FakeSnap();

	void OnDirectInput(const CNetObj_PlayerInput *NewInput);
	void OnPredictedInput(const CNetObj_PlayerInput *NewInput);
	void OnDisconnect(const char *pReason);

	void KillCharacter(int Weapon = WEAPON_GAME);
	CCharacter *GetCharacter();
	bool IsBot() const;


	bool AddClientInfoSnap(const char *pName, const char *pClanName, int Country, const char *pSkinName, bool UseCustomColor, int ColorBody, int ColorFeet);
	bool AddPlayerInfoSnap(int Score, int Team, int Latency, bool Local);
	bool AddSpectatorInfoSnap(int SpectatorID, int X, int Y);

	//---------------------------------------------------------
	// this is used for snapping so we know how we can clip the view for the player
	vec2 m_ViewPos;

	// states if the client is chatting, accessing a menu etc.
	int m_PlayerFlags;

	// used for snapping to just update latency if the scoreboard is active
	int m_aActLatency[MAX_CLIENTS];
	int GetActLatency(int CID)
	{
		if(CID >= 0 && CID < MAX_CLIENTS)
			return m_aActLatency[CID];
		return 0;
	}

	// used for spectator mode
	int m_SpectatorID;

	bool m_IsReady;

	//
	int m_Vote;
	int m_VotePos;
	//
	int m_LastVoteCall;
	int m_LastVoteTry;
	int m_LastChat;
	int m_LastSetTeam;
	int m_LastSetSpectatorMode;
	int m_LastChangeInfo;
	int m_LastEmote;
	int m_LastKill;

	// TODO: clean this up
	struct CTeeInfos
	{
		char m_aSkinName[64];
		int m_UseCustomColor;
		int m_ColorBody;
		int m_ColorFeet;

		// for lua
		const char *GetSkinName() const { return m_aSkinName; }
		void SetSkinName(const char *pSkinName) { str_copyb(m_aSkinName, pSkinName); }
	} m_TeeInfos;

	int m_RespawnTick;
	int m_DieTick;
	int m_Score;
	int m_ScoreStartTick;
	bool m_ForceBalanced;
	int m_LastActionTick;
	int m_TeamChangeTick;
	struct
	{
		int m_TargetX;
		int m_TargetY;
	} m_LatestActivity;

	// network latency calculations
	struct
	{
		int m_Accum;
		int m_AccumMin;
		int m_AccumMax;
		int m_Avg;
		int m_Min;
		int m_Max;
	} m_Latency;

private:
	CCharacter *m_pCharacter;
	CGameContext *m_pGameServer;

	int m_Class;
	int m_MapMenu;
	int m_MapMenuTick;
	//
	bool m_Spawning;
	int m_ClientID;
	int m_Team;

protected:
	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const;
};

enum
{
	PLAYERITER_ALL=0x0,
	
	PLAYERITER_COND_READY=0x1,
	PLAYERITER_COND_SPEC=0x2,
	PLAYERITER_COND_NOSPEC=0x4,
	
	PLAYERITER_INGAME = PLAYERITER_COND_READY | PLAYERITER_COND_NOSPEC,
	PLAYERITER_SPECTATORS = PLAYERITER_COND_READY | PLAYERITER_COND_SPEC,
};

template<int FLAGS>
class CPlayerIterator
{
private:
	CPlayer** m_ppPlayers;
	int m_ClientID;
	
public:
	
	CPlayerIterator(CPlayer** ppPlayers) :
		m_ppPlayers(ppPlayers)
	{
		Reset();
	}
	
	inline bool Next()
	{
		for(m_ClientID = m_ClientID+1; m_ClientID<MAX_CLIENTS; m_ClientID++)
		{
			CPlayer* pPlayer = Player();
			
			if(!pPlayer) continue;
			if((FLAGS & PLAYERITER_COND_READY) && (!pPlayer->m_IsInGame)) continue;
			if((FLAGS & PLAYERITER_COND_NOSPEC) && (pPlayer->GetTeam() == TEAM_SPECTATORS)) continue;
			if((FLAGS & PLAYERITER_COND_SPEC) && (pPlayer->GetTeam() != TEAM_SPECTATORS)) continue;
			
			return true;
		}
		
		return false;
	}
	
	inline void Reset() { m_ClientID = -1; }
	
	inline CPlayer* Player() { return m_ppPlayers[m_ClientID]; }
	inline int ClientID() { return m_ClientID; }
};

#endif
