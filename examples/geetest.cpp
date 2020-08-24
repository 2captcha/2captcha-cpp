#include <cstdio>

#include "curl_http.hpp"
#include "api2captcha.hpp"

int main (int ac, char ** av)
{
   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (API_KEY);

   api2captcha::geetest_t cap;
   cap.set_gt ("f2ae6cadcf7886856696502e1d55e00c");
   cap.set_api_server ("api-na.geetest.com");
   cap.set_challenge ("12345678abc90123d45678ef90123a456b");
   cap.set_url ("https://mysite.com/captcha.html");

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
