using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace jacobsenroad
{
    static public class Datagen
    {

        static public string[] FirstNames = { "John", "Ben", "An", "Casey", "Ryan", "Justin", "Michael", "Cherry", "Luke", "Leia", "Vader", "Artoo", "Threepio", "Chewie", "Han", "Jarjar" };
        static public string[] Comments = { "A long time ago, in a galaxy far, far, away...", 
                                              "It is a period of civil war. Rebel spaceships, striking from a hidden base, have won their first victory against the evil Galactic Empire.", 
                                              "During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the Death Star, an armored space station with enough power to destroy an entire planet.", 
                                              "Pursued by the Empire's sinister agents, Princess Leia races home aboard her starship, custodian of the stolen plans that can save her people and restore freedom to the galaxy...", 
                                              "The awesome yellow planet of Tatooine emerges from a total eclipse, her two moons glowing against the darkness.", 
                                              "A tiny silver spacecraft, a Rebel Blockade Runner firing lasers from the back of the ship, races through space." ,
                                              "It is pursed by a giant Imperial Stardestroyer. Hundreds of deadly laserbolts streak from the Imperial Stardestroyer, causing the main solar fin of the Rebel craft to disintegrate.",
                                              "Did you hear that? They've shut down the main reactor. We'll be destroyed for sure. This is madness!",
                                              "There'll be no escape for the Princess this time.",
                                              "What's that?",
                                              "Hurry up! Come with me! What are you waiting for?! Get in gear!",
                                              "Artoo! Artoo-Detoo, where are you?",
                                              "At last! Where have you been?",
                                              "They're heading in this direction. What are we going to do? We'll be sent to the spice mine of Kessel or smashed into who knows what!",
                                              "Wait a minute, where are you going?",
                                              "The Death Star plans are not in the main computer.",
                                              "Where are those transmissions you intercepted?",
                                              "What have you done with those plans?",
                                              "We intercepted no transmissions. Aaah... This is a consular ship. Were on a diplomatic mission.",
                                              "If this is a consular ship... were is the Ambassador?",
                                              "Great shot, kid. That was one in a million.",
                                              "Remember, the Force will be with you... always.",
                                              "I have you now.",
                                              "You may fire when ready.",
                                              "Rebel base, in range.",
                                              "Commence primary ignition.",
                                              "The Force is strong with this one!",
                                              "Luke, trust me.",
                                              "Use the Force, Luke.",
                                              "I'm on the leader.",
                                              "Rebel base, thirty seconds and closing.",
                                              "It'll be just like Beggar's Canyon back home.",
                                              "Luke, at that speed will you be able to pull out in time?",
                                              "Hokey religions and ancient weapons are no match for a good blaster at your side, kid.",
                                              "You don't believe in the Force, do you?",
                                              "Kid, I've flown from one side of this galaxy to the other. I've seen a lot of strange stuff, but I've never seen anything to make me believe there's one all-powerful force controlling everything. There's no mystical energy field that controls my destiny.",
                                              "Remember, a Jedi can feel the Force flowing through him.",
                                              "You mean it controls your actions?",
                                              "Partially. But it also obeys your commands.",
                                              "Anyway, we should be at Alderaan about oh-two-hundred hours.",
                                              "Well, you can forget your troubles with those Imperial slugs. I told you I'd outrun 'em.",
                                              "I felt a great disturbance in the Force... as if millions of voices suddenly cried out in terror and were suddenly silenced. I fear something terrible has happened.",
                                              "I need your help, Luke. She needs your help. I'm getting too old for this sort of thing.",

                                          };

        static public string GetFirstName()
        {
            Random rnd = new Random();
            return FirstNames[rnd.Next(0, FirstNames.Count())];
        }

        static public string GetComment()
        {
            Random rnd = new Random();
            return Comments[rnd.Next(0, Comments.Count())];
        }
    }
}
