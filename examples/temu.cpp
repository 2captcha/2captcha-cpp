#include <cstdio>
#include <filesystem>
#include <iostream>

#include <fstream>
#include <vector>
#include <string>

#include "curl_http.hpp"
#include "api2captcha.hpp"
#include "b64.hpp"

using namespace std;


   //**********************************// encode to b64 //************************************/
   string getFile2Base64(std::string &image_path) {
    //const std::string image_path = "/home/victor/Workspace/Rucaptcha/projects/C++/2captcha-cpp/examples/images/temu/body.png"; // Replace with your image file path

    // 1. Read the image file into a byte vector
    std::ifstream file(image_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening image file: " << image_path << std::endl;
        return "";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> image_data(size);
    if (!file.read(reinterpret_cast<char*>(image_data.data()), size)) {
        std::cerr << "Error reading image file: " << image_path << std::endl;
        return "";
    }

    // 2. Encode the image data to Base64
    api2captcha::B64 b64;
    std::string base64_encoded_image = b64.base64_encode(image_data);

    // 3. Print the Base64 string
    //std::cout << "Base64 encoded image:\n" << base64_encoded_image << std::endl;

    return base64_encoded_image;
}
   //---------------


int main (int ac, char ** av)
{
   if (ac < 2)
   {
      printf ("Usage: ./vk \"API KEY\"\n");
      return 0;
   }

   string current_path = filesystem::current_path().c_str();
   string bodyFilePath = current_path + "/images/temu/body.png";
   string part1FilePath = current_path + "/images/temu/part1.png";
   string part2FilePath = current_path + "/images/temu/part2.png";
   string part3FilePath = current_path + "/images/temu/part3.png";

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   std::string method = "temuimage";
   const char *c_strMethod = method.c_str();

   api2captcha::temu_t cap (c_strMethod);

   string bodyB64Str = getFile2Base64(bodyFilePath);
   string part1B64Str = getFile2Base64(part1FilePath);
   string part2B64Str = getFile2Base64(part2FilePath);
   string part3B64Str = getFile2Base64(part3FilePath);

   cap.set_body(bodyB64Str);
   cap.set_part1(part1B64Str);
   cap.set_part2(part2B64Str);
   cap.set_part3(part3B64Str);

   try
   {
      client.solve (cap);
      printf ("code '%s'\n", cap.code ().c_str ());
   }
   catch (std::exception & e)
   {
      fprintf (stderr, "Failed: %s\n", e.what ());
   }

   return 0;   
}
