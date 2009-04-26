//===========================================================================
//	Shadow	version 0.0
//
//
//	Made with doktor's BotStudio v. 0.98
//	http://www.planetquake.com/botstudio  e-mail: doktor@cutey.com
//===========================================================================


chat "shadow"
{
	#include "teamplay.h"

	type "game_enter"
	{
		"Aaargh Chaos power";
		"You Egghead";
		"I'm here in a Sonic Second";
	}

	type "game_exit"
	{
		"Catch me if you can";
		"I smell chilli dogs";
		"Do it, to it";
	}

	type "level_start"
	{
		"CHAOS POWER";
		"I feel the need for speed";
		"I'm waitin";
	}

	type "level_end"
	{
		"What?! Over so soon?";
		"I could have finished in half that time";
		"I could do this all day";
	}

	type "level_end_victory"
	{
		"I think I deserve a chilli dog";
		"Kiss my blurry butt";
		"You cant beat the power of the Chaos emeralds";
		"All fall before me.";
	}

	type "level_end_lose"
	{
		"I went easy on ya";
		"Slow and steady wins the race.... But lightning speeds are more my pace.";
		"My speed shoes came untied";
	}

	type "hit_talking"
	{
		"HEY, I was doing up my speed shoes";
		"Just wait till I powerup";
		"I'm handing you over to Eggman myself";
		"Grab every advantage, ", 0, ". It will not be enough.";
	}

	type "hit_nodeath"
	{
		"Too slow!";
		"Nothing a gold ring won't cure";
		"Damn I dropped my rings";
	}

	type "hit_nokill"
	{
		"You have a sonic second worth of time left";
		"Prepare the Robotisism Capsule for ", 0;
		"You can't get away";
	}

	type "death_telefrag"
	{
		"You saw the power of chaos first hand";
		"You saw the power of chaos first hand";
		"You saw the power of chaos first hand";
	}

	type "death_cratered"
	{
		"Next time I will land on ", 0;
		"Should have wore my spring shoes";
		"Next time I will land on ", 0;
	}

	type "death_lava"
	{
		"Like running across hot coals";
		"Wasn't watchin where I was heading";
		"This fur isn't flame proof";
	}

	type "death_slime"
	{
		"Eww";
		"Eww";
		"Eww";
	}

	type "death_drown"
	{
		"5......... 4......... 3......... 2......... 1.........";
		"I needed a water shield";
		"I needed a water shield";
	}

	type "death_suicide"
	{
		"I tripped";
		"I tripped";
		"Time for a chilli dog break";
		"Chaos power is just too strong ";
	}

	type "death_gauntlet"
	{
		"Now I'm going to super spin dash you";
		"Knuckles!! Help me!!";
		"If only Knuckles were here";
	}

	type "death_rail"
	{
		"I could have out ran that Rail Slug";
		"I should have stopped";
		"Lucky shot";
	}

	type "death_bfg"
	{
		"Aren't skilled enough to use a real gun huh?!";
		"I end in a green blaze. There could be no other way.";
		"I end in a green blaze. There could be no other way.";
	}

	type "death_insult"
	{
		"Another servant of Freedom Fighters eh?";
		"All followers of Freedom shall die";
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
		"Finally a weapon as fast as me";
		"Too SLOW";
		"Got to be faster than that";
		"You die, ", 0, ". Any way I choose.";
	}

	type "kill_gauntlet"
	{
		"You just got buzz sawed";
		"I smacked you in the head with a gold ring";
		"I knew these gloves were good for somthing";
	}

	type "kill_telefrag"
	{
		"You shouldn't have stood there ", 0;
		"If only you were faster than me.... HAHA";
		"You shouldn't have stood there ", 0;
	}

	type "kill_suicide"
	{
		"Did my speed confuse you?";
		"Now I can save energy for a worthy foe";
		"I agree with your choice, ", 0, ", for what was the point?";
	}

	type "kill_insult"
	{
		"I'm running rings around you";
		"Sonic speed too much for you?";
		"I'm running rings around you";
	}

	type "kill_praise"
	{
		"Join us, we need people like you to fight Freedom";
		"Join us, we need people like you to fight Freedom";
		"Join us, we need people like you to fight Freedom";
	}

	type "random_insult"
	{
		"You are as ugly as Eggman";
		"I'll catch up to ya";
		"Kiss my blurry  butt";
	}

	type "random_misc"
	{
		"There in a Sonic Second";
		"Feel the burn";
		"Catch me if you can";
	}
}
