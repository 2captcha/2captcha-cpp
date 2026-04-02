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
      printf ("Usage: ./altchacaptcha \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   api2captcha::altchacaptcha_t cap;
   cap.set_challenge_url("https://.../captcha/api/altcha/challenge");
   cap.set_page_url ("https://site.com/");

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
