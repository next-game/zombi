/**
 * Created: 2023-09-01
 * @author Yauhen Saroka <yauhen.saroka@gmail.com>
 */

enum InediaInfectedAI_PlayerIdentity {
	ANY,
	NAME,
	STEAMID,
	PID
};

// fIv/bBWEg1uHCX+RmC3SGWE1c2xSd3UwdzNwRlFLUDlPS0Y5TmFhRStiMUxUd0hUSTUwckYyZ3ZSK1V3WlM4R0QvYndjUkZ1anFoZ1JjMGZsdW96ZjZPZkZrQkNZTUpXZWNDUXBBNEFvUWxzR2VFQnppUW05U1JzbnI0UUc4TWhjZUFoUnpiSVAzV3JEWWZtZnJFeG41bWcwUDQ4eU9iWGJPK2dudHZsNkZRNDQ1NjRlTHN1SmE3U1VmSXBBd1Vvcyt3eEtzbUdiS1lwelY0bCtkWjdCRU5ScGlBandYekxTT2R1L2VhcWJRbEVtd0w5eDIwaVlicFJnekx3dUc3K3oyeE1RaGlLUGpuRTE1dEZpSlUwQjBxMkNGVFNIclppK2g4ZVNRPT0=

modded class MissionServer extends MissionBase
{
	ref InediaInfectedAI_Smells InediaInfectedAI_SmellsInstance;

	override void OnInit()
	{
		super.OnInit();

		InediaInfectedAI_SmellsInstance = InediaInfectedAI_Smells.GetInstance();
	}

	override void TickScheduler(float timeslice)
	{
		super.TickScheduler(timeslice);

		InediaInfectedAI_SmellsInstance.TickScheduler(timeslice);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();

		InediaInfectedAI_Debug.InitDebugRules();
		InediaInfectedAIAdmins_Config.GetInstance();
		InediaInfectedAI_Config.GetInstance();
	}
	
	override void EquipCharacter(MenuDefaultCharacterData char_data)
	{
		super.EquipCharacter(char_data);
		
		if (m_player) {
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_player.InediaInfectedAI_RespawnEvent, 3000, false);
		}
	}

	PlayerBase InediaInfectedAI_GetPlayer(string tag, InediaInfectedAI_PlayerIdentity type)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		PlayerBase p;

		bool nameMatch;
		bool steamIdMatch;
		bool pidMatch;

		for (int i = 0; i < players.Count(); ++i)
		{
			Class.CastTo(p, players.Get(i));

			nameMatch = p.GetIdentity().GetName() == tag;
			steamIdMatch = p.GetIdentity().GetPlainId() == tag;
			pidMatch = p.GetID() == tag.ToInt();

			if ( type == InediaInfectedAI_PlayerIdentity.ANY ) {
				if ( nameMatch || steamIdMatch || pidMatch )
					return p;
			}

			else if ( type == InediaInfectedAI_PlayerIdentity.NAME ) {
				if ( nameMatch )
					return p;
			}

			else if ( type == InediaInfectedAI_PlayerIdentity.STEAMID ) {
				if ( steamIdMatch )
					return p;
			}

			else if ( type == InediaInfectedAI_PlayerIdentity.PID ) {
				if ( pidMatch )
					return p;
			}
		}

		return null;
	}

	void InediaInfectedAI_SendPlayerMessage(PlayerBase player, string message)
	{
		if (player.GetIdentity()) {
			Param1<string> Msgparam;
			Msgparam = new Param1<string>(message);
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
		}
	}

	void InediaInfectedAI_MySplit(string text, string delim, out TStringArray list)
	{
		string temp = text + delim;
		string word = "";

		for (int i = 0; i < temp.Length(); i++ )
		{
			string x = temp.Get(i);

			if ( x != delim ) {
				word = word + x;
			}
			else {
				list.Insert(word);
				word = "";
			}
		}
	}

	bool InediaInfectedAI_Command(PlayerBase player, string command)
	{
		if (!player) {
			return false;
		}
		
		const string helpMsg = "InediaInfectedAI available commands: \\ihide \\igm \\ifgm \\iheal \\irep \\irepauto \\ieq \\ieq2 \\ieq3 \\ieq4 \\ieq5 \\iammo \\iday \\inight \\icar \\izmb \\izmbm izmbs \\iplayer \\ieai \\ieaishaman \\iwolf \\ibear \\ideer \\iclear \\idebug \\ireload";

		// Split command message into args
		TStringArray args = new TStringArray;
		InediaInfectedAI_MySplit(command, " ", args);

		PlayerBase targetPlayer;
		float valuePercent;

		switch (args[0]) {
			case "\\idebug":
				map<string, bool> debugRules = InediaInfectedAI_Debug.GetDebugRules();
			
				string rulesText = "";
				foreach (string ruleName, bool ruleValue: debugRules) {
					rulesText += ruleName + " ";
				}
				rulesText = rulesText.Trim();
			
				if (args.Count() != 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\idebug [<ruleName>|stop]");
					InediaInfectedAI_SendPlayerMessage(player, "Rules: " + rulesText);
					return false;
				}
			
				string debugRule = args[1];
			
				if (debugRule == "stop") {
					InediaInfectedAI_Debug.DisableAllRules();
					InediaInfectedAI_SendPlayerMessage(player, "All debuggers have been stopped.");
					return false;
				} else if (debugRule == "dmg") {
					bool pvzIsActive = false;
#ifdef PVZMOD_CUSTOMISABLE_ZOMBIES
					pvzIsActive = true;
#else
	#ifdef PVZMOD_DARK_HORDE
					pvzIsActive = true;
	#endif
#endif

					if (pvzIsActive) {
						InediaInfectedAI_SendPlayerMessage(player, "Debug damage output is unavailable with the PvZ mod due to its own HP and damage system.");
						return false;
					}
				}
			
				if (!debugRules.Contains(debugRule)) {
					InediaInfectedAI_SendPlayerMessage(player, "Rule \"" + debugRule + "\" does not exist.");
					InediaInfectedAI_SendPlayerMessage(player, "Rules: " + rulesText);
				
					return false;
				}
			
				InediaInfectedAI_Debug.EnableRule(debugRule);
			
				InediaInfectedAI_SendPlayerMessage(player, "Debugger \"" + debugRule + "\" has been started.");
			
				break;
			
			case "\\ihide":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ihide [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}

				if (!targetPlayer.InediaInfectedAI_IsInHideMode()) {
					targetPlayer.InediaInfectedAI_SetHideMode();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") hide mode is turned on.");
				} else {
					targetPlayer.InediaInfectedAI_UnsetHideMode();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") hide mode is turned off.");
				}

				break;
			
			case "\\igm":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\igm [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}
			
				if (targetPlayer.GetAllowDamage()) {
					targetPlayer.SetAllowDamage(false);
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") invincibility mode is turned on.");
				} else {
					targetPlayer.SetAllowDamage(true);
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") invincibility mode is turned off.");
				}

				break;
			
			case "\\ifgm":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ifgm [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}
			
				if (!targetPlayer.InediaInfectedAI_IsInHideMode()) {
					targetPlayer.InediaInfectedAI_SetHideMode();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") hide mode is turned on.");
				}
			
				if (targetPlayer.GetAllowDamage()) {
					targetPlayer.SetAllowDamage(false);
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") invincibility mode is turned on.");
				}
			
				if (!targetPlayer.InediaInfectedAI_HasUnlimitedAmmo()) {
					targetPlayer.InediaInfectedAI_SetUnlimitedAmmo();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") unlimited ammo has been activated.");
				}
			
				if (!targetPlayer.m_InediaInfectedAI_HealEquipAuto) {
					targetPlayer.m_InediaInfectedAI_HealEquipAuto = true;
					targetPlayer.InediaInfectedAI_HealEquipAuto();
					InediaInfectedAI_SendPlayerMessage(player, "Player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + "). Automatic equipment repair every 5 seconds is enabled.");
				}

#ifdef INEDIA_STAMINA
				if (!targetPlayer.InediaStamina_IsUnlimitedStamina()) {
					targetPlayer.InediaStamina_SetUnlimitedStamina();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") unlimited stamina is turned on.");
				}
#endif

				break;

			case "\\iheal":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iheal [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}

				targetPlayer.InediaInfectedAI_Heal();
				InediaInfectedAI_SendPlayerMessage(player, "Player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") has been healed.");

				break;
			
			case "\\irep":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\irep [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}

				targetPlayer.InediaInfectedAI_HealEquip();
				InediaInfectedAI_SendPlayerMessage(player, "Player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + "). All equipment and items have been repaired.");

				break;
			
		case "\\irepauto":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\irepauto [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}

				if (!targetPlayer.m_InediaInfectedAI_HealEquipAuto) {
					targetPlayer.m_InediaInfectedAI_HealEquipAuto = true;
					targetPlayer.InediaInfectedAI_HealEquipAuto();
					InediaInfectedAI_SendPlayerMessage(player, "Player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + "). Automatic equipment repair every 5 seconds is enabled.");
				} else {
					targetPlayer.m_InediaInfectedAI_HealEquipAuto = false;
					InediaInfectedAI_SendPlayerMessage(player, "Player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + "). Automatic equipment repair every 5 seconds is disabled.");
				}
				
				

				break;
			
			case "\\ieq":
			case "\\ieq2":
			case "\\ieq3":
			case "\\ieq4":
			case "\\ieq5":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ieq* [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}

				switch (args[0]) {
					case "\\ieq":
						InediaInfectedAI_Spawner.InediaInfectedAI_PlayerTestEquip1(targetPlayer);
						break;
					case "\\ieq2":
						InediaInfectedAI_Spawner.InediaInfectedAI_PlayerTestEquip2(targetPlayer);
						break;
					case "\\ieq3":
						InediaInfectedAI_Spawner.InediaInfectedAI_PlayerTestEquip3(targetPlayer);
						break;
					case "\\ieq4":
						InediaInfectedAI_Spawner.InediaInfectedAI_PlayerTestEquip4(targetPlayer);
						break;
					case "\\ieq5":
						InediaInfectedAI_Spawner.InediaInfectedAI_PlayerTestEquip5(targetPlayer);
						break;
				}
			
				InediaInfectedAI_SendPlayerMessage(player, "Player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + "). Test equipment has been added.");
	
				break;
			
			case "\\iammo":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iammo [playerName|playerSteamID]");
					return false;
				}

				if (args.Count() == 1) {
					targetPlayer = player;
				} else if (args.Count() == 2) {
					targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.NAME);
					if (!targetPlayer) {
						targetPlayer = InediaInfectedAI_GetPlayer(args[1], InediaInfectedAI_PlayerIdentity.STEAMID);
					}

					if (!targetPlayer) {
						InediaInfectedAI_SendPlayerMessage(player, "Player not found: \"" + args[1] + "\"");
						return false;
					}
				}
			
				if (!targetPlayer.InediaInfectedAI_HasUnlimitedAmmo()) {
					targetPlayer.InediaInfectedAI_SetUnlimitedAmmo();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") unlimited ammo has been activated.");
				} else {
					targetPlayer.InediaInfectedAI_UnsetUnlimitedAmmo();
					InediaInfectedAI_SendPlayerMessage(player, "For player " + targetPlayer.GetIdentity().GetName() + " (" + targetPlayer.GetIdentity().GetPlainId() + ") unlimited ammo has been deactivated.");
				}

				break;
			
			case "\\iday":
				if (args.Count() != 1) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iday");
					return false;
				}

				GetGame().GetWorld().SetDate(2022, 10, 15, 9, 0);
			
				InediaInfectedAI_SendPlayerMessage(player, "Daytime has been activated.");

				break;
			
			case "\\inight":
				if (args.Count() != 1) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\inight");
					return false;
				}

				GetGame().GetWorld().SetDate(2022, 10, 15, 0, 0);
			
				InediaInfectedAI_SendPlayerMessage(player, "Nighttime has been activated.");

				break;
			
			case "\\irain":
				if (args.Count() != 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\irain <0...100>");
					return false;
				}
			
				float rainIntensityPercent = Math.Clamp(args[1].ToFloat(), 0, 100);
				
				GetGame().GetWeather().GetOvercast().Set(0.6, 0, 36000);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().GetWeather().GetRain().Set, 1000, false, rainIntensityPercent / 100, 0, 36000);
			
				InediaInfectedAI_SendPlayerMessage(player, "The rain intensity is set to " + rainIntensityPercent + "%.");

				break;
			
			case "\\isnow":
				if (args.Count() != 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\isnow <0...100>");
					return false;
				}
			
				float snowfallIntensityPercent = Math.Clamp(args[1].ToFloat(), 0, 100);

				GetGame().GetWeather().GetOvercast().Set(0.6, 0, 36000);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().GetWeather().GetSnowfall().Set, 1000, false, snowfallIntensityPercent / 100, 0, 36000);
			
				InediaInfectedAI_SendPlayerMessage(player, "The snowfall intensity is set to " + snowfallIntensityPercent + "%.");

				break;
			
			case "\\iwind":
				if (args.Count() != 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iwind <0...20>");
					return false;
				}
			
				float windMagnitude = Math.Clamp(args[1].ToFloat(), 0, 20);

				GetGame().GetWeather().GetOvercast().Set(0.6, 0, 36000);
				GetGame().GetWeather().GetWindDirection().Set(GetGame().GetWeather().WindDirectionToAngle(player.GetDirection()), 0, 36000);
				GetGame().GetWeather().GetWindMagnitude().Set(windMagnitude, 0, 36000);

				InediaInfectedAI_SendPlayerMessage(player, "The wind speed is set to " + windMagnitude + " m/s, direction aligned with the player's view.");

				break;
			
			case "\\icar":
				if (args.Count() != 1) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\icar");
					return false;
				}

				InediaInfectedAI_Spawner.Car(player);
			
				InediaInfectedAI_SendPlayerMessage(player, "The test car was spawned.");

				break;
			
			case "\\izmb":
			case "\\izmbm":
			case "\\izmbs":
				if (args.Count() < 1 || args.Count() > 3) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\izmb [distance] [amount]");
					return false;
				}

				float zmbDistance = 10;
				int zmbAmount = 1;
			
				if (args.Count() == 2) {
					zmbDistance = args[1].ToFloat();
				} else if (args.Count() == 3) {
					zmbDistance = args[1].ToFloat();
					zmbAmount = args[2].ToInt();
				}
			
				if (zmbDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (zmbDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else if (zmbAmount <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [amount] parameter.");
				} else if (zmbAmount > 300) {
					InediaInfectedAI_SendPlayerMessage(player, "The amount cannot be more than 300.");
				} else {
					switch (args[0]) {
						case "\\izmb":
							InediaInfectedAI_Spawner.Zombies(player, zmbDistance, zmbAmount, 0);
							break;
						case "\\izmbm":
							InediaInfectedAI_Spawner.Zombies(player, zmbDistance, zmbAmount, 1);
							break;
						case "\\izmbs":
							InediaInfectedAI_Spawner.Zombies(player, zmbDistance, zmbAmount, 2);
							break;
					}
				}
			
				break;
			
			case "\\iplayer":
				if (args.Count() < 1 || args.Count() > 4) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iplayer [distance] [armor] [reversepain]");
					return false;
				}

				float playerDistance = 10;
				bool playerArmor = true;
				bool playerReversepain = true;
			
				if (args.Count() == 2) {
					playerDistance = args[1].ToFloat();
				} else if (args.Count() == 3) {
					playerDistance = args[1].ToFloat();
					playerArmor = args[2].ToInt();
				} else if (args.Count() == 4) {
					playerDistance = args[1].ToFloat();
					playerArmor = args[2].ToInt();
					playerReversepain = args[3].ToInt();
				}
			
				if (playerDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (playerDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else if (playerArmor < 0 || playerArmor > 1) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [armor] parameter.");
				} else if (playerReversepain < 0 || playerReversepain > 1) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [reversepain] parameter.");
				} else {
					InediaInfectedAI_Spawner.Player(player, playerDistance, playerArmor, playerReversepain);
				
					InediaInfectedAI_SendPlayerMessage(player, "The test player was spawned.");
				}
			
				break;
			
			case "\\ieai":
#ifndef EXPANSIONMODAI
					InediaInfectedAI_SendPlayerMessage(player, "The 'DayZ-Expansion-AI' mod is not installed.");
					return false;
#endif
			
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ieai [distance]");
					return false;
				}

				float eAIDistance = 10;
			
				if (args.Count() == 2) {
					eAIDistance = args[1].ToFloat();
				}
			
				if (eAIDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (eAIDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else {
					if (InediaInfectedAI_ExpansionAICompatibility.SpawnEAIBot(player, eAIDistance)) {
						InediaInfectedAI_SendPlayerMessage(player, "The eAI player was spawned.");
					} else {
						InediaInfectedAI_SendPlayerMessage(player, "Failed to spawn the eAI player.");
					}
				}
			
				break;
			
			case "\\ieaishaman":
#ifndef EXPANSIONMODAI
					InediaInfectedAI_SendPlayerMessage(player, "The 'DayZ-Expansion-AI' mod is not installed.");
					return false;
#endif
			
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ieaishaman [distance]");
					return false;
				}

				float eAIShamanDistance = 10;
			
				if (args.Count() == 2) {
					eAIShamanDistance = args[1].ToFloat();
				}
			
				if (eAIShamanDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (eAIShamanDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else {
					if (InediaInfectedAI_ExpansionAICompatibility.SpawnEAIShamanFactionBot(player, eAIShamanDistance)) {
						InediaInfectedAI_SendPlayerMessage(player, "The eAI 'Shaman' faction player was spawned.");
					} else {
						InediaInfectedAI_SendPlayerMessage(player, "Failed to spawn the eAI 'Shaman' faction player.");
					}
				}
			
				break;
			
			case "\\iwolf":
				if (args.Count() < 1 || args.Count() > 3) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iwolf [distance] [amount]");
					return false;
				}

				float wolfDistance = 10;
				int wolfAmount = 1;
			
				if (args.Count() == 2) {
					wolfDistance = args[1].ToFloat();
				} else if (args.Count() == 3) {
					wolfDistance = args[1].ToFloat();
					wolfAmount = args[2].ToInt();
				}
			
				if (wolfDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (wolfDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else if (wolfAmount <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [amount] parameter.");
				} else if (wolfAmount > 300) {
					InediaInfectedAI_SendPlayerMessage(player, "The amount cannot be more than 300.");
				} else {
					InediaInfectedAI_Spawner.Wolves(player, wolfDistance, wolfAmount);
				}
			
				break;
			
			case "\\ibear":
				if (args.Count() < 1 || args.Count() > 3) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ibear [distance] [amount]");
					return false;
				}

				float bearDistance = 10;
				int bearAmount = 1;
			
				if (args.Count() == 2) {
					bearDistance = args[1].ToFloat();
				} else if (args.Count() == 3) {
					bearDistance = args[1].ToFloat();
					bearAmount = args[2].ToInt();
				}
			
				if (bearDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (bearDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else if (bearAmount <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [amount] parameter.");
				} else if (bearAmount > 300) {
					InediaInfectedAI_SendPlayerMessage(player, "The amount cannot be more than 300.");
				} else {
					InediaInfectedAI_Spawner.Bears(player, bearDistance, bearAmount);
				}
			
				break;
			
			case "\\ideer":
				if (args.Count() < 1 || args.Count() > 3) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\ideer [distance] [amount]");
					return false;
				}

				float deerDistance = 10;
				int deerAmount = 1;
			
				if (args.Count() == 2) {
					deerDistance = args[1].ToFloat();
				} else if (args.Count() == 3) {
					deerDistance = args[1].ToFloat();
					deerAmount = args[2].ToInt();
				}
			
				if (deerDistance <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [distance] parameter.");
				} else if (deerDistance > 1000) {
					InediaInfectedAI_SendPlayerMessage(player, "The distance cannot be more than 1000 meters.");
				} else if (deerAmount <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [amount] parameter.");
				} else if (deerAmount > 300) {
					InediaInfectedAI_SendPlayerMessage(player, "The amount cannot be more than 300.");
				} else {
					InediaInfectedAI_Spawner.Deers(player, deerDistance, deerAmount);
				}
			
				break;
			
			case "\\iclear":
				if (args.Count() < 1 || args.Count() > 2) {
					InediaInfectedAI_SendPlayerMessage(player, "Syntax: \\iclear [radius]");
					return false;
				}

				float clearRadius = 500;
			
				if (args.Count() == 2) {
					clearRadius = args[1].ToFloat();
				}
			
				if (clearRadius <= 0) {
					InediaInfectedAI_SendPlayerMessage(player, "Incorrect [radius] parameter.");
				} else {
					// Remove all zombies/animals/test players/items/vehicles in [radius] from player
					array<EntityAI> nearbyEntities = new array<EntityAI>;
        			MiscGameplayFunctions.InediaInfectedAI_GetEntitiesAtPosition3D(player.GetPosition(), clearRadius, nearbyEntities);
				    foreach (EntityAI nearEntity : nearbyEntities) {
						if (nearEntity.IsInherited(BaseBuildingBase)) {
							continue;
						}
					
						if (nearEntity.IsInherited(ZombieBase) || nearEntity.IsInherited(AnimalBase) || nearEntity.IsInherited(ItemBase) || nearEntity.IsInherited(CarScript)) {
				        	nearEntity.Delete();
						} else if (nearEntity.IsInherited(PlayerBase)) {
			                PlayerBase playerToDelete = PlayerBase.Cast(nearEntity);
			                if (playerToDelete && playerToDelete.m_InediaInfectedAI_IsTestPlayer) {
			                    playerToDelete.Delete();
			                }
						}
				    }
				
					InediaInfectedAI_SendPlayerMessage(player, "All zombies/animals/test players/items/vehicles within a radius of " + clearRadius + " meters have been removed.");
				}
			
				break;
			
			case "\\idmgdebug":
				InediaInfectedAI_SendPlayerMessage(player, "This command is deprecated, use \"\\idebug dmg\" instead.");
				return false;

				break;
			
			case "\\iirrdebug":
				InediaInfectedAI_SendPlayerMessage(player, "This command is deprecated, use \"\\idebug irr\" instead.");
				return false;

				break;
			
			case "\\ireload":
				InediaInfectedAI_Config.ReloadInstance();
				InediaInfectedAI_SmellsInstance.InitConfig();
			
				if (!InediaInfectedAI_Config.m_InstanceInitError) {
					InediaInfectedAI_SendPlayerMessage(player, "InediaInfectedAI: The configuration file has been reloaded.");
				} else {
					player.InediaInfectedAI_SendImportantMessage("InediaInfectedAI: Error in the configuration file, please check the server crash logs.");
				}
			
				break;

			default:
				//InediaInfectedAI_SendPlayerMessage(player, helpMsg);
				return false;
				break;
		}

		return true;
	}

//                                                                                                                          +*+-
//                                                                        ..   ..                                           *+++
//                                                                   .-+####%%#%%%#*=:                                      #==*
//                                                                .-*#*+*##***#%%%##%%*-                                    %--#
//                                                              .=##++#%#++++*#%%*+++=*%*.                                 .%--%
//                                                             :##++#**++++++=+#%#*###+*##.                                .%::%
//                                                            =%*=*#++++**+++=+=*%#***%=#%#.                               :%::%.
//                                                          .+%*=**+*+#******#+%+%#+*%**:%##.                              :#::#:
//                                                         :##*++**###*#*+===--:-=+#=+%+-:*##.                             -#::#:
//                                                       :*%**=*###%%%#.           .#+=##=:+%#:                            =#:.#-
//                                                      +%%+*=###%%%%#---------------##+*#*=+*%=.                          =#:.#-
//                                                     =%#+=+#%*#%%%*.                =%%#*#+=*##-                         +*-.#=
//                                                    :%%+=#*##%%%%=                   *%#%#****%%+.                       **-.#=
//                                                    *#*####*+*+#: :-=+=:.    .-=+=-. *%:%%%###*#%=                       *+-.*=
//                                                   -%%#+=-=+%%=+. :++*##+.  -++*##*- %--%%%%%##*%*                       #+:.*=
//                                                 .+%*=-+#*=#%%++-  .--:--=  :-.:::. =*-#%%%*%*#%%#                       %+:.*=
//                                               .-%*+*##%--%#+#%%#:       =         ===%%%%%=#-#%%*                       %=-.*=
//                                               .:-*###+=+=-=#%%%%==.    --  .    :=.:%%%%%%=+-#%%=                      .%=-.*+
//                                               .+%#*+==-=#%%%%%%%=.=.   .:-.:   .:  =%%%%%*--#%%%:                      .%=:.++
//                                            .=*%*=--=*%%##**#%%%%%: -.  .:-::.     :%%%%#+==#%%%#                       .%-:.++
//                                          :*%*--+##**++=+*#%%%%%%%%=.: :=::::=:  .+%##***##*%%%%-.:.                    :%-:.+*
//                                         =##--==#+===+=#%%#%%%%%%%%%%=. :--::. .=#=#+%%%%%%*#%%#**=+*+:                 :%-:.+*.
//                                        -*=:=-=*-#-+-*%%%#+%%%%%%%%%%**+:    .+*++*+-*%%%%%%%%%+=-:::-+*=.              -%-:.=*.
//                                        +.+ =+=:#+.+=##%%**%%%%%%%%%%+-+******===-:.==#%%%%%*=-:.    .::-**-.           -%+:-+#
//                                        =.*.=#..*#.=++##%#%%%%%%%%%%%:-:::::--:.     -#%%%*--. :.       .::=*-.         =%*:#-#
//                                        .:-*:**::++:+*.-+%%%%%%%%%%%%.-:    =:     .:-*%*=-. ::           :-=##-        =%+:#:%
//                                         ..:#++%%#%#+#*==*%#==+%%%#*%.-:    :-..:-=+#%#=-. --        ----------+*:      =%+.%.%
//                                         .=+-*%%%%%%%%%%%+:.*%%%*=.****+=====++--==#%=-. .=.      :=+:.---. .. --+=.    =%+.% %.
//                                        .+.+%%%%%%%%%%#=. =%%%*-*.=*#=       ::-+=*%:   .:      :--.:-:       ...--*:   +%+ # %.
//                  .                     -.*%%%%%%%%#+:  :#%##-.:--*=.      :--==::#*    .      ... .            ..:-+=  +%+.# %.
//                  ..                    .=%%%%%%%*-    -%%+*.   :::-----=+=-::    +#:                            .. =++.+%==+ %.
//                   ...                   #%%%%%+:     +%%*=    .:=-::::..        ::*%*=-::::---=====------------::.--*+**%=-+ %.
//                     .:                  %%%%=.      +%%*:  .--:.            .:-=---:=%##**++++++****#####***++=-==++==%%%* + %:.
//                       ::               :%%%=:     .*%%#:.-=:.           .-=+=-:  :::.=:=*%%%%%%%%%#*-:-*+==-::.  .-*#*+%%* + *++...
//                        .:-.           :*%%*-     .*%%%++-.           :=*+-:    ..  :-=:+*%%%%%%%%%#=+-  -*==++**#*+-.::-%#.+.=%+  ..:::.
//                           :-::.       .-%%=.    :#%#+-:.         .-+=:         .:::-=+ *-%%%%%%%%%%%+-+-. -+-..:..:-*###%* ==+%+      ..:.....
//                             .:::::--=+#%%+:    +%*-..         .-+*-          :::---. - =:#%%%%%%%%%%%#::=.:.:+=-=**+=--=%# ---%+             ..::.
//                                .::..:--+#-   .**:        .  ::--:         .::--:      - +-%%%%%%%%%%%%%+.-+==+*+-..:=*=+%* -=-%+                 :..
//                                   .::. ++.  :%=      .::...:--.         .:..+.        -:--+%%%%%%%%%%%%%#=*+=:..:=+=-  -%* -=-%+                   .
//                                      .-%*-..%+     ::..::.--           :- .+.          -:=-:=*#%%%%%%%%%%#+:---++-     -%* -=-%+
//                                        :-=*%%    .: .:. --             =  +.            -:=-  :-+*##*#%%%%%***=.      .=%* -=-%=
//                                          -+#*         :=.             -. :-              --+      ..=%%%%%%%%*.      :=#%* -=-#.
//                                         ==-=%..      ==               -  =               *+:       :%%%%%%%%%%%=  :+#+:=%* -+-##-
//                                       .+*=: #+=.    --                :  -              *= :-=-...-%%%%%%%%%%%%%###=.  =%* -+-%%**.
//                                    -+#%# =- :%+*:   -                    .            -=--===-===#%%%%%%%%%%%%%%%%#:  .=%* -%#*: :+.
//                                 .-*#-%%-  :=.:##%- .                              .-======-.   .*%=*%%%%%%%%%%%%%%%%=  =%* -%=.   .+:
//                              .-*#+::=*%#    :--*%%+.                      ..::-==**=-===:      +#:  .+%%%%%%%%%%%%%%%*.=%+ =%:    -=+=
//                            :+#*-.   *:%%*.     .-+%%%###===============+*=*+++=++=---.       .#*.     .=%%%%%%%%%%%%%%#*%+ =%:..:--. #*.=:
//                          :**=:.     =::#%%=:::::-+%%%*-.--.            = =.:-:.             :%+.        .*%%%%%%%%%%%%%%%+ =*=.-:. :*%*+=%*.
//                      .-+*=-:        .+  :+##***#%%=.+%=-. :=-.        .= * =               -%-           .%%%%+-%%%%%%**%+ -++:. :*%#+:::+%+
//                    -**-::.           =.    ..:=%*:  =% .-=- .-=-.      = * =              :#:           .*%%*=..%%%%+: +%= -:#-=##=.  :+= *#
//                 .=*=...               +    .=#*-    =#    .-=: .---.   = +.-.      ...    #:         .:=##+:   =%%+:   +%= -:*%+:  :==: = *#
//                =#=  .                 :=-+##=:      .#+      .=-:  ---:= -:.=        :---=+          .:..    .=#=.     +%= -:*% :=*+-.  = *#
//              .=*=:--=-           .:-+***=-.          =%*:       :==:  :---+.=    .:::----*+                 .-:        +%= -:#%*=+: .   = **.
//             .*#:==-: .=-   .:-+*#*+-:.               #%%=          .-=-.  .-=**=-:...    :%*:                          +%= -:#%.==    .:+ #*.
//             -%:  .::=: -**##*=-.                     =%-==-.          .-==:   :==-.    ::-*#%#-.                       +%= -:#% ==      + %+
//            :#*=+++++=++**-.                          .%#:  :---.          :---:. .:------::-#%%#=.                     +%= -:#% =-      = %=
//           :%#+=-----*%+.                             .%+.-==:  :---.          .-===-:.   ....=%%%%+:                   +%= -:#%.*.      =.%-
//          -=:        #=                              .*%: .--:-=-: .--=-.           .:-=====+==+%%%%%*-                 +%= -:#%.+       =:%:
//       .==.         =+                              :*##=::      :---: .:----.              -..:+=*%%%%#=.              +%= -:#%:+       -=%.
//       == .:.:-.    *.                             :#+==*=.          :----: .:-----.  .:------:. .:=%%%%%%=.            +%= -.##--       =#+
//      +-.  :+%%#.  :=                             .###. .===-.            .---=-::-=--:..      .-=**..=#%%%%+:          *%= -=+#=        =*-
//      .+=+.-*++*: :%:                            -%+ .==-.::--===:..::------::..::::....::-=+**+=:.+.   -*%%%%*-        +%= -:+#::::--:. ==-
//       -+*..-#=. -%#+                             -%*:  :+=. :::.:::...::::::::::-=+*###*=-:.      :*.    :+%%%%#=.     +%= --#+:::::.:-=++=
//       .+.==*=. :%-:#                             .***+.  .+*:  ::-:::. .-=+*##*+=-:--:::::--::.    -+      .=#%%%%+.   +%= -*%*++==-:.::*=.
//       .===+#--=**-=-                           ==-- .-*+.  .=*=.    :+*+=-:.  .::::.       .::---:::*:       .-#%%%%*- *%+ *%*--==+**##*#.
//           .-=+--+:                            .#+:===++#%+.   -#+-=*+.    .---.         .::.    .::..:---.      :*%%%%##%= *%-         .#
//                                              =#-.=.     :=#*-   :=-.   :=-:           ::.  :+*#%%%%%%%#=.+-       :*%+=#%+ *%-         .*
//                                              #*::.=.       :+#+:   :===:            .:.:.-#%%*=-:...:=#%#=====      :  *%* *%-        .*:.:.
//                                             -%#--#.+          :=+++=.              .:.:.+%#-           -#%#- +-  =-:.  **-##%=::::---=##*###
//                                           .+%=.  =+-:      .                      .:.-:#%+               :#%=-#- %#+=-:=*=-=##########***#:.
//                                          -#+.    :%.+      :.        .            : :=%%*                  *%*#%*%%#+.:=+-*--=====+=--+=*=
//                                        .=#:      -%:+      -.        =           -..+%#.:+                 .:#%%%%%#-+-.+ #  .:    ...=+*
//                                       .**.       -%:+      --        *          :: *%+.  *.                  -+%%%%*#::+*+#=.       ::=#:
//                                      -#=         =%.+      :+       :*          - *%-    ::                   *=%%%=-%+.:%=  =: .:  :=*=

	override void OnEvent(EventType eventTypeId, Param params)
	{
		switch (eventTypeId) {
			// Handle user command
			case ChatMessageEventTypeID:
				ChatMessageEventParams chatParams;
				Class.CastTo(chatParams, params);

				string cmd = string.ToString(chatParams.param3, false, false, false);
				if (cmd.Get(0) != "\\") break;

				string senderName = string.ToString(chatParams.param2, false, false, false);
				PlayerBase sender = InediaInfectedAI_GetPlayer(senderName, InediaInfectedAI_PlayerIdentity.NAME);

				if (sender) {
					if (sender.InediaInfectedAI_IsAdmin()) {
						InediaInfectedAI_Command(sender, cmd);
					} else {
						// InediaInfectedAI_SendPlayerMessage(sender, "Not enough permissions to execute command.");
					}
				}
			break;
		}

		super.OnEvent(eventTypeId, params);
	}
}
