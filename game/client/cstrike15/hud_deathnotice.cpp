//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Draws CSPort's death notices
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "c_playerresource.h"
#include "iclientmode.h"
#include <vgui_controls/Controls.h>
#include <vgui_controls/Panel.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <keyvalues.h>
#include "c_baseplayer.h"
#include "c_team.h"

#include "cs_shareddefs.h"
#include "clientmode_csnormal.h"
#include "c_cs_player.h"
#include "c_cs_playerresource.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const int DOMINATION_DRAW_HEIGHT = 20;
const int DOMINATION_DRAW_WIDTH = 20;

static ConVar hud_deathnotice_time( "hud_deathnotice_time", "6", 0 );
static ConVar hud_deathnotice_count( "hud_deathnotice_count", "10", 0 );
//extern ConVar mp_display_kill_assists( "mp_display_kill_assists", "1", 0 );
extern ConVar mp_display_kill_assists;
// Player entries in a death notice
struct DeathNoticePlayer
{
	char		szName[MAX_PLAYER_NAME_LENGTH];
	char		szClan[MAX_CLAN_TAG_LENGTH];
	int			iEntIndex;
	Color		color;
};

// Contents of each entry in our list of death notices
struct DeathNoticeItem 
{
	DeathNoticePlayer	Killer;
	DeathNoticePlayer   	Victim;
	DeathNoticePlayer   	Assister;
	
	CHudTexture *iconDeath;
	int			iSuicide;
	int			iAssistKill;
	float		flDisplayTime;
	bool		bHeadshot;
	bool		bWallshot;
	int			iDominationImageId;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudDeathNotice : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudDeathNotice, vgui::Panel );
public:
	CHudDeathNotice( const char *pElementName );

	void Init( void );
	void VidInit( void );
	virtual bool ShouldDraw( void );
	virtual void Paint( void );
	virtual void ApplySchemeSettings( vgui::IScheme *scheme );

	void RetireExpiredDeathNotices( void );
	
	void FireGameEvent( IGameEvent *event );

protected:
	int SetupHudImageId( const char* fname );

private:

    CPanelAnimationVarAliasType( float, m_flLineHeight, "LineHeight", "13", "proportional_float" );

    CPanelAnimationVar( float, m_flMaxDeathNotices, "MaxDeathNotices", "15" );

	CPanelAnimationVar( bool, m_bRightJustify, "RightJustify", "1" );

    CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "HudNumbersTimer" );

    CPanelAnimationVar( Color, m_clrCTText, "CTTextColor", "CTTextColor" );
    CPanelAnimationVar( Color, m_clrTerroristText, "TerroristTextColor", "TerroristTextColor" );

	// Texture for skull symbol
	CHudTexture		*m_iconD_skull;  
	CHudTexture		*m_iconD_headshot;  
	CHudTexture		*m_iconD_wallshot; 
	int				m_iNemesisImageId;
	int				m_iDominatedImageId;
	int				m_iRevengeImageId;

	Color			m_teamColors[TEAM_MAXCOUNT];

	CUtlVector<DeathNoticeItem> m_DeathNotices;
};

using namespace vgui;

DECLARE_HUDELEMENT( CHudDeathNotice );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHudDeathNotice::CHudDeathNotice( const char *pElementName ) :
	CHudElement( pElementName ), BaseClass( NULL, "HudDeathNotice" )
{
	vgui::Panel *pParent = GetClientMode()->GetViewport();
	SetParent( pParent );
	

	m_iconD_headshot = NULL;
	m_iconD_skull = NULL;
	m_iconD_wallshot = NULL;

	SetHiddenBits( HIDEHUD_MISCSTATUS );

	m_iNemesisImageId = SetupHudImageId("hud/freeze_nemesis");
	m_iDominatedImageId = SetupHudImageId("hud/freeze_dominated");
	m_iRevengeImageId = SetupHudImageId("hud/freeze_revenge");
}


/**
 * Helper function to get an image id and set 
 */
int CHudDeathNotice::SetupHudImageId( const char* fname )
{
	int imageId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile( imageId, fname, true, false );
	return imageId;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
	SetPaintBackgroundEnabled( false );

	// make team color lookups easier
	memset(m_teamColors, 0, sizeof(m_teamColors));
	m_teamColors[TEAM_CT] = m_clrCTText;
	m_teamColors[TEAM_TERRORIST] = m_clrTerroristText;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::Init( void )
{
	ListenForGameEvent( "player_death" );	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::VidInit( void )
{
	m_iconD_skull = HudIcons().GetIcon( "d_skull_cs" );
   	m_iconD_headshot = HudIcons().GetIcon( "d_headshot" );
   	m_iconD_wallshot = HudIcons().GetIcon( "ammo_12g" );
	m_DeathNotices.Purge();
}

//-----------------------------------------------------------------------------
// Purpose: Draw if we've got at least one death notice in the queue
//-----------------------------------------------------------------------------
bool CHudDeathNotice::ShouldDraw( void )
{
	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();

	if ( !pPlayer )
		return false;

	// don't show death notices when flashed
	if ( pPlayer->IsAlive() && pPlayer->m_flFlashBangTime >= gpGlobals->curtime )
	{
		float flAlpha = pPlayer->m_flFlashMaxAlpha * (pPlayer->m_flFlashBangTime - gpGlobals->curtime) / pPlayer->m_flFlashDuration;
		if ( flAlpha > 75.0f ) // 0..255
		{
			return false;
		}
	}

	return ( CHudElement::ShouldDraw() && ( m_DeathNotices.Count() ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudDeathNotice::Paint()
{
	if ( !m_iconD_wallshot || !m_iconD_headshot || !m_iconD_skull )
		return;

	int yStart = GetClientModeCSNormal()->GetDeathMessageStartHeight();

	surface()->DrawSetTextFont( m_hTextFont );
	surface()->DrawSetTextColor( m_clrCTText );

	int dominationDrawWidth = scheme()->GetProportionalScaledValueEx( GetScheme(), DOMINATION_DRAW_WIDTH );
	int dominationDrawHeight = scheme()->GetProportionalScaledValueEx( GetScheme(), DOMINATION_DRAW_HEIGHT );

	int iconHeadshotWide;
	int iconHeadshotTall;
	int iconWallshotWide;
	int iconWallshotTall;

	if( m_iconD_headshot->bRenderUsingFont )
	{
		iconHeadshotWide = surface()->GetCharacterWidth( m_iconD_headshot->hFont, m_iconD_headshot->cCharacterInFont );
		iconHeadshotTall = surface()->GetFontTall( m_iconD_headshot->hFont );
	}
	else
	{
		float scale = ( (float)ScreenHeight() / 480.0f );	//scale based on 640x480
		iconHeadshotWide = (int)( scale * (float)m_iconD_headshot->Width() );
		iconHeadshotTall = (int)( scale * (float)m_iconD_headshot->Height() );
	}
	if( m_iconD_wallshot->bRenderUsingFont )
	{
		iconWallshotWide = surface()->GetCharacterWidth( m_iconD_wallshot->hFont, m_iconD_wallshot->cCharacterInFont );
		iconWallshotTall = surface()->GetFontTall( m_iconD_wallshot->hFont );
	}
	else
	{
		float scale = ( (float)ScreenHeight() / 480.0f );	//scale based on 640x480
		iconWallshotWide = (int)( scale * (float)m_iconD_wallshot->Width() );
		iconWallshotTall = (int)( scale * (float)m_iconD_wallshot->Height() );
	}

	int iCount = m_DeathNotices.Count();
	for ( int i = 0; i < iCount; i++ )
	{
		CHudTexture *icon = m_DeathNotices[i].iconDeath;
		if ( !icon )
			continue;

		wchar_t victim[ 256 ];
		wchar_t assist[ 256 ];
		wchar_t assistplus[ 10 ];
		wchar_t killer[ 256 ];
		wchar_t victimclan[ 256 ];
		wchar_t killerclan[ 256 ];

		g_pVGuiLocalize->ConvertANSIToUnicode( m_DeathNotices[i].Victim.szName, victim, sizeof( victim ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( m_DeathNotices[i].Killer.szName, killer, sizeof( killer ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( " + ", assistplus, sizeof( assistplus ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( m_DeathNotices[i].Assister.szName, assist, sizeof( assist ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( m_DeathNotices[i].Victim.szClan, victimclan, sizeof( victimclan ) );
		g_pVGuiLocalize->ConvertANSIToUnicode( m_DeathNotices[i].Killer.szClan, killerclan, sizeof( killerclan ) );

		// Get the local position for this notice
		int victimNameLen = UTIL_ComputeStringWidth( m_hTextFont, victim );
		int victimClanLen = UTIL_ComputeStringWidth( m_hTextFont, victimclan );
		int assistNameLen = UTIL_ComputeStringWidth( m_hTextFont, assist );
		int assistplusLen = UTIL_ComputeStringWidth( m_hTextFont, assistplus );
		int y = yStart + (m_flLineHeight * i);

		int iconWide;
		int iconTall;

		if( icon->bRenderUsingFont )
		{
			iconWide = surface()->GetCharacterWidth( icon->hFont, icon->cCharacterInFont );
			iconTall = surface()->GetFontTall( icon->hFont );
		}
		else
		{
			float scale = ( (float)ScreenHeight() / 480.0f );	//scale based on 640x480
			iconWide = (int)( scale * (float)icon->Width() );
			iconTall = (int)( scale * (float)icon->Height() );
		}

		int x = 0;
		if ( m_bRightJustify )
		{
			x =	GetWide();
			x -= victimNameLen;
			x -= victimClanLen;
			x -= iconWide;

			if ( m_DeathNotices[i].bHeadshot )
				x -= iconHeadshotWide;
			if ( m_DeathNotices[i].bWallshot )
				x -= iconWallshotWide;

			if ( !m_DeathNotices[i].iSuicide )
			{
				if(m_DeathNotices[i].iAssistKill)
				{
					x -= assistNameLen ;
					x -= assistplusLen ;
				}
				//x -= iconWide;
				x -= UTIL_ComputeStringWidth( m_hTextFont, killer );
				x -= UTIL_ComputeStringWidth( m_hTextFont, killerclan );
			}

			if (m_DeathNotices[i].iDominationImageId >= 0)
			{				
				x -= dominationDrawWidth;
			}
		}

		if (m_DeathNotices[i].iDominationImageId >= 0)
		{			
			surface()->DrawSetTexture(m_DeathNotices[i].iDominationImageId);
			surface()->DrawSetColor(m_DeathNotices[i].Killer.color);
			surface()->DrawTexturedRect( x, y, x + dominationDrawWidth, y + dominationDrawHeight );
			x += dominationDrawWidth;
		}
		
		// Only draw killers name if it wasn't a suicide
		if ( !m_DeathNotices[i].iSuicide )
		{
			// Draw killer's clan
			surface()->DrawSetTextColor( m_DeathNotices[i].Killer.color );
			surface()->DrawSetTextPos( x, y );
			surface()->DrawSetTextFont( m_hTextFont );
			surface()->DrawUnicodeString( killerclan );
			surface()->DrawGetTextPos( x, y );

			// Draw killer's name
			surface()->DrawSetTextColor( m_DeathNotices[i].Killer.color );
			surface()->DrawSetTextPos( x, y );
			surface()->DrawSetTextFont( m_hTextFont );
			surface()->DrawUnicodeString( killer );
			surface()->DrawGetTextPos( x, y );

			if( m_DeathNotices[i].iAssistKill)
			{
				surface()->DrawSetTextColor( COLOR_WHITE );
				surface()->DrawSetTextPos( x, y );
				surface()->DrawSetTextFont( m_hTextFont );
				surface()->DrawUnicodeString( assistplus );
				surface()->DrawGetTextPos( x, y );

				// Draw assistant's name
				surface()->DrawSetTextColor( m_DeathNotices[i].Assister.color );
				surface()->DrawSetTextPos( x, y );
				surface()->DrawSetTextFont( m_hTextFont );
				surface()->DrawUnicodeString( assist );
				surface()->DrawGetTextPos( x, y );
			}	
		}


		// Draw death weapon
		//If we're using a font char, this will ignore iconTall and iconWide
		//Color iconColor( 255, 80, 0, 255 );
		Color iconColor( 255, 120, 60, 255 );
		icon->DrawSelf( x, y, iconWide , iconTall , iconColor );
		x += iconWide;		

		if( m_DeathNotices[i].bHeadshot )
		{
			m_iconD_headshot->DrawSelf( x, y, iconHeadshotWide, iconHeadshotTall, iconColor );
			x += iconHeadshotWide;
		}
		if( m_DeathNotices[i].bWallshot )
		{
			m_iconD_wallshot->DrawSelf( x, y, iconWallshotWide, iconWallshotTall, iconColor );
			x += iconWallshotWide;
		}

		// Draw victims clan
		surface()->DrawSetTextColor( m_DeathNotices[i].Victim.color );
		surface()->DrawSetTextPos( x, y );
		surface()->DrawSetTextFont( m_hTextFont );	//reset the font, draw icon can change it
		surface()->DrawUnicodeString( victimclan );
		surface()->DrawGetTextPos( x, y );

		// Draw victims name
		surface()->DrawSetTextColor( m_DeathNotices[i].Victim.color );
		surface()->DrawSetTextPos( x, y );
		surface()->DrawSetTextFont( m_hTextFont );	//reset the font, draw icon can change it
		surface()->DrawUnicodeString( victim );
	}

	// Now retire any death notices that have expired
	RetireExpiredDeathNotices();
}

//-----------------------------------------------------------------------------
// Purpose: This message handler may be better off elsewhere
//-----------------------------------------------------------------------------
void CHudDeathNotice::RetireExpiredDeathNotices( void )
{
	// Loop backwards because we might remove one

    int iSize = m_DeathNotices.Count();
    for ( int i = iSize-1; i >= 0; i-- )
    {
		if ( m_DeathNotices[i].flDisplayTime < gpGlobals->curtime )
		{
			m_DeathNotices.Remove(i);
		}
    }
}

//-----------------------------------------------------------------------------
// Purpose: Server's told us that someone's died
//-----------------------------------------------------------------------------
void CHudDeathNotice::FireGameEvent( IGameEvent *event )
{
	if (!g_PR)
		return;

	C_CS_PlayerResource *cs_PR = dynamic_cast<C_CS_PlayerResource *>( g_PR );
	if ( !cs_PR )
		return;

	if ( hud_deathnotice_time.GetFloat() == 0 )
		return;

	// the event should be "player_death"
	
	int iKiller = engine->GetPlayerForUserID( event->GetInt("attacker") );
	int iVictim = engine->GetPlayerForUserID( event->GetInt("userid") );
	int iAssister = mp_display_kill_assists.GetBool() ? engine->GetPlayerForUserID( event->GetInt( "assister" ) ) : 0;

	const char *killedwith = event->GetString( "weapon" );
	bool headshot = event->GetInt( "headshot" ) > 0;
	bool wallshot = event->GetInt( "penetrated" ) > 0;

	char fullkilledwith[128];
	if ( killedwith && *killedwith )
	{
		Q_snprintf( fullkilledwith, sizeof(fullkilledwith), "d_%s", killedwith );
	}
	else
	{
		fullkilledwith[0] = 0;
	}

	// Do we have too many death messages in the queue?
	if ( m_DeathNotices.Count() > 0 && m_DeathNotices.Count() >= hud_deathnotice_count.GetInt() )
	{
		// Remove the oldest one in the queue, which will always be the first
		m_DeathNotices.Remove(0);
	}

	// Get the names of the players
	const char *killer_name = iKiller > 0 ? g_PR->GetPlayerName( iKiller ) : NULL;
	const char *victim_name = iVictim > 0 ? g_PR->GetPlayerName( iVictim ) : NULL;
	const char *assister_name = iAssister > 0 ? g_PR->GetPlayerName( iAssister ) : NULL;
	
	if ( !killer_name )
		killer_name = "";
	if ( !victim_name )
		victim_name = "";
	if ( !assister_name )
		assister_name = "";

	// Get the clan tags of the players
	const char *killer_clan = iKiller > 0 ? cs_PR->GetClanTag( iKiller ) : NULL;
	const char *victim_clan = iVictim > 0 ? cs_PR->GetClanTag( iVictim ) : NULL;

	if ( !killer_clan )
		killer_clan = "";
	if ( !victim_clan )
		victim_clan = "";

	// Make a new death notice
	DeathNoticeItem deathMsg;
	deathMsg.Killer.iEntIndex = iKiller;
	deathMsg.Victim.iEntIndex = iVictim;
	deathMsg.Assister.iEntIndex = iAssister;

	deathMsg.Killer.color = iKiller > 0 ? m_teamColors[g_PR->GetTeam(iKiller)] : COLOR_WHITE;
	deathMsg.Assister.color = iAssister > 0 ? m_teamColors[g_PR->GetTeam(iAssister)] : COLOR_WHITE;
	deathMsg.Victim.color = iVictim > 0 ? m_teamColors[g_PR->GetTeam(iVictim)] : COLOR_WHITE;

	Q_snprintf( deathMsg.Killer.szClan, sizeof( deathMsg.Killer.szClan ), "%s ", killer_clan );
	Q_snprintf( deathMsg.Victim.szClan, sizeof( deathMsg.Victim.szClan ), "%s ", victim_clan );

	Q_strncpy( deathMsg.Killer.szName, killer_name, MAX_PLAYER_NAME_LENGTH );
	Q_strncpy( deathMsg.Assister.szName, assister_name, MAX_PLAYER_NAME_LENGTH );
	Q_strncpy( deathMsg.Victim.szName, victim_name, MAX_PLAYER_NAME_LENGTH );

	deathMsg.flDisplayTime = gpGlobals->curtime + hud_deathnotice_time.GetFloat();
	deathMsg.iSuicide = ( !iKiller || iKiller == iVictim );
	deathMsg.iAssistKill = ( iAssister > 0 ); // || iKiller == iVictim 
	deathMsg.bHeadshot = headshot;
	deathMsg.bWallshot = wallshot;
	deathMsg.iDominationImageId = -1;

	CCSPlayer* pKiller = ToCSPlayer(ClientEntityList().GetBaseEntity(iKiller));

	// the local player is dead, see if this is a new nemesis or a revenge
	if ( event->GetInt( "dominated" ) > 0 || (pKiller != NULL && pKiller->IsPlayerDominated(iVictim)) )
	{
		deathMsg.iDominationImageId = m_iDominatedImageId;
	}
	else if ( event->GetInt( "revenge" ) > 0 )
	{
		deathMsg.iDominationImageId = m_iRevengeImageId;
	}

	// Try and find the death identifier in the icon list
	deathMsg.iconDeath = HudIcons().GetIcon( fullkilledwith );

	if ( !deathMsg.iconDeath )
	{
		// Can't find it, so use the default skull & crossbones icon
		deathMsg.iconDeath = m_iconD_skull;
	}

	// Add it to our list of death notices
	m_DeathNotices.AddToTail( deathMsg );

	char sDeathMsg[512];

	// Record the death notice in the console
	if ( deathMsg.iSuicide )
	{
		if ( !strcmp( fullkilledwith, "d_planted_c4" ) )
		{
			Q_snprintf( sDeathMsg, sizeof( sDeathMsg ), "%s was a bit too close to the c4.\n", deathMsg.Victim.szName );
		}
		else if ( !strcmp( fullkilledwith, "d_worldspawn" ) )
		{
			Q_snprintf( sDeathMsg, sizeof( sDeathMsg ), "%s died.\n", deathMsg.Victim.szName );
		}
		else	//d_world
		{
			Q_snprintf( sDeathMsg, sizeof( sDeathMsg ), "%s suicided.\n", deathMsg.Victim.szName );
		}
	}
	else
	{
		Q_snprintf( sDeathMsg, sizeof( sDeathMsg ), "%s killed %s", deathMsg.Killer.szName, deathMsg.Victim.szName );

		if ( fullkilledwith && *fullkilledwith && (*fullkilledwith > 13 ) )
		{
			Q_strncat( sDeathMsg, VarArgs( " with %s.\n", fullkilledwith+2 ), sizeof( sDeathMsg ), COPY_ALL_CHARACTERS );
		}
	}

	Msg( "%s", sDeathMsg );
}



