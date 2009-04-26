//===========================================================================
//	AquaSonic	version 0.0
//
//
//	Made with doktor's BotStudio v. 0.98
//	http://www.planetquake.com/botstudio  e-mail: doktor@cutey.com
//===========================================================================


chat "aquasonic"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Hey the cool blue hedgehog has arrived!";
		"Say hello to Sonic, the galaxiest coolest hedgehog";
		"Hey the cool blue hedgehog has arrived!";
	}

	type "game_exit"
	{
		"Gottagoseeyouround'bye!";
		"Gottagoseeyouround'bye!";
		"Over already?";
	}

	type "level_start"
	{
		"Sonic speed!";
		"Sonic speed and these weapons will win!";
		"Sonic speed!";
	}

	type "level_end"
	{
		"Sonic power!";
		"Sonic power!";
		"Sonic power!";
	}

	type "level_end_victory"
	{
		"Sonic power wins!";
		"If I can beat Ro-butt-nik, I'm not surporised I beat you lot";
		"Sonic power!";
		"First Robotnik, then Metallix, now ", 0;
	}

	type "level_end_lose"
	{
		"I tripped";
		"I'm outta shape";
		"I stopped to tie my laces";
		"How come you beat me after I beat Robotnik";
	}

	type "hit_talking"
	{
		"You could teach the badniks a lesson in tactics";
		"You could teach the badniks a lesson in tactics";
		"You could teach the badniks a lesson in tactics";
	}

	type "hit_nodeath"
	{
		"Too fast for you to kill huh?";
		"Sonic speed means I dodge most of your ammo";
		"Too fast for you to kill huh?";
	}

	type "hit_nokill"
	{
		"Too sloow to catch this hedgehog!";
		"You can't shoot what you can't see!";
		"I can out run your weapons!";
	}

	type "death_telefrag"
	{
		"Guess I shoulda used my sonic speed ";
		"Sonic speed wasn't quite enough there huh?";
		"Guess I shoulda used my sonic speed ";
	}

	type "death_cratered"
	{
		"Trust Robotnik to remove the springpads!";
		"Trust Robotnik to remove the springpads!";
		"AAAH! No springpad!";
	}

	type "death_lava"
	{
		"Oh no, a lava trap!";
		"Oh no, a lava trap";
		"Oh no, a lava trap";
	}

	type "death_slime"
	{
		"Oh no Mega mack!";
		"Oh no Mega mack!";
		"Oh no Mega mack!";
	}

	type "death_drown"
	{
		"Glub! Gasp!";
		"I need a water shield";
		"5...4...3..2...1...";
	}

	type "death_suicide"
	{
		"Robotnik'll never take me alive!";
		"Robotnik'll never take me alive!";
		"Robotnik'll never take me alive!";
	}

	type "death_gauntlet"
	{
		"Now I'm going to super spin dash you";
		"Knuckles!! Help me!!";
		"If only Knuckles were here";
	}

	type "death_rail"
	{
		"Sonic speed  wasn't enough";
		"I coulda outrun that slug";
		"Faster than me!";
	}

	type "death_bfg"
	{
		"Wow that's the kinda gun Robotnik would love";
		"Sonic speed wasn't enough";
		"Wow that's the kinda gun Robotnik would love";
	}

	type "death_insult"
	{
		"Another servant of Robotnik eh?";
		"All followers of Robotnik shall die";
		"How'd you beat me after I have defeated ol' Egg features";
	}

	type "death_praise"
	{
		"Nearly as fast as me!";
		"Nearly as fast as me!";
		"You must have some of my sonic speed";
	}

	type "kill_rail"
	{
		"This gun's nearly as fast as I am";
		"This gun's nearly as fast as I am";
		"This gun's nearly as fast as I am";
	}

	type "kill_gauntlet"
	{
		"Spin attack!";
		"Spin attack!";
		"Super spin attack!";
	}

	type "kill_telefrag"
	{
		"Guess you didn't see me cos I'm so fast";
		"Sonic speed was too much for you huh?";
		"Guess you didn't see me cos I'm so fast";
	}

	type "kill_suicide"
	{
		"Robotnik would appreciate your sacrifice";
		"Couldn't face losing to the coolest hedgehog on Mobius I guess";
		"Couldn't face losing to the coolest hedgehog on Mobius I guess";
	}

	type "kill_insult"
	{
		"I'm running rings around you";
		"I'm running rings around you";
		"Sonic speed too much for you?";
	}

	type "kill_praise"
	{
		"Join us, we need people like you to fight eggman";
		"Join us, we need people like you to fight eggman";
		"Join us, we need people like you to fight eggman";
	}

	type "random_insult"
	{
		"Join Robotnik's army, he needs losers like you ", 0;
		 0, " would be a good henchman for egg breath";
		 1, " wasn't fast enough will ", 0, " be better?";
	}

	type "random_misc"
	{
		"I can beat Robotnik, so I can beat you ";
		"I can beat Robotnik, so I can beat you ";
		"Super sonic speed can beat anyone and out run anything";
	}
}
