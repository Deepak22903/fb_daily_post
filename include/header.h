#ifndef HEADER_H
#define HEADER_H
#include <Poco/Base64Encoder.h>
#include <chrono>
#include <cppcodec/base64_rfc4648.hpp>
#include <cstddef>
#include <ctime>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;
using namespace cppcodec;

// Base64 encoding table
const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789+/";

// ✅ Use `const string` instead of `#define`
const string CLOUDFLARE_ACCOUNT_ID = "d03ba4adab44b19322eed9dd5a40de7d";
const string CLOUDFLARE_AUTH_TOKEN = "MkPwb3osbvgKuSWn8XR43dnk3vAs2VW-EgXZ_pek";
const string FB_PAGE_ID_FAMILY_ISLAND = "555233441009283";
const string FB_ACCESS_TOKEN_FAMILY_ISLAND =
    "EAAdBZBbj3fdwBO69Ltwr34trvRwRCHhe5vkgyfMYIMnrbfELEuVAK7EEdccGYDQ4Lq0a6dwZC"
    "jGigVyaIiDZBdpsU1wLEBXMu9vi7pA7iT6ZBiZCNQJZAj5qDMf8CD8tLXHhlR1mFd8m6yZCREs"
    "kVsOZB2dOMf0dKzZBNGXYLJVfSgdDmt70KaNPZCqCIW3WU1YiiU";
const string FB_PAGE_ID_COIN_TALES = "587922784397058";
const string FB_ACCESS_TOKEN_COIN_TALES =
    "EAAdBZBbj3fdwBO99QPl9sKbjWxQmgQ2LKgqgkY6djZC5a2ymBBMfBUZCzTokH5DZAV5Duwu31"
    "MceEXPhV8TVXkkyAgGpc5ZAbfWC5UQnumMh4gLM6jW7cpgOSrornyJSijIklATiWBkVxSGNVwy"
    "xMDZA9FuHWAJYaZBtcl8He1fYoxK7nlHThaRUSocdUymJ5mZC";
// Function Declarations
std::string image_to_base64(const std::string &image_path);
string generatePrompt(string prompt_input);
string generateImage(string prompt);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *output);
string post_to_fb(string generated_img, string page_id, string fb_access_token);
string get_date();
string generateImageFromImage(string img, string prompt);
void family_island();
void coin_tales();

#endif // HEADER_H
