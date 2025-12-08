#ifndef B64_HPP
#define B64_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace api2captcha
{

    class B64
    {

        public:
        // A simple Base64 encoding function (you might use a more robust library in production)
        std::string base64_encode(const std::vector<unsigned char> &data)
        {
            const std::string base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

            std::string encoded_string;
            int i = 0;
            int j = 0;
            unsigned char char_array_3[3];
            unsigned char char_array_4[4];

            for (unsigned char byte : data)
            {
                char_array_3[i++] = byte;
                if (i == 3)
                {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for (i = 0; i < 4; i++)
                    {
                        encoded_string += base64_chars[char_array_4[i]];
                    }
                    i = 0;
                }
            }

            if (i)
            {
                for (j = i; j < 3; j++)
                {
                    char_array_3[j] = '\0';
                }

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (j = 0; j < i + 1; j++)
                {
                    encoded_string += base64_chars[char_array_4[j]];
                }

                while (i++ < 3)
                {
                    encoded_string += '=';
                }
            }

            return encoded_string;
        }
    };
};
#endif /* B64_HPP */