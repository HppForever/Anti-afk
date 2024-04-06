#include "../../main/main.h"

void Movement::AntiAfk::Actions::Position(bool JumpAndDuck_Return)
{
	switch (cvar.movement.anti_afk.position)
	{
	  case PositionType::Ducked:  g_Engine.pfnClientCmd(JumpAndDuck_Return ? CMD_DUCK_DEACTIVATED : CMD_DUCK_ACTIVATED); break;
	  case PositionType::Jumping: g_Engine.pfnClientCmd(JumpAndDuck_Return ? CMD_JUMP_DEACTIVATED : CMD_JUMP_ACTIVATED); break;

	  case PositionType::Ducked_And_Jumping:
	  {
		 g_Engine.pfnClientCmd(JumpAndDuck_Return ? CMD_DUCK_DEACTIVATED : CMD_DUCK_ACTIVATED);
		 g_Engine.pfnClientCmd(JumpAndDuck_Return ? CMD_JUMP_DEACTIVATED : CMD_JUMP_ACTIVATED);
	  }
	  break;
	}
}

void Movement::AntiAfk::Actions::Attack(bool AttackReturn)
{
	switch (cvar.movement.anti_afk.attack_type)
	{
	  case AttackType::Slash: g_Engine.pfnClientCmd(AttackReturn ? CMD_ATTACK_DEACTIVATED : CMD_ATTACK_ACTIVATED);   break;
	  case AttackType::Stab:  g_Engine.pfnClientCmd(AttackReturn ? CMD_ATTACK2_DEACTIVATED : CMD_ATTACK2_ACTIVATED); break;
	}
}

void Movement::AntiAfk::SetMove(const char* ForwardMove, const char* BackMove, const char* LeftMove, const char* RightMove, bool JumpAndDuck_Return, bool AttackReturn)
{
	g_Engine.pfnClientCmd(ForwardMove == skCrypt("move") ? CMD_FORWARD_ACTIVATED : CMD_FORWARD_DEACTIVATED);
	g_Engine.pfnClientCmd(BackMove == skCrypt("move") ? CMD_BACK_ACTIVATED : CMD_BACK_DEACTIVATED);
	g_Engine.pfnClientCmd(LeftMove == skCrypt("move") ? CMD_MOVELEFT_ACTIVATED : CMD_MOVELEFT_DEACTIVATED);
	g_Engine.pfnClientCmd(RightMove == skCrypt("move") ? CMD_MOVERIGHT_ACTIVATED : CMD_MOVERIGHT_DEACTIVATED);

	Actions::Position(JumpAndDuck_Return);
	Actions::Attack(AttackReturn);
}

void Movement::AntiAfk::SetRotation(const char* LeftMove, const char* RightMove, bool RotationReturn = false, bool JumpAndDuck_Return, bool AttackReturn)
{
	switch (cvar.movement.anti_afk.rotation_type)
	{
	  case RotationType::Left:
	  {
		  g_Engine.pfnClientCmd(RotationReturn ? CMD_LEFT_DEACTIVATED : CMD_LEFT_ACTIVATED);
		  g_Engine.pfnClientCmd(LeftMove == skCrypt("move") ? CMD_MOVELEFT_ACTIVATED : CMD_MOVELEFT_DEACTIVATED);
	  }
	  break;

	  case RotationType::Right:
	  {
		  g_Engine.pfnClientCmd(RotationReturn ? CMD_RIGHT_DEACTIVATED : CMD_RIGHT_ACTIVATED);
		  g_Engine.pfnClientCmd(RightMove == skCrypt("move") ? CMD_MOVERIGHT_ACTIVATED : CMD_MOVERIGHT_DEACTIVATED);
	  }
	  break;
	}

	Actions::Position(JumpAndDuck_Return);
	Actions::Attack(AttackReturn);
}

void Movement::AntiAfk::Reset_AntiAfk(static bool& Initialized)
{
	if (Initialized)
	{
		switch (cvar.movement.anti_afk.rotation_type)
		{
		  case RotationType::DontRotate: SetMove(skCrypt("no move"), skCrypt("no move"), skCrypt("no move"), skCrypt("no move"), true, true); 
			  break;

		  default: SetRotation(skCrypt("no move"), skCrypt("no move"), true, true, true);
			  break;
		}

		Initialized = false;
	}
}

void Movement::AntiAfk::Run()
{
	static bool Initialized = false;
	static bool Moving = true;
	static float LastChangeTime = 0.0f;

	if (!cvar.movement.anti_afk.enable) 
	{ 
	    Reset_AntiAfk(Initialized);
	    return; 
	}

	if (g_Local.m_bIsDead || g_Local.m_iIndex == SPECTATOR) 
	{
		Reset_AntiAfk(Initialized);
		return;
	}

	if (!(pmove->flags & FL_ONGROUND)) 
	{
		Reset_AntiAfk(Initialized);
		return;
	}

	if (cvar.movement.anti_afk.auto_disable_on_activity)
	{
		if (cvar.visuals.hud_info.key.forward || cvar.visuals.hud_info.key.back || cvar.visuals.hud_info.key.duck || cvar.visuals.hud_info.key.moveright || cvar.visuals.hud_info.key.moveleft || cvar.visuals.hud_info.key.jump || cvar.visuals.hud_info.key.duck || state.movement.bunnyhop_active || state.movement.gstrafe_active || state.movement.gstrafe_standup_active || state.movement.strafe_helper_active || state.movement.rage_strafe_active || state.movement.speedhack_active || state.movement.goto_active)
		{
			Reset_AntiAfk(Initialized);
			return;
		}
	}

	switch (cvar.movement.anti_afk.rotation_type)
	{
	  case RotationType::Left:
	  {
		  SetRotation(skCrypt("move"), skCrypt("no move"));
		  Initialized = true; 
		  return; 
	  } 
	  break;

	  case RotationType::Right:
	  {
		  SetRotation(skCrypt("no move"), skCrypt("move"));
		  Initialized = true; 
		  return; 
	  } 
	  break;
	}

	float CurrentTime = g_Engine.GetClientTime();

	if (!Initialized || CurrentTime - LastChangeTime > cvar.movement.anti_afk.interval)
	{
		if (Moving)
		{
			switch (cvar.movement.anti_afk.direction)
			{
			  case DirectionType::Left_And_Right:       
				  SetMove(skCrypt("no move"), skCrypt("no move"), skCrypt("move"), skCrypt("no move")); 
				  break;

			  case DirectionType::Forward_And_Backward:
				  SetMove(skCrypt("move"), skCrypt("no move"), skCrypt("no move"), skCrypt("no move")); 
				  break;
			}

			Moving = false;
		}
		else
		{
			switch (cvar.movement.anti_afk.direction)
			{
			  case DirectionType::Left_And_Right:  
				  SetMove(skCrypt("no move"), skCrypt("no move"), skCrypt("no move"), skCrypt("move"));
				  break;

			  case DirectionType::Forward_And_Backward: 
				  SetMove(skCrypt("no move"), skCrypt("move"), skCrypt("no move"), skCrypt("no move")); 
				  break;
			}

			Moving = true;
		}

		LastChangeTime = CurrentTime;
		Initialized = true;
	}
}
