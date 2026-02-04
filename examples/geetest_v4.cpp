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
      printf ("Usage: ./geetest_v4 \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   std::string method = "geetest_v4";
   const char * c_strMethod = method.c_str();

   api2captcha::geetest_v4_t cap (c_strMethod);
   cap.set_captcha_id("2d9c743cf7d63dbc9db578a608196bcd");
   cap.set_pageurl("https://m.avito.ru/all/avtomobili?context=H4sIAAAAAAAA_0q0MrKqLraysFJKK8rPDUhMT1WyLrYyNLVSKs7ILypJLi0pDs9MSU8tAQubWCllJBa7VpQUJQYkFiXmFitZJ1kZWtcCAgAA__-ChIjFRgAAAA&radius=0&presentationType=serp");

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