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

   api2captcha::lemin lemin;
   lemin.setCaptchaid ("CROPPED_d3d4d56_73ca4008925b4f83a8bed59c2dd0df6d");
   lemin.setUrl ("http://sat2.aksigorta.com.tr");
   lemin.setApiServer("api.leminnow.com");

   try
   {
      client.solve (lemin);
      printf ("code '%s'\n", lemin.code ().c_str ());
   }
   catch (std::exception & e)
   {
      fprintf (stderr, "Failed: %s\n", e.what ());
   }

   return 0;   
}