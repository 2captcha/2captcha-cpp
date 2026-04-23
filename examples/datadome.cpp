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
      printf ("Usage: ./datadome \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   api2captcha::captcha_t cap;

   cap.set_param("method", "datadome");
   cap.set_param("captcha_url", "https://geo.captcha-delivery.com/captcha/?initialCid=AHrlqAAA...");
   cap.set_param("pageurl", "https://example.com/");
   cap.set_param("userAgent", "Mozilla/5.0 (Linux; Android 10; K) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Mobile Safari/537.3");
   cap.set_param("proxytype", "HTTPS");
   cap.set_param("proxy", "login:password@IP_address:PORT");

   
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
