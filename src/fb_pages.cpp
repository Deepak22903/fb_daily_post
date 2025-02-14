#include "../include/header.h"

void family_island() {

  cout << "                                   "
          "╔════════════════════════════════════════╗\n"
          "                                   ║             Family Island      "
          "        ║\n"
          "                                   "
          "╚════════════════════════════════════════╝\n"
       << endl;

  cout << "\nGenerating prompt for image..." << endl;
  string prompt = generatePrompt(

      "Suggest a single prompt, don't output anything other than the "
      "prompt. The prompt will be given to a text-to-image generation AI. "
      "dont include and single or double quotes in the response. Generate "
      "cartoon style images. And the text Free Energy should be visible."
      "The image is about Family Island game and free energy, which is an "
      "item inside the game. Here is some context about the game: "
      "Immerse yourself in an unforgettable world of adventure with a "
      "modern "
      "Stone Age family! Imagine what your life would be without modern "
      "technology—what would you do? Perhaps you would explore "
      "territories, build houses or even entire villages; farm, harvest, "
      "or maybe you would conquer new lands. You have an opportunity to "
      "be on a desert island together with the heroes of the Family "
      "Island game and try yourself in different roles: farmer, cook, "
      "explorer, trader, and many others."
      "Interested? Here are some more features of our game:"
      "★ Explore wild territories, solve puzzles, find hidden objects, and "
      "embark on thrilling adventures."
      "★ Build and improve your little city in the middle of the ocean."
      "★ Start your own family farm! Harvest, grow crops, and craft useful "
      "goods to trade with others."
      "★ Cook healthy and tasty food using ingredients found on the "
      "island."
      "★ Customize your village with beautiful decorations! Choose flowers "
      "and plants to match the unique landscapes."
      "★ Meet unusual animals: island hamsters, wild goats, and even a "
      "dinosaur are waiting for you!"
      "★ Help a family survive on a desert island."
      "And that is not all! Family Island is a farm game full of "
      "unexpected twists and captivating adventures!");
  cout << "Prompt generated successfully!" << endl;
  cout << "Generating image..." << endl;
  string img_path = generateImage(prompt);
  cout << "Image generated successfully with path : " << img_path << endl;
  cout << "Publishing to facebook..." << endl;
  string post_id = post_to_fb(img_path, FB_PAGE_ID_FAMILY_ISLAND,
                              FB_ACCESS_TOKEN_FAMILY_ISLAND);
  cout << "Post added with post_id : " << post_id << endl;
}

void coin_tales() {

  cout << "                                   "
          "╔════════════════════════════════════════╗\n"
          "                                   ║               Coin Tales       "
          "        ║\n"
          "                                   "
          "╚════════════════════════════════════════╝\n"
       << endl;

  cout << "\nGenerating prompt for image..." << endl;
  string prompt = generatePrompt(

      "Suggest a single prompt, don't output anything other than the "
      "prompt. The prompt will be given to a text-to-image generation AI. "
      "dont include and single or double quotes in the response "
      "The image is about Coin Tales game and free spins, which is an "
      "item inside the game. Generate cartoon style images. And the text Free "
      "Spins should be visible. Here is some "
      "context about the game: "
      " Step into the immersive universe of Coin Tales, a thrilling, "
      "interactive mobile game that brings the excitement of the world to your "
      "fingertips. Embark on an adventure that will see you build, raid, and "
      "trade your way to legendary status in a land of enchanting whimsy and "
      "endless fun."

      "🌍 EXPLORE A VAST WORLD: Traverse through magical realms and diverse "
      "landscapes in your quest to become the supreme Coin Tales champion. "
      "Each location you conquer is filled with unique coins, treasures, and "
      "characters waiting to be discovered."

      "BUILD YOUR DREAM KINGDOM: Use your hard-earned loot to construct your "
      "empire. From simple cottages to imposing castles, the power is yours to "
      "create. Fortify your kingdom's defenses to ward off attacks from both "
      "friends and rivals."

      "⚔️ RAID OTHER KINGDOMS: Connect with players from around the globe! Spin "
      "the wheel to snatch their coins or pillage their treasure. But stay "
      "alert – they can seek revenge! Launch daring raids to amass your "
      "fortune."

      "🔄 TRADE TO THRIVE: Need to complete your card collection? Our trading "
      "feature allows you to swap cards with friends. The more collections you "
      "complete, the greater the rewards!"

      "🎁 DAILY REWARDS & BONUSES: Sign in every day for enticing bonuses and "
      "spin the wheel for a chance to claim fantastic prizes. Participate in "
      "our weekly events for an added layer of thrill."

      "🐷 PET COMPANIONS: You’re never alone on this journey. Unlock adorable "
      "pet companions that enhance your earnings and safeguard your kingdom. "
      "Level them up and watch your wealth multiply!"

      "👑 LEADERBOARDS & COMPETITIONS: Compete with players from around the "
      "world and ascend the global leaderboard. The more you play, the more "
      "opportunities you have to claim victory!");

  cout << "Prompt generated successfully!" << endl;
  cout << "Generating image..." << endl;
  string img_path = generateImage(prompt);
  cout << "Image generated successfully with path : " << img_path << endl;
  cout << "Publishing to facebook..." << endl;
  string post_id =
      post_to_fb(img_path, FB_PAGE_ID_COIN_TALES, FB_ACCESS_TOKEN_COIN_TALES);
  cout << "Post added with post_id : " << post_id << endl;
}
