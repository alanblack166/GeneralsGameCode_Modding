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

// FILE: ChronoDeathBehavior.h /////////////////////////////////////////////////////////////////////////
// Author: Steven Johnson, Sep 2002
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __ChronoDeathBehavior_H_
#define __ChronoDeathBehavior_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/DieModule.h"

class FXList;
class ObjectCreationList;
class WeaponTemplate;
class DamageInfo;

//-------------------------------------------------------------------------------------------------
class ChronoDeathBehaviorModuleData : public UpdateModuleData
{
public:
	DieMuxData m_dieMuxData;

	const ObjectCreationList* m_ocl;
	const FXList* m_startFX;
	const FXList* m_endFX;

	Real m_startScale;
	Real m_endScale;
	Real m_startAlpha;
	Real m_endAlpha;

	UnsignedInt	m_destructionDelay;

	ChronoDeathBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse& p);

private:

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class ChronoDeathBehavior : public UpdateModule, public DieModuleInterface
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( ChronoDeathBehavior, "ChronoDeathBehavior" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( ChronoDeathBehavior, ChronoDeathBehaviorModuleData )

public:

	ChronoDeathBehavior( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	static Int getInterfaceMask() { return UpdateModule::getInterfaceMask() | (MODULEINTERFACE_DIE); }

	// BehaviorModule
	virtual DieModuleInterface* getDie() { return this; }

	// UpdateModuleInterface
	virtual UpdateSleepTime update();
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DISABLEDMASK_ALL; }

	// DieModuleInterface
	virtual void onDie( const DamageInfo *damageInfo );
	virtual Bool isDieApplicable(const DamageInfo* damageInfo) const { return getChronoDeathBehaviorModuleData()->m_dieMuxData.isDieApplicable(getObject(), damageInfo); }


protected:

	virtual void beginChronoDeath(const DamageInfo* damageInfo);

private:
	Bool m_deathTriggered;

	UnsignedInt m_destructionFrame;
	UnsignedInt m_dieFrame;

};

#endif // __ChronoDeathBehavior_H_

