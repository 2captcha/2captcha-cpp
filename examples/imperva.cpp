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
      printf ("Usage: ./imperva \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   api2captcha::imperva_t cap;
   cap.set_url ("https://example.com/login");
   cap.set_incapsula_script_url ("/_Incapsula_Resource?SWUDNSAI=31&xinfo=1");
   cap.set_incapsula_cookies ("visid_incap_123456=sample; incap_ses_123_123456=sample");
   cap.set_reese84_url_endpoint ("https://example.com/_Incapsula_Resource?reese84=1");
   cap.set_user_agent ("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
   cap.set_proxy ("HTTP", "login:password@IP_address:PORT");

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
