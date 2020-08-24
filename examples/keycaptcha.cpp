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

   api2captcha::keycaptcha_t cap;
   cap.set_user_id (10);
   cap.set_session_id ("493e52c37c10c2bcdf4a00cbc9ccd1e8");
   cap.set_web_server_sign ("9006dc725760858e4c0715b835472f22");
   cap.set_web_server_sign2 ("2ca3abe86d90c6142d5571db98af6714");
   cap.set_url ("https://www.keycaptcha.ru/demo-magnetic/");

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
