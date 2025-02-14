#include "../include/header.h"

std::string image_to_base64(const std::string &image_path) {
  // Read the image as binary data
  std::ifstream file(image_path, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + image_path);
  }

  std::ostringstream oss;
  Poco::Base64Encoder encoder(oss);
  encoder << file.rdbuf(); // Encode file content into Base64
  encoder.close();

  return oss.str();
}
string get_date() {
  auto now = chrono::system_clock::now();
  time_t now_time = chrono::system_clock::to_time_t(now);
  tm local_time = *localtime(&now_time);
  ostringstream oss;
  oss << put_time(&local_time, "%d %B %Y");
  string date = oss.str();
  return date;
}

// Callback function to store response
size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     string *output) {
  size_t totalSize = size * nmemb;
  output->append((char *)contents, totalSize);
  return totalSize;
}

string generatePrompt(string prompt_input) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    cerr << "Failed to initialize cURL" << endl;
    return "";
  }

  string response_data;

  // Cloudflare API endpoint
  string url = "https://api.cloudflare.com/client/v4/accounts/" +
               CLOUDFLARE_ACCOUNT_ID +
               "/ai/run/@cf/meta/llama-3.1-70b-instruct";

  // Bearer token for authentication
  string auth_header = "Authorization: Bearer " + CLOUDFLARE_AUTH_TOKEN;

  // JSON payload
  json payload = {
      {"messages",
       {{{"role", "system"}, {"content", "You are a friendly assistant"}},
        {{"role", "user"}, {"content", prompt_input}}}}};
  string jsonData = payload.dump();

  // Set cURL options
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, auth_header.c_str());

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
  curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");

  // Perform request
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    cerr << "cURL request failed: " << curl_easy_strerror(res) << endl;
  } else {
    try {
      // Parse JSON response
      json jsonResponse = json::parse(response_data);

      // Extract the "response" field
      if (jsonResponse.contains("result") &&
          jsonResponse["result"].contains("response")) {
        string extracted_response =
            jsonResponse["result"]["response"].get<string>();
        // Remove surrounding quotes if they exist
        if (!extracted_response.empty() && extracted_response.front() == '"' &&
            extracted_response.back() == '"') {
          extracted_response =
              extracted_response.substr(1, extracted_response.size() - 2);
        }
        return extracted_response;
      } else {
        cout << "Response field not found in JSON!" << endl;
        cout << jsonResponse << endl;
      }
    } catch (json::parse_error &e) {
      cerr << "JSON parsing error: " << e.what() << endl;
    }
  }

  // Cleanup
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  return "";
}

string generateImage(string prompt) {
  CURL *curl = curl_easy_init();
  string response_data;
  if (!curl)
    cerr << "Failed to initialize cURL" << endl;

  string url = "https://api.cloudflare.com/client/v4/accounts/" +
               CLOUDFLARE_ACCOUNT_ID +
               "/ai/run/@cf/black-forest-labs/flux-1-schnell";

  string auth_hdr = "Authorization: Bearer " + CLOUDFLARE_AUTH_TOKEN;

  string content_type_header = "Content-Type: application/json";

  prompt = "{ \"prompt\": \"" + prompt + "\" }";

  struct curl_slist *headers = NULL;

  headers = curl_slist_append(headers, content_type_header.c_str());
  headers = curl_slist_append(headers, auth_hdr.c_str());

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, prompt.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

  CURLcode res = curl_easy_perform(curl);
  string extracted_image;
  if (res != CURLE_OK)
    cerr << "cURL request failed: " << curl_easy_strerror(res) << endl;
  else {

    try {
      // Parse JSON response
      json jsonResponse = json::parse(response_data);

      // Extract the "response" field
      if (jsonResponse.contains("result") &&
          jsonResponse["result"].contains("image")) {
        extracted_image = jsonResponse["result"]["image"].get<string>();
        // Remove surrounding quotes if they exist
        if (!extracted_image.empty() && extracted_image.front() == '"' &&
            extracted_image.back() == '"') {
          extracted_image =
              extracted_image.substr(1, extracted_image.size() - 2);
        }
      } else {
        cout << "Response field not found in JSON!" << endl;
        cout << jsonResponse << endl;
      }
    } catch (json::parse_error &e) {
      cerr << "JSON parsing error: " << e.what() << endl;
    }
  }
  curl_easy_cleanup(curl);
  curl_slist_free_all(headers);

  vector<unsigned char> decoded = base64_rfc4648::decode(extracted_image);
  std::string baseName = "./output/result";
  std::string extension = ".jpg";
  int count = 1;

  // Find the next available filename
  std::string filename;
  do {
    std::ostringstream oss;
    oss << baseName << count << extension;
    filename = oss.str();
    count++;
  } while (std::filesystem::exists(filename));

  if (!decoded.empty()) {
    ofstream file(filename, ios::binary);
    file.write(reinterpret_cast<const char *>(decoded.data()), decoded.size());
  } else {
    cerr << "Failed to decode Base64" << endl;
  }
  return filename;
}

string post_to_fb(string generated_img, string page_id,
                  string fb_access_token) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    cerr << "Failed to initialize cURL" << endl;
    return "";
  }

  string date = "(" + get_date() + ")";
  string response_data;
  string url = "https://graph.facebook.com/v22.0/" + page_id + "/photos";
  string caption =
      "Family Island Free Energy ⚡🎁\n" + date + "\nhttps://gogl.to/3GEF ✅";

  curl_mime *form = curl_mime_init(curl);
  curl_mimepart *field;

  // Add access token - FIXED: Don't include "access_token=" prefix
  field = curl_mime_addpart(form);
  curl_mime_name(field, "access_token");
  curl_mime_data(field, fb_access_token.c_str(), CURL_ZERO_TERMINATED);

  // Add image file
  field = curl_mime_addpart(form);
  curl_mime_name(field, "source");
  curl_mime_filedata(field, generated_img.c_str());

  // Add caption
  field = curl_mime_addpart(form);
  curl_mime_name(field, "caption");
  curl_mime_data(field, caption.c_str(), CURL_ZERO_TERMINATED);

  // Set up the request
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

  string post_id;
  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    cerr << "cURL request failed: " << curl_easy_strerror(res) << endl;
  } else {
    try {
      json jsonData = json::parse(response_data);
      if (jsonData.contains("id")) { // Changed from "post_id" to "id"
        post_id = jsonData["id"];
      } else if (jsonData.contains("error")) {
        cerr << "Facebook API error: " << jsonData["error"]["message"] << endl;
      }
    } catch (json::parse_error &e) {
      cerr << "JSON parsing error: " << e.what() << endl;
    }
  }

  curl_mime_free(form);
  curl_easy_cleanup(curl);
  return post_id;
}

string generateImageFromImage(string img, string prompt) {
  // Debug: Input validation
  cout << "Debug: Starting image generation with input path: " << img << endl;
  cout << "Debug: Prompt: " << prompt << endl;

  if (img.empty() || prompt.empty()) {
    cerr << "Error: Empty input image path or prompt" << endl;
    return "";
  }

  // Check if input file exists
  if (!filesystem::exists(img)) {
    cerr << "Error: Input image file does not exist: " << img << endl;
    return "";
  }

  string base64_image;
  try {
    std::string image_path = img;
    base64_image = image_to_base64(image_path);

    std::cout << "Base64 Encoded Image:\n" << base64_image << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  CURL *curl = curl_easy_init();
  if (!curl) {
    cerr << "Failed to initialize cURL" << endl;
    return "";
  }

  // Debug: API configuration
  cout << "Debug: Using Cloudflare account ID: " << CLOUDFLARE_ACCOUNT_ID
       << endl;
  cout << "Debug: Auth token length: " << CLOUDFLARE_AUTH_TOKEN.length()
       << endl;

  string response_data;
  string url = "https://api.cloudflare.com/client/v4/accounts/" +
               CLOUDFLARE_ACCOUNT_ID +
               "/ai/run/@cf/runwayml/stable-diffusion-v1-5-img2img";

  // Debug: Request preparation
  json payload = {{"prompt", prompt}, {"image_b64", base64_image}};
  string json_str = payload.dump();
  cout << "Debug: Payload size: " << json_str.length() << " bytes" << endl;
  cout << "Debug: JSON structure valid: "
       << (json::accept(json_str) ? "yes" : "no") << endl;

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(
      headers, ("Authorization: Bearer " + CLOUDFLARE_AUTH_TOKEN).c_str());

  // Debug: cURL verbose output
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

  // Add timeout to prevent hanging
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

  // Debug: Pre-request state
  cout << "Debug: Sending request to URL: " << url << endl;

  CURLcode res = curl_easy_perform(curl);

  // Debug: Response information
  long http_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  cout << "Debug: HTTP response code: " << http_code << endl;

  if (res != CURLE_OK) {
    cerr << "cURL request failed: " << curl_easy_strerror(res) << endl;
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return "";
  }

  // Debug: Response validation
  cout << "Debug: Response data size: " << response_data.size() << " bytes"
       << endl;
  if (response_data.empty()) {
    cerr << "Error: Empty response received" << endl;
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return "";
  }

  // Debug: Response content check
  cout << "Debug: First 50 bytes of response: ";
  for (size_t i = 0; i < min(size_t(50), response_data.size()); i++) {
    cout << hex << setw(2) << setfill('0')
         << (int)(unsigned char)response_data[i];
  }
  cout << dec << endl;

  // Save the image
  string baseName = "./output/result";
  string extension = ".png";
  int count = 1;
  string filename;

  // Debug: Output directory check
  if (!filesystem::exists("./output")) {
    cout << "Debug: Creating output directory" << endl;
    filesystem::create_directory("./output");
  }

  do {
    ostringstream oss;
    oss << baseName << count << extension;
    filename = oss.str();
    count++;
  } while (filesystem::exists(filename));

  // Debug: File writing
  cout << "Debug: Writing to file: " << filename << endl;

  ofstream file(filename, ios::binary);
  if (!file.is_open()) {
    cerr << "Error: Could not open output file for writing" << endl;
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return "";
  }

  file.write(response_data.c_str(), response_data.size());
  file.close();

  // Debug: Output file verification
  if (filesystem::exists(filename)) {
    cout << "Debug: Output file created successfully. Size: "
         << filesystem::file_size(filename) << " bytes" << endl;
  } else {
    cerr << "Error: Failed to create output file" << endl;
  }

  curl_easy_cleanup(curl);
  curl_slist_free_all(headers);
  return filename;
}
