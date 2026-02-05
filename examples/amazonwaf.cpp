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
      printf ("Usage: ./amazonwaf \"API KEY\"\n");
      return 0;
   }

   api2captcha::curl_http_t http;
   http.set_verbose (true);

   api2captcha::client_t client;
   client.set_http_client (&http);
   client.set_api_key (av[1]);

   std::string method = "amazon_waf";
   const char * c_strMethod = method.c_str();

   api2captcha::amazonwaf_t cap (c_strMethod);
   cap.set_site_key("AQIDAHjcYu/GjX+QlghicBgQ/7bFaQZ+m5FKCMDnO");
   cap.set_pageurl("https://non-existent-example.execute-api.us-east-1.amazonaws.com");
   cap.set_iv("test_iv");
   cap.set_context("test_context");

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