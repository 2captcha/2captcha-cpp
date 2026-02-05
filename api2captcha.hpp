#ifndef API2CAPTCHA_HPP
#define API2CAPTCHA_HPP

#include <cassert>

#include <string>
#include <map>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <vector>

namespace api2captcha
{

   typedef std::map<std::string, std::string> params_t;
   typedef std::map<std::string, std::string> files_t;

   class exception_t : public std::runtime_error
   {
   public:
      exception_t(const std::string &s) : std::runtime_error(s) {}
   };

   class api_exception_t : public exception_t
   {
   public:
      api_exception_t(const std::string &s) : exception_t(s) {}
   };

   class timeout_exception_t : public exception_t
   {
   public:
      timeout_exception_t(const std::string &s) : exception_t(s) {}
   };

   class network_exception_t : public exception_t
   {
   public:
      network_exception_t(const std::string &s) : exception_t(s) {}
   };

   struct http_client_i
   {
      virtual ~http_client_i() {};
      virtual int status() const = 0;
      virtual std::string get(
          const std::string &url, const int timeout, const params_t &params) = 0;
      virtual std::string post(
          const std::string &url, const int timeout,
          const params_t &params, const files_t &files) = 0;
   };

   class captcha_t
   {
      std::string id_;
      std::string code_;

      mutable params_t params_;
      files_t files_;

   protected:
      void set_param(const std::string &name, const std::string &value)
      {
         params_[name] = value;
      }

      void set_file(const std::string &name, const std::string &value)
      {
         files_[name] = value;
      }

      captcha_t() {};

   public:
      // make polymorphic
      virtual ~captcha_t() {}

      const std::string &id() const { return id_; }
      const std::string &code() const { return code_; }

      void set_id(const std::string &s) { id_ = s; }
      void set_code(const std::string &s) { code_ = s; }

      void set_user_agent(const std::string &s) 
      {
         params_["userAgent"] = s;
      };

      void set_proxy(const std::string &type, const std::string &url)
      {
         params_["proxytype"] = type;
         params_["proxy"] = url;
      }

      void set_soft_id(const int soft_id) { params_["soft_id"] = std::to_string(soft_id); }
      void set_callback(const std::string &url) { params_["pingback"] = url; }

      const params_t &params() const
      {
         if (params_["method"].empty())
         {
            if (!params_["body"].empty())
               params_["method"] = "base64";
            else
               params_["method"] = "post";
         }
         return params_;
      }

      const files_t &files() const { return files_; }
   };

   class canvas_t : public captcha_t
   {
   public:
      canvas_t()
      {
         set_param("canvas", "1");
         set_param("recaptcha", "1");
      }

      void set_file(const std::string &path) { captcha_t::set_file("file", path); }
      void set_base64(const std::string &base64) { set_param("body", base64); }
      void set_previous_id(const int id) { set_param("previousID", std::to_string(id)); }
      void set_can_skip(const bool can) { set_param("can_no_answer", can ? "1" : "0"); }
      void set_lang(const std::string &lang) { set_param("lang", lang); }
      void set_hint_text(const std::string &text) { set_param("textinstructions", text); }
      void set_hint_img(const std::string &base64) { set_param("imginstructions", base64); }
      void set_hint_img_file(const std::string &path) { captcha_t::set_file("imginstructions", path); }
   };

   class capy_t : public captcha_t
   {
   public:
      capy_t()
      {
         set_param("method", "capy");
      }

      void set_site_key(const std::string &s) { set_param("captchakey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
      void set_api_server(const std::string &s) { set_param("api_server", s); };
   };

   class coordinates_t : public captcha_t
   {
   public:
      coordinates_t()
      {
         set_param("coordinatescaptcha", "1");
      }

      coordinates_t(const std::string &path)
          : coordinates_t()
      {
         set_file(path);
      }

      void set_file(const std::string &path) { captcha_t::set_file("file", path); };
      void set_base64(const std::string &base64) { set_param("body", base64); }
      void set_lang(const std::string &lang) { set_param("lang", lang); }
      void set_hint_text(const std::string &text) { set_param("textinstructions", text); }
      void set_hint_img(const std::string &base64) { set_param("imginstructions", base64); }
      void set_hint_img_file(const std::string &path) { captcha_t::set_file("imginstructions", path); }
   };

   class funcaptcha_t : public captcha_t
   {
   public:
      funcaptcha_t()
      {
         set_param("method", "funcaptcha");
      }

      void set_site_key(const std::string &s) { set_param("publickey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
      void set_surl(const std::string &s) { set_param("surl", s); };
      void set_user_agent(const std::string &s) { set_param("userAgent", s); };
      void set_data(const std::string &key, const std::string &value)
      {
         set_param("data[" + key + "]", value);
      }
   };

   class amazonwaf_t : public captcha_t
   {
   public:
      amazonwaf_t(const std::string &method)
      {
         set_param("method", method);
      }

      void set_site_key(const std::string &s) { set_param("sitekey", s); };
      void set_pageurl(const std::string &pageurl) { set_param("pageurl", pageurl); };
      void set_iv(const std::string &iv) { set_param("iv", iv); };
      void set_context(const std::string &context) { set_param("context", context); };

      void set_challenge_script(const std::string &challenge_script) { set_param("challenge_script", challenge_script); };
      void set_captcha_script(const std::string &captcha_script) { set_param("captcha_script", captcha_script); };
      void set_header_acao(const int header_acao) { set_param("header_acao", std::to_string(header_acao)); }      
   };

   class geetest_v4_t : public captcha_t
   {
   public:
      geetest_v4_t(const std::string &method)
      {
         set_param("method", method);
      }

      void set_captcha_id(const std::string &captcha_id) { set_param("captcha_id", captcha_id); };
      void set_risk_type(const std::string &risk_type) { set_param("risk_type", risk_type); };
      void set_header_acao(const int header_acao) { set_param("header_acao", std::to_string(header_acao)); } 
      void set_pingback(const std::string &pingback) { set_param("pingback", pingback); };
      void set_pageurl(const std::string &pageurl) { set_param("pageurl", pageurl); };
   };

   class geetest_t : public captcha_t
   {
   public:
      geetest_t()
      {
         set_param("method", "geetest");
      }

      void set_gt(const std::string &s) { set_param("gt", s); };
      void set_challenge(const std::string &s) { set_param("challenge", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
      void set_api_server(const std::string &s) { set_param("api_server", s); };
   };

   class grid_t : public captcha_t
   {
   public:
      grid_t() {}

      grid_t(const std::string &path)
      {
         set_file(path);
      }

      void set_file(const std::string &path) { captcha_t::set_file("file", path); };
      void set_base64(const std::string &base64) { set_param("body", base64); }
      void set_rows(const int rows) { set_param("recaptcharows", std::to_string(rows)); }
      void set_cols(const int cols) { set_param("recaptchacols", std::to_string(cols)); }
      void set_previous_id(const int id) { set_param("previousID", std::to_string(id)); }
      void set_can_skip(const bool can) { set_param("can_no_answer", can ? "1" : "0"); }
      void set_lang(const std::string &lang) { set_param("lang", lang); }
      void set_hint_text(const std::string &text) { set_param("textinstructions", text); }
      void set_hint_img(const std::string &base64) { set_param("imginstructions", base64); }
      void set_hint_img_file(const std::string &path) { captcha_t::set_file("imginstructions", path); }
   };

   class hcaptcha_t : public captcha_t
   {
   public:
      hcaptcha_t()
      {
         set_param("method", "hcaptcha");
      }

      void set_site_key(const std::string &s) { set_param("sitekey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
   };

   class keycaptcha_t : public captcha_t
   {
   public:
      keycaptcha_t()
      {
         set_param("method", "keycaptcha");
      }

      void set_user_id(const int user_id) { set_user_id(std::to_string(user_id)); };
      void set_user_id(const std::string &user_id) { set_param("s_s_c_user_id", user_id); };
      void set_session_id(const std::string &session_id)
      {
         set_param("s_s_c_session_id", session_id);
      }
      void set_web_server_sign(const std::string &sign)
      {
         set_param("s_s_c_web_server_sign", sign);
      }
      void set_web_server_sign2(const std::string &sign)
      {
         set_param("s_s_c_web_server_sign2", sign);
      }
      void set_url(const std::string &s) { set_param("pageurl", s); };
   };

   class normal_t : public captcha_t
   {
   public:
      normal_t() {}

      normal_t(const std::string &path)
      {
         set_file(path);
      }

      void set_file(const std::string &path) { captcha_t::set_file("file", path); };
      void set_base64(const std::string &base64) { set_param("body", base64); }

      void set_phrase(const bool ph) { set_param("phrase", ph ? "1" : "0"); }
      void set_case_sensitive(const bool cs) { set_param("regsense", cs ? "1" : "0"); }
      void set_calc(const bool calc) { set_param("calc", calc ? "1" : "0"); }
      void set_numeric(const int n) { set_param("numeric", std::to_string(n)); }
      void set_min_len(const int l) { set_param("min_len", std::to_string(l)); }
      void set_max_len(const int l) { set_param("max_len", std::to_string(l)); }

      void set_lang(const std::string &lang) { set_param("lang", lang); }
      void set_hint_text(const std::string &text) { set_param("textinstructions", text); }
      void set_hint_img(const std::string &base64) { set_param("imginstructions", base64); }
      void set_hint_img_file(const std::string &path) { captcha_t::set_file("imginstructions", path); }
   };

   class recaptcha_t : public captcha_t
   {
   public:
      recaptcha_t()
      {
         set_param("method", "userrecaptcha");
      }

      void set_site_key(const std::string &s) { set_param("googlekey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
      void set_invisible(const bool inv) { set_param("invisible", inv ? "1" : "0"); }
      void set_version(const std::string &s) { set_param("version", s); };
      void set_action(const std::string &s) { set_param("action", s); };
      void set_score(const double s) { set_param("min_score", std::to_string(s)); };
   };

   class rotate_t : public captcha_t
   {
   public:
      rotate_t()
      {
         set_param("method", "rotatecaptcha");
      }

      rotate_t(const std::string &path)
          : rotate_t()
      {
         set_file(path);
      }

      rotate_t(const std::vector<std::string> &paths)
          : rotate_t()
      {
         set_files(paths);
      }

      void set_file(const std::string &path) { captcha_t::set_file("file_1", path); };
      void set_files(const std::vector<std::string> &paths)
      {
         for (size_t i = 0; i < paths.size(); ++i)
            captcha_t::set_file("file_" + std::to_string(i + 1), paths[i]);
      }

      void set_angle(const int a) { set_param("angle", std::to_string(a)); }

      void set_lang(const std::string &lang) { set_param("lang", lang); }
      void set_hint_text(const std::string &text) { set_param("textinstructions", text); }
      void set_hint_img(const std::string &base64) { set_param("imginstructions", base64); }
      void set_hint_img_file(const std::string &path) { captcha_t::set_file("imginstructions", path); }
   };

   class text_t : public captcha_t
   {
   public:
      text_t()
      {
         set_param("method", "post");
      }
      text_t(const std::string &text)
          : text_t()
      {
         set_text(text);
      }

      void set_text(const std::string &s) { set_param("textcaptcha", s); };
      void set_lang(const std::string &s) { set_param("lang", s); };
   };

   class lemin : public captcha_t
   {
   public:
      lemin()
      {
         set_param("method", "lemin");
      }

      void setApiServer(const std::string &s) { set_param("api_server", s); };
      void setCaptchaid(const std::string &s) { set_param("captcha_id", s); };
      void setUrl(const std::string &s) { set_param("pageurl", s); };
   };

   class prosopo_t : public captcha_t
   {
   public:
      prosopo_t(){
         set_param("method", "prosopo");
      }

      void set_site_key(const std::string &s) { set_param("sitekey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
   };

   class captchafox_t : public captcha_t
   {
   public:
      captchafox_t(){
         set_param("method", "captchafox");
      }

      void set_site_key(const std::string &s) { set_param("sitekey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
   };

   class temu_t : public captcha_t
   {
   public:
      temu_t(const std::string &method)
      {
         set_param("method", method);
      }

      void set_file(const std::string &path) { captcha_t::set_file("file", path); }
      void set_base64(const std::string &base64) { set_param("body", base64); }
      void set_steps(const std::string &steps) { set_param("steps", steps); }
      void set_redirect_uri(const std::string &redirect_uri) { set_param("redirect_uri", redirect_uri); }
      void set_user_agent(const std::string &user_agent) { set_param("userAgent", user_agent); }
      void set_body(const std::string &b64) { captcha_t::set_param("body", b64); }
      void set_part1(const std::string &b64) { captcha_t::set_param("part1", b64); }
      void set_part2(const std::string &b64) { captcha_t::set_param("part2", b64); }
      void set_part3(const std::string &b64) { captcha_t::set_param("part3", b64); }
   };

   class audio_t : public captcha_t
   {
   public:
      audio_t()
      {
         set_param("method", "audio");
      }

      void set_body(const std::string &b64) { captcha_t::set_param("body", b64); }
      void set_lang(const std::string &lang) { captcha_t::set_param("lang", lang); }
   };

   class yandex_t : public captcha_t
   {
   public:
      yandex_t(){
         set_param("method", "yandex");
      }

      void set_site_key(const std::string &s) { set_param("sitekey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
   };

   class turnstile_t : public captcha_t
   {
   public:
      turnstile_t(){
         set_param("method", "turnstile");
      }

      void set_site_key(const std::string &s) { set_param("sitekey", s); };
      void set_url(const std::string &s) { set_param("pageurl", s); };
   };


   class vk_t : public captcha_t
   {
   public:
      vk_t(const std::string &method)
      {
         set_param("method", method);
      }

      void set_file(const std::string &path) { captcha_t::set_file("file", path); }
      void set_base64(const std::string &base64) { set_param("body", base64); }
      void set_steps(const std::string &steps) { set_param("steps", steps); }
      void set_redirect_uri(const std::string &redirect_uri) { set_param("redirect_uri", redirect_uri); }
      void set_user_agent(const std::string &user_agent) { set_param("userAgent", user_agent); }
   };

   class client_t
   {
      class impl_t
      {
         static const constexpr char *HOST = "2captcha.com";

         std::string api_key_;
         int soft_id_ = 4586;
         std::string callback_;
         int default_timeout_ = 120;    // sec
         int recaptcha_timeout_ = 600; // sec
         int polling_interval_ = 10;   // sec
         bool last_captcha_has_callback_ = false;
         http_client_i *http_ = nullptr;

         void check_http_response(const std::string &r)
         {
            const auto status = http_->status();
            if (status == 0)
               throw network_exception_t("Failed to send captcha");
            if (status != 200)
               throw api_exception_t("Bad captcha api status: " + std::to_string(status));
            if (r.substr(0, 6) == "ERROR_")
               throw api_exception_t("Captcha api error: " + r);
         }

         std::string do_http_in(
             const params_t &params,
             const files_t &files,
             const int timeout)
         {
            assert(is_valid());
            const auto url = std::string("https://") + HOST + "/in.php";
            const auto response = http_->post(url, timeout, params, files);
            check_http_response(response);
            return response;
         }

         std::string do_http_res(
             const params_t &params, const int timeout)
         {
            assert(is_valid());
            const auto url = std::string("https://") + HOST + "/res.php";
            const auto response = http_->get(url, timeout, params);
            check_http_response(response);
            return response;
         }

         std::string res(params_t &params, int timeout = 0)
         {
            params["key"] = api_key_;

            if (timeout <= 0)
               timeout = default_timeout_;

            return do_http_res(params, timeout);
         }

         std::string res(const std::string &action)
         {
            params_t params;
            params["action"] = action;
            return res(params);
         }

         void attach_default_params(params_t &params)
         {
            params["key"] = api_key_;

            if (!callback_.empty())
            {
               if (params.count("pingback") == 0)
               {
                  params["pingback"] = callback_;
               }
               else if (params["pingback"].empty())
               {
                  params.erase("pingback");
               }
            }

            last_captcha_has_callback_ = params.count("pingback");

            if (soft_id_ && !params.count("soft_id"))
               params["soft_id"] = std::to_string(soft_id_);
         }

         static std::string extract_ok_response(const std::string &response)
         {
            if (response.substr(0, 3) != "OK|")
               throw api_exception_t("Cannot recognize api response (" + response + ")");

            return response.substr(3);
         }

      public:
         impl_t() {}

         bool is_valid() const { return http_; }

         void set_http_client(http_client_i *http)
         {
            assert(http);
            http_ = http;
         }
         void set_api_key(const std::string &s) { api_key_ = s; }
         void set_soft_id(const int id) { soft_id_ = id; }
         void set_callback(const std::string &url) { callback_ = url; }
         void set_default_timeout(const int sec) { default_timeout_ = sec; }
         void set_recaptcha_timeout(const int sec) { recaptcha_timeout_ = sec; }
         void set_polling_interval(const int sec) { polling_interval_ = sec; }

         void solve(captcha_t &captcha)
         {
            std::map<std::string, int> wait_options;
            if (dynamic_cast<recaptcha_t *>(&captcha))
               wait_options["timeout"] = recaptcha_timeout_;

            solve(captcha, wait_options);
         }

         /* Can set 'timeout' and 'pollingInterval' options
          */
         void solve(captcha_t &captcha, const std::map<std::string, int> &wait_options)
         {
            int timeout = default_timeout_;
            auto i = wait_options.find("timeout");
            if (i != std::end(wait_options))
               timeout = i->second;

            const auto id = send(captcha, timeout);
            captcha.set_id(id);

            if (!last_captcha_has_callback_)
               wait_for_result(captcha, wait_options);
         }

         void wait_for_result(
             captcha_t &captcha, const std::map<std::string, int> &wait_options)
         {
            const auto started_at = std::chrono::steady_clock::now();

            int timeout = default_timeout_;
            auto i = wait_options.find("timeout");
            if (i != std::end(wait_options))
               timeout = i->second;

            int polling_interval = polling_interval_;
            i = wait_options.find("pollingInterval");
            if (i != std::end(wait_options))
               polling_interval = i->second;

            typedef std::chrono::duration<double> duration_t;
            double interval = 0;
            do
            {
               std::this_thread::sleep_for(std::chrono::seconds(polling_interval));

               try
               {
                  const auto result = get_result(captcha.id(), timeout);
                  if (!result.empty())
                  {
                     captcha.set_code(result);
                     return;
                  }
               }
               catch (network_exception_t &e)
               {
                  // ignore network errors
               }

               const auto now = std::chrono::steady_clock::now();

               const auto dur = std::chrono::duration_cast<duration_t>(now - started_at);
               interval = dur.count();
            } while (interval < (double)timeout);

            throw timeout_exception_t(
                "Timeout " + std::to_string(timeout) + " seconds reached");
         }

         std::string send(captcha_t &captcha, int timeout = 0)
         {
            const auto &files = captcha.files();

            // copy params
            auto params = captcha.params();

            attach_default_params(params);

            if (timeout <= 0)
               timeout = default_timeout_;

            const auto response = do_http_in(params, files, timeout);
            return extract_ok_response(response);
         }

         /**
          * Returns result of captcha if it was solved or empty string if result is not ready
          */
         std::string get_result(const std::string &id, int timeout = 0)
         {
            params_t params;
            params["action"] = "get";
            params["id"] = id;

            const auto response = res(params, timeout);

            if (response == "CAPCHA_NOT_READY")
               return "";

            return extract_ok_response(response);
         }

         double get_balance()
         {
            const auto response = res("getbalance");
            try
            {
               return std::stold(response, nullptr);
            }
            catch (...)
            {
               throw api_exception_t("Bad balance response (" + response + ")");
            }
         }

         void report(const std::string &id, const bool correct)
         {
            params_t params;
            params["id"] = id;

            if (correct)
            {
               params["action"] = "reportgood";
            }
            else
            {
               params["action"] = "reportbad";
            }

            res(params);
         }
      };

      impl_t *i_ = nullptr;

      client_t(const client_t &other);
      client_t &operator=(const client_t &other);

   public:
      client_t() : i_(new impl_t()) {}
      client_t(client_t &&other) : i_(other.i_) { other.i_ = nullptr; }
      client_t &operator=(client_t &&other)
      {
         client_t tmp(std::move(other));
         std::swap(i_, tmp.i_);
         return *this;
      }
      ~client_t() { delete i_; }

      bool is_valid() const { return i_->is_valid(); }
      void set_http_client(http_client_i *http) { return i_->set_http_client(http); }

      void set_api_key(const std::string &s) { return i_->set_api_key(s); }
      void set_soft_id(const int id) { return i_->set_soft_id(id); }
      void set_callback(const std::string &url) { return i_->set_callback(url); }
      void set_default_timeout(const int sec) { return i_->set_default_timeout(sec); }
      void set_recaptcha_timeout(const int sec) { return i_->set_recaptcha_timeout(sec); }
      void set_polling_interval(const int sec) { return i_->set_polling_interval(sec); }

      void solve(captcha_t &captcha) { return i_->solve(captcha); }

      void solve(captcha_t &captcha, const std::map<std::string, int> &wait_options)
      {
         return i_->solve(captcha, wait_options);
      }

      void wait_for_result(
          captcha_t &captcha, const std::map<std::string, int> &wait_options)
      {
         return i_->wait_for_result(captcha, wait_options);
      }

      std::string send(captcha_t &captcha, int timeout = 0)
      {
         return i_->send(captcha, timeout);
      }

      std::string get_result(const std::string &id, int timeout = 0)
      {
         return i_->get_result(id, timeout);
      }

      double get_balance()
      {
         return i_->get_balance();
      }

      void report(const std::string &id, const bool correct)
      {
         return i_->report(id, correct);
      }
   };

};

#endif /* API2CAPTCHA_HPP */
