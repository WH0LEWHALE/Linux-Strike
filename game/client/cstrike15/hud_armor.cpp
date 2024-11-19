//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
 //====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "hudelement.h"
#include <vgui_controls/Panel.h>
#include <vgui/ISurface.h>
#include "clientmode_csnormal.h"
//#include "cs_gamerules.h"
#include "hud_numericdisplay.h"


class CHudArmor : public CHudElement, public CHudNumericDisplay
{
public:
	DECLARE_CLASS_SIMPLE( CHudArmor, CHudNumericDisplay );

	CHudArmor( const char *name );

    //virtual bool ShouldDraw();
	virtual void Paint();
    virtual void OnThink();
	virtual void Init();
	virtual void ApplySchemeSettings( IScheme *scheme );

private:
	CHudTexture *m_pArmorIcon;
	CHudTexture *m_pArmor_HelmetIcon;

	CPanelAnimationVarAliasType( float, icon_xpos, "icon_xpos", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_ypos, "icon_ypos", "2", "proportional_float" );
    CPanelAnimationVar( Color, m_TextColor, "TextColor", "Panel.FgColor" );
	float icon_wide;
	float icon_tall;
};


DECLARE_HUDELEMENT( CHudArmor );


CHudArmor::CHudArmor( const char *pName ) : CHudNumericDisplay( NULL, "HudArmor" ), CHudElement( pName ), m_pArmorIcon( NULL )
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
}


void CHudArmor::Init()
{
	SetIndent(true);
}

void CHudArmor::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	if( !m_pArmorIcon )
	{
		m_pArmorIcon = HudIcons().GetIcon( "shield_bright" );
	}

	if( !m_pArmor_HelmetIcon )
	{
		m_pArmor_HelmetIcon = HudIcons().GetIcon( "shield_kevlar_bright" );
	}	

	if( m_pArmorIcon )
	{
		icon_tall = GetTall() - YRES(2);
		float scale = icon_tall / (float)m_pArmorIcon->Height();
		icon_wide = ( scale ) * (float)m_pArmorIcon->Width();
	}
	SetFgColor( m_TextColor );
}

/*
bool CHudArmor::ShouldDraw()
{
    //necessary?
    C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();
    if ( pPlayer )
    {
        if(pPlayer->IsAlive() || pPlayer->GetObserverMode() == OBS_MODE_IN_EYE )
            return true;
        //return !pPlayer->IsObserver();
    }

    return false;
}
*/

void CHudArmor::OnThink()//Paint()
{
	// Update the time.
	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();
    if(!pPlayer)
        return;
    if ( pPlayer )
	{
        if( pPlayer->GetObserverMode() && pPlayer->GetObserverTarget())
        {
            pPlayer = ToCSPlayer( pPlayer->GetObserverTarget() );
            if(!pPlayer)
                return;
        }
        SetFgColor((int)pPlayer->ArmorValue() > 25 ? m_TextColor : COLOR_RED );
		SetDisplayValue((int)pPlayer->ArmorValue() );

	}
}

void CHudArmor::Paint( void )
{
    C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();
    if( !pPlayer )
        return;
    if( pPlayer->GetObserverMode() && pPlayer->GetObserverTarget())
    {
        pPlayer = ToCSPlayer( pPlayer->GetObserverTarget() );
        if( !pPlayer )
            return;
    }

    if( pPlayer->HasHelmet() && (int)pPlayer->ArmorValue() > 0 )
    {
        if( m_pArmor_HelmetIcon )
        {
            m_pArmor_HelmetIcon->DrawSelf( icon_xpos, icon_ypos, icon_wide, icon_tall, GetFgColor() );
        }
    }
    else
    {
        if( m_pArmorIcon )
        {
            m_pArmorIcon->DrawSelf( icon_xpos, icon_ypos, icon_wide, icon_tall, GetFgColor() );
        }
    }

    //draw the health icon
    BaseClass::Paint();
}
