#ifndef HEADER_H
#define HEADER_H
#include "./dotenv/dotenv.h"
#include <Poco/Base64Encoder.h>
#include <chrono>
#include <cppcodec/base64_rfc4648.hpp>
#include <cstddef>
#include <cstdlib>
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

extern std::string CLOUDFLARE_ACCOUNT_ID;
extern std::string CLOUDFLARE_AUTH_TOKEN;
extern std::string FB_PAGE_ID;
extern std::string FB_ACCESS_TOKEN;

// Base64 encoding table
const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789+/";
// Function Declarations
string getEnvVar(const string &key);
void load_env_vars();
std::string image_to_base64(const std::string &image_path);
string generatePrompt(string prompt_input);
string generateImage(string prompt);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *output);
string post_to_fb(string generated_img, string page_id, string fb_access_token,
                  string caption);
string get_date();
string generateImageFromImage(string img, string prompt);
void family_island();
void coin_tales();
string caption_randomizer(vector<string> captions);

#endif // HEADER_H
