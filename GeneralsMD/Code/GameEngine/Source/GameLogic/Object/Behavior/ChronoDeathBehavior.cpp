/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: ChronoDeathBehavior.cpp ///////////////////////////////////////////////////////////////////////
// Author:
// Desc:  
///////////////////////////////////////////////////////////////////////////////////////////////////


// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine
#define DEFINE_SLOWDEATHPHASE_NAMES

#include "Common/Thing.h"
#include "Common/ThingTemplate.h"
#include "Common/INI.h"
#include "Common/Xfer.h"
#include "GameClient/Drawable.h"
#include "GameClient/FXList.h"
#include "GameClient/InGameUI.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Module/BodyModule.h"
#include "GameLogic/Module/ChronoDeathBehavior.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Object.h"
#include "GameLogic/ObjectCreationList.h"
#include "GameClient/Drawable.h"

//-------------------------------------------------------------------------------------------------
ChronoDeathBehaviorModuleData::ChronoDeathBehaviorModuleData()
{
	m_ocl = NULL;
	m_startFX = NULL;
	m_endFX = NULL;
	m_startScale = 1.0;
	m_endScale = 1.0;
	m_startAlpha = 1.0;
	m_endAlpha = 1.0;
	m_destructionDelay = 1;
}

//-------------------------------------------------------------------------------------------------
/*static*/ void ChronoDeathBehaviorModuleData::buildFieldParse(MultiIniFieldParse& p) 
{
	UpdateModuleData::buildFieldParse(p);

	static const FieldParse dataFieldParse[] = 
	{
		{ "StartFX",			INI::parseFXList,			    NULL, offsetof(ChronoDeathBehaviorModuleData, m_startFX) },
		{ "EndFX",				INI::parseFXList,			    NULL, offsetof(ChronoDeathBehaviorModuleData, m_endFX) },
		{ "OCL",				INI::parseObjectCreationList,	NULL, offsetof(ChronoDeathBehaviorModuleData, m_ocl) },
		{ "StartScale",			INI::parseReal,			        NULL, offsetof(ChronoDeathBehaviorModuleData, m_startScale) },
		{ "EndScale",			INI::parseReal,		            NULL, offsetof(ChronoDeathBehaviorModuleData, m_endScale) },
		{ "StartOpacity",			INI::parseReal,		            NULL, offsetof(ChronoDeathBehaviorModuleData, m_startAlpha) },
		{ "EndOpacity",			INI::parseReal,		            NULL, offsetof(ChronoDeathBehaviorModuleData, m_endAlpha) },
		{ "DestructionDelay",	INI::parseDurationUnsignedInt,	NULL, offsetof(ChronoDeathBehaviorModuleData, m_destructionDelay) },
		{ 0, 0, 0, 0 }
	};
    p.add(dataFieldParse);
    p.add(DieMuxData::getFieldParse(), offsetof(ChronoDeathBehaviorModuleData, m_dieMuxData));
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
ChronoDeathBehavior::ChronoDeathBehavior( Thing *thing, const ModuleData* moduleData ) : UpdateModule(thing, moduleData)
{
	m_destructionFrame = 0;
	m_dieFrame = 0;
	m_deathTriggered = FALSE;
	setWakeFrame(getObject(), UPDATE_SLEEP_FOREVER);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
ChronoDeathBehavior::~ChronoDeathBehavior( void )
{
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
UpdateSleepTime ChronoDeathBehavior::update()
{
	DEBUG_ASSERTCRASH(m_deathTriggered, ("hmm, this should not be possible"));

	const ChronoDeathBehaviorModuleData* d = getChronoDeathBehaviorModuleData();
	Object* obj = getObject();
	UnsignedInt now = TheGameLogic->getFrame();

	// Calculate current Alpha and Scale
	Real progress = INT_TO_REAL(now - m_dieFrame) / INT_TO_REAL(m_destructionFrame - m_dieFrame);

	Real scale = (1 - progress) * d->m_startScale + progress * d->m_endScale;
	Real opacity = (1 - progress) * d->m_startAlpha + progress * d->m_endAlpha;

	Drawable* draw = obj->getDrawable();

	// Make sure to include template scale
	draw->setInstanceScale(obj->getTemplate()->getAssetScale() * scale);
	draw->setDrawableOpacity(opacity);
	//draw->setEffectiveOpacity(opacity);
	//draw->setSecondMaterialPassOpacity(opacity);

	if (now >= m_destructionFrame)
	{
		if (d->m_endFX)
		{
			FXList::doFXObj(d->m_startFX, obj);
		}
		TheGameLogic->destroyObject(obj);
		return UPDATE_SLEEP_FOREVER;
	}

	return UPDATE_SLEEP_NONE;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void ChronoDeathBehavior::beginChronoDeath(const DamageInfo* damageInfo)
{
	if (m_deathTriggered)
		return;

	const ChronoDeathBehaviorModuleData* d = getChronoDeathBehaviorModuleData();
	Object* obj = getObject();

	// deselect this unit for all players.
	TheGameLogic->deselectObject(obj, PLAYERMASK_ALL, TRUE);

	Drawable* draw = obj->getDrawable();
	if (draw) {
		draw->setShadowsEnabled(false);
		draw->setTerrainDecalFadeTarget(0.0f, -0.2f);
	}

	if (d->m_startFX)
	{	
		FXList::doFXObj(d->m_startFX, obj);
	}

	if (d->m_ocl)
	{
		// TODO: Create Dynamic Scale module and pass geometry size of parent object;
		/* Object* newObject = */ ObjectCreationList::create(d->m_ocl, obj, NULL);
	}

	UnsignedInt now = TheGameLogic->getFrame();
	m_dieFrame = now;
	m_destructionFrame = now + d->m_destructionDelay;

	m_deathTriggered = TRUE;

	setWakeFrame(obj, UPDATE_SLEEP_NONE);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void ChronoDeathBehavior::onDie( const DamageInfo *damageInfo )
{
	if (!isDieApplicable(damageInfo))
		return;

	AIUpdateInterface* ai = getObject()->getAIUpdateInterface();
	if (ai)
	{
		// has another AI already handled us.
		if (ai->isAiInDeadState())
			return;
		ai->markAsDead();
	}
	beginChronoDeath(damageInfo);
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void ChronoDeathBehavior::crc( Xfer *xfer )
{

	// extend base class
	UpdateModule::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
	* Version Info:
	* 1: Initial version */
// ------------------------------------------------------------------------------------------------
void ChronoDeathBehavior::xfer( Xfer *xfer )
{

	// version
	XferVersion currentVersion = 1;
	XferVersion version = currentVersion;
	xfer->xferVersion( &version, currentVersion );

	// extend base class
	UpdateModule::xfer( xfer );

	// is triggered
	xfer->xferBool(&m_deathTriggered);

	// destruction frame
	xfer->xferUnsignedInt(&m_destructionFrame);

	// die frame
	xfer->xferUnsignedInt(&m_dieFrame);

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void ChronoDeathBehavior::loadPostProcess( void )
{

	// extend base class
	UpdateModule::loadPostProcess();

}  // end loadPostProcess
