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
      printf ("Usage: ./atbcaptcha \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   api2captcha::captcha_t cap;
   cap.set_param("method", "atb_captcha");
   cap.set_param("app_id", "af23e041b22d000a11e22a230fa7777c");
   cap.set_param("api_server", "https://cap.aisecurius.com");
   cap.set_param("pageurl", "https://mysite.com/page/with/atbcaptcha");

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
