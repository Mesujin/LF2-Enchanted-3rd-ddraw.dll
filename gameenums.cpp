#include "gameenums.h"

void RegisterGameEnums(asIScriptEngine *ScriptEngine){
	ScriptEngine->RegisterEnum("ObjectStates");

	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_STANDING",0);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_WALKING",1);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_RUNNING",2);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_ATTACKING",3);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_JUMPING",4);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_DASHING",5);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_ROWING",6);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_DEFENDING",7);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BROKEN_DEFENCE",8);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_CATCHING",9);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_CAUGHT",10);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_INJURED",11);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_FALLING",12);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_FROZEN",13);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LYING",14);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_OTHER",15);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_DOP",16);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_DRINKING",17);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BURNING",18);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_FIRERUNNING",19);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LOUIS_DASH_ATTACKING",100);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_DEEP_STRAFING",301);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_TELEPORTING_ENEMY",400);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_TELEPORTING_FRIEND",401);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_RUDOLF_TRANSFORMING",500);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_RUDOLF_TRANSFORMING2",501);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LIGHT_WEAPON_FALLING",1000);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LIGHT_WEAPON_HELD",1001);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LIGHT_WEAPON_THROWN",1002);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LIGHT_WEAPON_JUST_ON_GROUND",1003);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LIGHT_WEAPON_GROUND",1004);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_SELF_HEALING",1700);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_HEAVY_WEAPON_FALLING",2000);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_HEAVY_WEAPON_HELD",2001);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_HEAVY_WEAPON_GROUND",2004);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_FLYING",3000);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_HITTING",3001);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_HIT",3002);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_REBOUND",3003);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_DISAPPEAR",3004);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_FLYING_INVIS",3005);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_BALL_PIERCING",3006);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_CRIMINAL_TRANSFORM_BEGIN",8000);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_CRIMINAL_TRANSFORM_END",8999);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LOUIS_TRANSFORM",9995);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LOUIS_ARMOR",9996);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LOUIS_MESSAGE",9997);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LOUIS_DELETE",9998);
	ScriptEngine->RegisterEnumValue("ObjectStates","STATE_LOUIS_BROKEN_WEAPON",9999);

	ScriptEngine->RegisterEnum("ItrKinds");

	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_NORMAL", 0);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_CATCH_DOP", 1);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_PICK_WEAPON", 2);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_CATCH", 3);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_THROWN", 4);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_WEAPON", 5);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_FINISH_HIM", 6);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_ROWING_PICK", 7);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_HEAL", 8);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_SHIELD", 9);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_FLUTE", 10);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_FLUTE_2", 11);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_OBSTACLE", 14);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_WHIRLWIND_LIFT", 15);
	ScriptEngine->RegisterEnumValue("ItrKinds", "KIND_WHIRLWIND_FREEZE", 16);
}