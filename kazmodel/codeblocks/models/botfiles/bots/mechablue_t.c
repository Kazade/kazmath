//===========================================================================
//	MechaBlue	version 0.0
//
//
//	Made with doktor's BotStudio v. 0.98
//	http://www.planetquake.com/botstudio  e-mail: doktor@cutey.com
//===========================================================================


chat "mechablue"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Prepare to be robotisized";
		"Dr. Robotnik needs your parts";
		"Objective set: Destroy Red";
	}

	type "game_exit"
	{
		"You are of no use to the Robotic legion";
		"Exiting training arena";
		"MechaBlue shall return";
	}

	type "level_start"
	{
		"Powering up";
		"Target located in Sector 14F7B, preparing to intercept";
		"Weapons are armed and ready to fire";
	}

	type "level_end"
	{
		"We will meet again";
		"You are worthy enough to be Robotisized";
		"You are weak from battle. Robotnik will attack soon";
	}

	type "level_end_victory"
	{
		"You all belong to Robotnik";
		"Bow to your new master, Dr Robotnik";
		"You will await further instructions";
	}

	type "level_end_lose"
	{
		"I have failed my master";
		"I have harnessed the chaos emerald, How can I fail";
		"I will be upgraded";
	}

	type "hit_talking"
	{
		"You are evil enough to join the Robotic legion ", 0;
		 0, ", I shall robotisize you personally";
		"I expected as much from Red scum";
	}

	type "hit_nodeath"
	{
		"Systems Damaged";
		"Cerebral Cortex Damaged";
		"Power Loss Imminent";
	}

	type "hit_nokill"
	{
		"Robotnik requires your technology";
		"Your death is imminent";
		"Prepare the Robotisism Capsule for ", 0;
	}

	type "death_telefrag"
	{
		"You saw the power of chaos first hand";
		"You saw the power of chaos first hand";
		"You saw the power of chaos first hand";
	}

	type "death_cratered"
	{
		"Hydraulic Limbs Failing";
		"Next time Robotnik will install booster rockets";
		"Next time Robotnik will install booster rockets";
	}

	type "death_lava"
	{
		"Emerald Core is cracking under thermal heat";
		"From heat I was created, and to heat I shall return";
		"I can not be salvaged master";
	}

	type "death_slime"
	{
		"Such toxins would aid the Rogotic legion";
		"Such toxins would aid the Rogotic legion";
		"Such toxins would aid the Rogotic legion";
	}

	type "death_drown"
	{
		"Robotnik shall salvage me";
		"Robotnik shall salvage me";
		"I will pollute these waters with oil and toxic waste";
	}

	type "death_suicide"
	{
		"Self Destruction Set";
		"You will not acquire my technology";
		"Forgive me Dr. Robotnik";
	}

	type "death_gauntlet"
	{
		"I was programmed for long range combat only";
		"Aren't you skilled enough to hold a gun?";
		"Sending Message: Attack ", 0, ", he is without weapons";
	}

	type "death_rail"
	{
		"You have pierced my core";
		 0, " must be a camper";
		"Your vision is needed for the Robotic legion";
	}

	type "death_bfg"
	{
		"Such a powerful weapon should only be possessed by Dr. Robotnik";
		"Such a powerful weapon should only be possessed by Dr. Robotnik";
		"Such a powerful weapon should only be possessed by Dr. Robotnik";
	}

	type "death_insult"
	{
		"Another servant of Red eh?";
		"All followers of Red shall die";
		"How'd you beat me after I have defeated ol' Egg features";
	}

	type "death_praise"
	{
		"Nearly as fast as me!";
		"You must have some of my sonic speed";
		"Nearly as fast as me!";
	}

	type "kill_rail"
	{
		"All of the Mecha Sonic series are equipped with night vision";
		"The remains of ", 0, " will be robotisised";
		"I will augment your remains to my own";
	}

	type "kill_gauntlet"
	{
		"Are you aware of the saw blades down my spikes?";
		"The form I was based on battled close range only";
		"Such an easy target isn't worth Robotisising";
	}

	type "kill_telefrag"
	{
		"You shouldn't have stood there ", 0;
		"You shouldn't have stood there ", 0;
		"You have been crushed by Mecha Blue";
	}

	type "kill_suicide"
	{
		"I can still augment your remains";
		"You are wise to surrender";
		"Now I can save my ammo for use on the rest of Red";
	}

	type "kill_insult"
	{
		"I'm running rings around you";
		"Sonic speed too much for you?";
		"I'm running rings around you";
	}

	type "kill_praise"
	{
		"Join us, we need people like you";
		"Join us, we need people like you";
		"Join us, we need people like you";
	}

	type "random_insult"
	{
		"You are an inferior creature";
		"There is nowhere to run, nowhere to hide.";
		"You will await the harsh fist of Robotnik";
	}

	type "random_misc"
	{
		"Join Dr Robotnik's army";
		"Prepare to be Robotisised";
		"Bow down to the power of the Chaos Emeralds";
	}
}
