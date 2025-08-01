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

// FILE: UpgradeSpecialPower.h /////////////////////////////////////////////////////////////////
// Author: Andreas W, July 25
// Desc:   Special Power will grant an upgrade to the object
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __UPGRADE_SPECIAL_POWER_H_
#define __UPGRADE_SPECIAL_POWER_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/SpecialPowerModule.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class FXList;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class UpgradeSpecialPowerModuleData : public SpecialPowerModuleData
{

public:

	UpgradeSpecialPowerModuleData(void);

	static void buildFieldParse(MultiIniFieldParse& p);

	AsciiString		m_upgradeName;			///< name of the upgrade to be granted.

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class UpgradeSpecialPower : public SpecialPowerModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE(UpgradeSpecialPower, "UpgradeSpecialPower")
		MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA(UpgradeSpecialPower, UpgradeSpecialPowerModuleData)

public:

	UpgradeSpecialPower(Thing* thing, const ModuleData* moduleData);
	// virtual destructor prototype provided by memory pool object

	virtual void doSpecialPower(UnsignedInt commandOptions);

	virtual void doSpecialPowerAtObject(Object* obj, UnsignedInt commandOptions);

protected:

	void grantUpgrade(Object* object);
};

#endif
