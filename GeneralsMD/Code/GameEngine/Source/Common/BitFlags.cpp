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

// FILE: BitFlags.cpp ///////////////////////////////////////////////////////////
//
// Used to set detail levels of various game systems.
//  Steven Johnson, Sept 2002
//
//
///////////////////////////////////////////////////////////////////////////////

#include "PreRTS.h"	// This must go first in EVERY cpp file in the GameEngine

#include "Common/BitFlags.h" 
#include "Common/BitFlagsIO.h"
#include "Common/ModelState.h"
#include "GameLogic/ArmorSet.h"

template<>
const char* ModelConditionFlags::s_bitNameList[] = 
{	
	"TOPPLED", 
	"FRONTCRUSHED",
	"BACKCRUSHED",
	"DAMAGED",
	"REALLYDAMAGED",
	"RUBBLE",
	"SPECIAL_DAMAGED",
	"NIGHT",
	"SNOW",
	"PARACHUTING",
	"GARRISONED",
	"ENEMYNEAR",
	"WEAPONSET_VETERAN",
	"WEAPONSET_ELITE",
	"WEAPONSET_HERO",
	"WEAPONSET_CRATEUPGRADE_ONE",
	"WEAPONSET_CRATEUPGRADE_TWO",
	"WEAPONSET_PLAYER_UPGRADE",
	"DOOR_1_OPENING",
	"DOOR_1_CLOSING",
	"DOOR_1_WAITING_OPEN",
	"DOOR_1_WAITING_TO_CLOSE",
	"DOOR_2_OPENING",
	"DOOR_2_CLOSING",
	"DOOR_2_WAITING_OPEN",
	"DOOR_2_WAITING_TO_CLOSE",
	"DOOR_3_OPENING",
	"DOOR_3_CLOSING",
	"DOOR_3_WAITING_OPEN",
	"DOOR_3_WAITING_TO_CLOSE",
	"DOOR_4_OPENING",
	"DOOR_4_CLOSING",
	"DOOR_4_WAITING_OPEN",
	"DOOR_4_WAITING_TO_CLOSE",
	"ATTACKING",
	"PREATTACK_A",
	"FIRING_A",
	"BETWEEN_FIRING_SHOTS_A",
	"RELOADING_A",
	"PREATTACK_B",
	"FIRING_B",
	"BETWEEN_FIRING_SHOTS_B",
	"RELOADING_B",
	"PREATTACK_C",
	"FIRING_C",
	"BETWEEN_FIRING_SHOTS_C",
	"RELOADING_C",
	"TURRET_ROTATE",
	"POST_COLLAPSE",
	"MOVING",
	"DYING",
	"AWAITING_CONSTRUCTION",
	"PARTIALLY_CONSTRUCTED",
	"ACTIVELY_BEING_CONSTRUCTED",
	"PRONE",
	"FREEFALL",
	"ACTIVELY_CONSTRUCTING",
	"CONSTRUCTION_COMPLETE",
	"RADAR_EXTENDING",
	"RADAR_UPGRADED",
	"PANICKING",	// yes, it's spelled with a "k". look it up.
	"AFLAME",
	"SMOLDERING",
	"BURNED",
	"DOCKING",
	"DOCKING_BEGINNING",
	"DOCKING_ACTIVE",
	"DOCKING_ENDING",
	"CARRYING",
	"FLOODED",
	"LOADED",
	"JETAFTERBURNER",
	"JETEXHAUST",
	"PACKING",
	"UNPACKING",
	"DEPLOYED",
	"OVER_WATER",
	"POWER_PLANT_UPGRADED",
	"CLIMBING",
	"SOLD",
#ifdef ALLOW_SURRENDER
	"SURRENDER",
#endif
	"RAPPELLING",
	"ARMED",
	"POWER_PLANT_UPGRADING",
	
	"SPECIAL_CHEERING",

	"CONTINUOUS_FIRE_SLOW",
	"CONTINUOUS_FIRE_MEAN",
	"CONTINUOUS_FIRE_FAST",

	"RAISING_FLAG",
	"CAPTURED",

	"EXPLODED_FLAILING",
	"EXPLODED_BOUNCING",
	"SPLATTED",

	"USING_WEAPON_A",
	"USING_WEAPON_B",
	"USING_WEAPON_C",

	"PREORDER",
	
	"CENTER_TO_LEFT",
	"LEFT_TO_CENTER",
	"CENTER_TO_RIGHT",
	"RIGHT_TO_CENTER",

	"RIDER1",	//Kris: Added these for different combat-bike riders, but feel free to use these for anything.
	"RIDER2",
	"RIDER3",
	"RIDER4",
	"RIDER5",
	"RIDER6",
	"RIDER7",
	"RIDER8",

  "STUNNED_FLAILING", // Daniel Teh's idea, added by Lorenzen, 5/28/03
	"STUNNED",
	"SECOND_LIFE",
	"JAMMED",
	"ARMORSET_CRATEUPGRADE_ONE",
	"ARMORSET_CRATEUPGRADE_TWO",

	"USER_1",	
	"USER_2",
	
	"DISGUISED",

	// New Weaponsets
	"WEAPONSET_PLAYER_UPGRADE2",
	"WEAPONSET_PLAYER_UPGRADE3",
	"WEAPONSET_PLAYER_UPGRADE4",

	// New Weaponslots (D-H)
	
	"PREATTACK_D",
	"FIRING_D",
	"BETWEEN_FIRING_SHOTS_D",
	"RELOADING_D",
	"USING_WEAPON_D",

	"PREATTACK_E",
	"FIRING_E",
	"BETWEEN_FIRING_SHOTS_E",
	"RELOADING_E",
	"USING_WEAPON_E",

	"PREATTACK_F",
	"FIRING_F",
	"BETWEEN_FIRING_SHOTS_F",
	"RELOADING_F",
	"USING_WEAPON_F",

	"PREATTACK_G",
	"FIRING_G",
	"BETWEEN_FIRING_SHOTS_G",
	"RELOADING_G",
	"USING_WEAPON_G",

	"PREATTACK_H",
	"FIRING_H",
	"BETWEEN_FIRING_SHOTS_H",
	"RELOADING_H",
	"USING_WEAPON_H",

	"TAKEOFF",
	"LANDING",

	"TELEPORT_RECOVER",
	
	NULL
};

template<>
const char* ArmorSetFlags::s_bitNameList[] = 
{
	"VETERAN",
	"ELITE",
	"HERO",
	"PLAYER_UPGRADE",
	"WEAK_VERSUS_BASEDEFENSES",
	"SECOND_LIFE",
	"CRATE_UPGRADE_ONE",
	"CRATE_UPGRADE_TWO",
	"PLAYER_UPGRADE2",
	"PLAYER_UPGRADE3",
	"PLAYER_UPGRADE4",

	NULL
};

