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
      printf("Usage: ./vk \"API KEY\"\n");
      return 0;
   }

   string current_path = filesystem::current_path().c_str();
   string bodyFilePath = current_path + "/images/temu/body.png";
   string part1FilePath = current_path + "/images/temu/part1.png";
   string part2FilePath = current_path + "/images/temu/part2.png";
   string part3FilePath = current_path + "/images/temu/part3.png";

   api2captcha::curl_http_t http;
   http.set_verbose(true);

   api2captcha::client_t client;
   client.set_http_client(&http);
   client.set_api_key(av[1]);

   std::string method = "temuimage";
   const char *c_strMethod = method.c_str();

   api2captcha::temu_t cap(c_strMethod);

   const std::vector<unsigned char> bodyData = api2captcha::FileUtils::readFile(bodyFilePath);
   const std::vector<unsigned char> part1Data = api2captcha::FileUtils::readFile(part1FilePath);
   const std::vector<unsigned char> part2Data = api2captcha::FileUtils::readFile(part2FilePath);
   const std::vector<unsigned char> part3Data = api2captcha::FileUtils::readFile(part3FilePath);

   cap.set_body(api2captcha::B64::base64_encode(bodyData));
   cap.set_part1(api2captcha::B64::base64_encode(part1Data));
   cap.set_part2(api2captcha::B64::base64_encode(part2Data));
   cap.set_part3(api2captcha::B64::base64_encode(part3Data));

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
