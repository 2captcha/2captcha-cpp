# C++ Module for 2Captcha API
The easiest way to quickly integrate [2Captcha] into your code to automate solving of any type of captcha.

- [Installation](#installation)
- [Configuration](#configuration)
- [Solve captcha](#solve-captcha)
  - [Normal Captcha](#normal-captcha)
  - [Text](#text-captcha)
  - [ReCaptcha v2](#recaptcha-v2)
  - [ReCaptcha v3](#recaptcha-v3)
  - [FunCaptcha](#funcaptcha)
  - [GeeTest](#geetest)
  - [hCaptcha](#hcaptcha)
  - [KeyCaptcha](#keycaptcha)
  - [Capy](#capy)
  - [Grid (ReCaptcha V2 Old Method)](#grid)
  - [Canvas](#canvas)
  - [ClickCaptcha](#clickcaptcha)
  - [Rotate](#rotate)
- [Other methods](#other-methods)
  - [send / getResult](#send--getresult)
  - [balance](#balance)
  - [report](#report)
- [Error handling](#error-handling)


## Installation
The api code is a single header file api2captcha.hpp, just copy it to your project and include where needed. You need to provide a HTTP-client implementation, as there is no standard one in C++. A sample HTTP-client based on libcurl is in curl_http.hpp (libcurl v. 7.62 or higher required), include it if you are using libcurl.

## Configuration
`Client` instance can be created like this:
```c++
api2captcha::client_t client ("YOUR_API_KEY");
```
There are few options that can be configured:
```c++
client.set_soft_id (123);
client.set_callback ("https://your.site/result-receiver");
client.set_default_timeout (120);
client.set_recaptcha_timeout (600);
client.set_polling_interval (10);
```

### Client instance options

|Option|Default value|Description|
|---|---|---|
|soft_id|-|your software ID obtained after publishing in [2captcha sofware catalog]|
|callback|-|URL of your web-sever that receives the captcha recognition result. The URl should be first registered in [pingback settings] of your account|
|default_timeout|120|Timeout in seconds for all captcha types except ReCaptcha. Defines how long the module tries to get the answer from `res.php` API endpoint|
|recaptcha_timeout|600|Timeout for ReCaptcha in seconds. Defines how long the module tries to get the answer from `res.php` API endpoint|
|polling_interval|10|Interval in seconds between requests to `res.php` API endpoint, setting values less than 5 seconds is not recommended|

>  **IMPORTANT:** once *callback URL* is defined for `client` instance with `set_callback`, all methods return only the captcha ID and DO NOT poll the API to get the result. The result will be sent to the callback URL.
To get the answer manually use [get_result method](#send--getresult)

## Solve captcha
When you submit any image-based captcha use can provide additional options to help 2captcha workers to solve it properly.

### Captcha options
|Option|Default Value|Description|
|---|---|---|
|numeric|0|Defines if captcha contains numeric or other symbols [see more info in the API docs][post options]|
|min_len|0|minimal answer lenght|
|max_len|0|maximum answer length|
|phrase|0|defines if the answer contains multiple words or not|
|case_sensitive|0|defines if the answer is case sensitive|
|calc|0|defines captcha requires calculation|
|lang|-|defines the captcha language, see the [list of supported languages] |
|hint_img|-|an image with hint shown to workers with the captcha|
|hint_text|-|hint or task text shown to workers with the captcha|

Below you can find basic examples for every captcha type. Check out [examples directory] to find more examples with all available options. You can build examples using 'make API_KEY=YOUR_API_KEY'. You can get example captchas from [2captcha demos](https://2captcha.com/demo).

### Basic example
Example below shows a basic solver call example with error handling.

```c++
api2captcha::normal_t captcha;
captcha.set_file ("path/to/captcha.jpg");
captcha.set_min_len (4);
captcha.set_max_len (20);
captcha.set_case_sensitive (true);
captcha.set_lang ("en");

try
{
    client.solve (captcha);
    printf ("Captcha solved: '%s'\n", captcha.code ().c_str ());
}
catch (std::exception & e)
{
    fprintf (stderr, "Error occurred: %s\n" + e.what ());
}
```

### Normal Captcha
To bypass a normal captcha (distorted text on image) use the following method. This method also can be used to recognize any text on the image.

```c++
api2captcha::normal_t captcha;
captcha.set_file ("path/to/captcha.jpg");
captcha.set_numeric (4);
captcha.set_min_len (4);
captcha.set_max_len (20);
captcha.set_phrase (true);
captcha.set_case_sensitive (true);
captcha.set_calc (false);
captcha.set_lang ("en");
captcha.set_hint_img_file ("path/to/hint.jpg");
captcha.set_hint_text ("Type red symbols only");
```

### Text Captcha
This method can be used to bypass a captcha that requires to answer a question provided in clear text.

```c++
api2captcha::text_t captcha;
captcha.set_text ("If tomorrow is Saturday, what day is today?");
captcha.set_lang ("en");
```

### ReCaptcha v2
Use this method to solve ReCaptcha V2 and obtain a token to bypass the protection.

```c++
api2captcha::recaptcha_t captcha;
captcha.set_site_key ("6Le-wvkSVVABCPBMRTvw0Q4Muexq1bi0DJwx_mJ-");
captcha.set_url ("https://mysite.com/page/with/recaptcha");
captcha.set_invisible (true);
captcha.set_action ("verify");
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```
### ReCaptcha v3
This method provides ReCaptcha V3 solver and returns a token.

```c++
api2captcha::recaptcha_t captcha;
captcha.set_site_key("6Le-wvkSVVABCPBMRTvw0Q4Muexq1bi0DJwx_mJ-");
captcha.set_url ("https://mysite.com/page/with/recaptcha");
captcha.set_version ("v3");
captcha.set_action ("verify");
captcha.set_score (0.3);
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```

### FunCaptcha
FunCaptcha (Arkoselabs) solving method. Returns a token.

```c++
api2captcha::funcaptcha_t captcha;
captcha.set_site_key ("69A21A01-CC7B-B9C6-0F9A-E7FA06677FFC");
captcha.set_url ("https://mysite.com/page/with/funcaptcha");
captcha.set_surl ("https://client-api.arkoselabs.com");
captcha.set_user_agent ("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.61 Safari/537.36");
captcha.set_data ("anyKey", "anyValue");
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```

### GeeTest
Method to solve GeeTest puzzle captcha. Returns a set of tokens as JSON.

```c++
api2captcha::geetest_t captcha;
captcha.set_gt ("f2ae6cadcf7886856696502e1d55e00c");
captcha.set_api_server ("api-na.geetest.com");
captcha.set_challenge ("12345678abc90123d45678ef90123a456b");
captcha.set_url ("https://mysite.com/captcha.html");
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```

### hCaptcha
Method to solve GeeTest puzzle captcha. Returns a set of tokens as JSON.

```c++
api2captcha::hcaptcha_t captcha;
captcha.set_site_key ("10000000-ffff-ffff-ffff-000000000001");
captcha.set_url ("https://www.site.com/page/");
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```

### KeyCaptcha
Token-based method to solve KeyCaptcha.

```c++
api2captcha::keycaptcha_t captcha;
captcha.set_user_id (10);
captcha.set_session_id ("493e52c37c10c2bcdf4a00cbc9ccd1e8");
captcha.set_web_server_sign ("9006dc725760858e4c0715b835472f22");
captcha.set_web_server_sign2 ("2ca3abe86d90c6142d5571db98af6714");
captcha.set_url ("https://www.keycaptcha.ru/demo-magnetic/");
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```

### Capy
Token-based method to bypass Capy puzzle captcha.

```c++
api2captcha::capy_t captcha;
captcha.set_site_key ("PUZZLE_Abc1dEFghIJKLM2no34P56q7rStu8v");
captcha.set_url ("https://www.mysite.com/captcha/");
captcha.set_proxy ("HTTPS", "login:password@IP_address:PORT");
```

### Grid
Grid method is originally called Old ReCaptcha V2 method. The method can be used to bypass any type of captcha where you can apply a grid on image and need to click specific grid boxes. Returns numbers of boxes.

```c++
api2captcha::grid_t captcha;
captcha.set_file ("path/to/captcha.jpg");
captcha.set_rows (3);
captcha.set_cols (3);
captcha.set_previous_id (0);
captcha.set_can_skip (false);
captcha.set_lang ("en");
captcha.set_hint_img_file ("path/to/hint.jpg");
captcha.set_hint_text ("Select all images with an Orange");
```

### Canvas
Canvas method can be used when you need to draw a line around an object on image. Returns a set of points' coordinates to draw a polygon.

```c++
api2captcha::canvas_t captcha;
captcha.set_file ("path/to/captcha.jpg");
captcha.set_previous_id (0);
captcha.set_can_skip (false);
captcha.set_lang ("en");
captcha.set_hint_img_file ("path/to/hint.jpg");
captcha.set_hint_text ("Draw around apple");
```

### ClickCaptcha
ClickCaptcha method returns coordinates of points on captcha image. Can be used if you need to click on particular points on the image.

```c++
api2captcha::coordinates_t captcha;
captcha.set_file ("path/to/captcha.jpg");
captcha.set_lang ("en");
captcha.set_hint_img_file ("path/to/hint.jpg");
captcha.set_hint_text ("Select all images with an Orange");
```

### Rotate
This method can be used to solve a captcha that asks to rotate an object. Mostly used to bypass FunCaptcha. Returns the rotation angle.

```c++
api2captcha::rotate_t captcha;
captcha.set_file ("path/to/captcha.jpg");
captcha.set_angle (40);
captcha.set_lang ("en");
captcha.set_hint_img_file ("path/to/hint.jpg");
captcha.set_hint_text ("Put the images in the correct way up");
```

## Other methods

### send / get_result
These methods can be used for manual captcha submission and answer polling.

```c++
std::string captcha_id = solver.send (captcha);

std::this_thread::sleep_for (std::chrono::seconds (10));

std::string code = solver.get_result (captcha_id);
```
### balance
Use this method to get your account's balance

```c++
double balance = solver.get_balance ();
```
### report
Use this method to report good or bad captcha answer.

```c++
solver.report (captcha.id (), true); // captcha solved correctly
solver.report (captcha.id (), false); // captcha solved incorrectly
```

## Error handling
If case of an error captcha solver throws an exception. It's important to properly handle these cases. We recommend to use `try catch` to handle exceptions.

```c++
try
{
    solver.solve (captcha);
}
catch (api2captcha::network_exception_t & e)
{
    // network error occurred
}
catch (api2captcha::api_exception_t & e)
{
    // api respond with error
}
catch (api2captcha::timeout_exception_t & e)
{
    // captcha is not solved so far
}
```
[examples directory]: /examples/
[2Captcha]: https://2captcha.com/
[2captcha sofware catalog]: https://2captcha.com/software
[pingback settings]: https://2captcha.com/setting/pingback
[post options]: https://2captcha.com/2captcha-api#normal_post
[list of supported languages]: https://2captcha.com/2captcha-api#language