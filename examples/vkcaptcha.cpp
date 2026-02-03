#include <cstdio>
#include <filesystem>
#include <iostream>

#include "curl_http.hpp"
#include "api2captcha.hpp"

using namespace std;

int main (int ac, char ** av)
{
   if (ac < 2)
   {
      printf ("Usage: ./vkcaptcha \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);


   std::string method = "vkcaptcha";
   const char *c_strMethod = method.c_str();

   api2captcha::vk_t cap (c_strMethod);
        cap.set_redirect_uri("https://id.vk.com/not_robot_captcha?domain=vk.com&session_token=eyJ....HGsc5B4LyvjA&variant=popup&blank=1");
        cap.set_user_agent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36");
        cap.set_proxy("http", "1.2.3.4");
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
