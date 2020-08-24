#ifndef CURL_HTTP_HPP
#define CURL_HTTP_HPP

#include <cassert>

#include <curl/curl.h>

#include "api2captcha.hpp"

namespace api2captcha
{

class curl_http_t : public http_client_i
{
   CURL * curl_ = nullptr;
   std::string result_;
   bool verbose_ = false;

   static size_t
   on_data (void * contents, 
	    const size_t size, const size_t nmemb, 
	    void * userp)
   {
      assert (userp);

      // get curl
      auto * self = reinterpret_cast<curl_http_t *> (userp);
      assert (self);

      // push data
      const size_t total = size * nmemb;
      self->result_.append ((char *)contents, total);

      // always confirm receival
      return total;
   }
   
public:

   curl_http_t ()
   {
      if (curl_global_init (CURL_GLOBAL_ALL))
	 throw std::runtime_error ("Failed to global init curl");
      
      curl_ = curl_easy_init ();
      if (!curl_)
      {
	 curl_global_cleanup ();
	 throw std::runtime_error ("Failed to init curl");
      }
   }

   ~curl_http_t ()
   {
      if (curl_)
	 curl_easy_cleanup (curl_);
	 
      curl_global_cleanup ();
   }

   void set_verbose (const bool v) { verbose_ = v; }
   
   virtual int status () const
   {
      long status = 0;
      curl_easy_getinfo (curl_, CURLINFO_RESPONSE_CODE, &status);
      return status;
   }
   
   virtual std::string get (
      const std::string & url, const int timeout, const params_t & params)
   {
      result_.clear ();
      curl_easy_reset (curl_);

      // format url
      std::string full_url = url + "?";
      for (const auto & i: params)
      {
	 auto * v = curl_easy_escape (curl_, i.second.c_str (), i.second.size ());
	 assert (v);
	 full_url += "&" + i.first + "=" + v;	 
	 curl_free (v);
      }

      // set url
      curl_easy_setopt (curl_, CURLOPT_URL, full_url.c_str ());
      
      // timeouts
      curl_easy_setopt (curl_, CURLOPT_TIMEOUT, timeout);

      // set self as on_data context
      curl_easy_setopt (curl_, CURLOPT_WRITEDATA, this);

      // data callback
      curl_easy_setopt (curl_, CURLOPT_WRITEFUNCTION, on_data);

      if (verbose_)
	 curl_easy_setopt (curl_, CURLOPT_VERBOSE, 1L);
      
      /* Perform the request, res will get the return code */ 
      const auto res = curl_easy_perform (curl_); 
      if (res)
	 throw network_exception_t (
	    std::string ("Failed to curl get: ") + curl_easy_strerror (res));
      
      return result_;
   }
   
   virtual std::string post (
      const std::string & url, const int timeout,
      const params_t & params, const files_t & files)
   {
      result_.clear ();
      curl_easy_reset (curl_);
      
      // Create the form
      auto * form = curl_mime_init (curl_);
      if (!form)
	 throw std::runtime_error ("Failed to create curl mime");

      // params
      for (const auto & i: params)
      {
	 auto * field = curl_mime_addpart (form);
	 curl_mime_name (field, i.first.c_str ());
	 curl_mime_data (field, i.second.c_str (), CURL_ZERO_TERMINATED);
      }
      
      // files
      for (const auto & i: files)
      {
	 auto * field = curl_mime_addpart (form);
	 curl_mime_name (field, i.first.c_str ());
	 if (curl_mime_filedata (field, i.second.c_str ()))
	    throw std::runtime_error ("Failed to set uploaded file in curl");
      }
 
      // set url
      curl_easy_setopt (curl_, CURLOPT_URL, url.c_str ());
    
      //  initialize custom header list (stating that Expect: 100-continue is not wanted
      // FIXME???
      auto * headerlist = curl_slist_append (NULL, "Expect:");
      curl_easy_setopt (curl_, CURLOPT_HTTPHEADER, headerlist);
      
      curl_easy_setopt (curl_, CURLOPT_MIMEPOST, form);
      
      // timeouts
      curl_easy_setopt (curl_, CURLOPT_TIMEOUT, timeout);

      // set self as on_data context
      curl_easy_setopt (curl_, CURLOPT_WRITEDATA, this);

      // data callback
      curl_easy_setopt (curl_, CURLOPT_WRITEFUNCTION, on_data);

      if (verbose_)
	 curl_easy_setopt (curl_, CURLOPT_VERBOSE, 1L);
      
      /* Perform the request, res will get the return code */ 
      const auto res = curl_easy_perform (curl_);
 
      curl_mime_free (form);
      curl_slist_free_all (headerlist);

      if (res)
	 throw network_exception_t (
	    std::string ("Failed to curl post: ") + curl_easy_strerror (res));
      
      return result_;
   }
   
};
   
};

#endif /* CURL_HTTP_HPP */
