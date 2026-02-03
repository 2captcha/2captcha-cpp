#include <cstdio>
#include <filesystem>
#include <iostream>

#include <fstream>
#include <vector>
#include <string>

#include "curl_http.hpp"
#include "api2captcha.hpp"
#include "b64.hpp"
#include "file_utils.hpp"

using namespace std;

int main(int ac, char **av)
{
   if (ac < 2)
   {
      printf("Usage: ./audio \"API KEY\"\n");
      return 0;
   }

   string current_path = filesystem::current_path().c_str();
   string bodyFilePath = current_path + "/assets/audio/audio-en.mp3";

   api2captcha::curl_http_t http;
   http.set_verbose(true);

   api2captcha::client_t client;
   client.set_http_client(&http);
   client.set_api_key(av[1]);

   const vector<unsigned char> bodyData = api2captcha::FileUtils::readFile(bodyFilePath);

   api2captcha::audio_t cap;

   cap.set_body(api2captcha::B64::base64_encode(bodyData));
   cap.set_lang("en");

   try
   {
      client.solve(cap);
      printf("code '%s'\n", cap.code().c_str());
   }
   catch (std::exception &e)
   {
      fprintf(stderr, "Failed: %s\n", e.what());
   }

   return 0;
}
