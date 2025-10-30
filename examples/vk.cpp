#include <cstdio>
#include <filesystem>
//#include <iostream>

#include "curl_http.hpp"
#include "api2captcha.hpp"

using namespace std;

int main (int ac, char ** av)
{
   printf ("Current path is : '%s'\n", filesystem::current_path().c_str ());

  /*
   cout << "Current path is " << filesystem::current_path()
         << endl;
*/

   if (ac < 3)
   {
      printf ("Usage: ./vk path/to/image.jpg \"steps\"\n");
      return 0;
   }
   
   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (API_KEY);

   assert (ac > 2);

   api2captcha::canvas_t cap;
   cap.set_file (av[1]);
   cap.set_hint_text (av[2]);

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
