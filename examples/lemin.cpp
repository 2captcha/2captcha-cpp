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
   lemin.setCaptchaid ("CROPPED_5a29582_ca114c2f3314482c84cd32fc7d2feb63");
   lemin.setUrl ("https://2captcha.com/demo/lemin");
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
