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
      printf ("Usage: ./yidun \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   api2captcha::yidun_t cap;
   cap.set_site_key ("SITE_KEY");
   cap.set_url ("https://example.com/page-with-yidun");
   cap.set_user_agent ("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/132.0.0.0 Safari/537.36");
   cap.set_proxy ("HTTPS", "login:password@IP_address:PORT");

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
