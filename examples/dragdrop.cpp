#include <cstdio>
#include <string>
#include <vector>
#include <filesystem>

#include "curl_http.hpp"
#include "api2captcha.hpp"
#include "b64.hpp"
#include "file_utils.hpp"

using namespace std;

int main(int ac, char **av)
{
   if (ac < 2)
   {
      printf("Usage: ./dragdrop \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose(true);

   api2captcha::client_t client;
   client.set_http_client(&http);
   client.set_api_key(av[1]);

   api2captcha::dragdrop_t cap;

   string current_path = filesystem::current_path().c_str();
   string backgroundFilePath = current_path + "/assets/dragdrop/background.jpeg";
   string image1FilePath = current_path + "/assets/dragdrop/image1.jpeg";
   string image2FilePath = current_path + "/assets/dragdrop/image2.jpeg";

   const vector<unsigned char> bodyData = api2captcha::FileUtils::readFile(backgroundFilePath);
   cap.set_body(api2captcha::B64::base64_encode(bodyData));

   vector<string> images;

   const vector<unsigned char> image1Data = api2captcha::FileUtils::readFile(image1FilePath);
   const vector<unsigned char> image2Data = api2captcha::FileUtils::readFile(image2FilePath);
   images.push_back(api2captcha::B64::base64_encode(image1Data));
   images.push_back(api2captcha::B64::base64_encode(image2Data));

   cap.set_images(images);

   cap.set_hint_text("Drag the images to proper position");

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
