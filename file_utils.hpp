#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <vector>
#include <fstream>

using namespace std;

namespace api2captcha
{
    class FileUtils
    {

    public:

        static const vector<unsigned char> readFile(std::string &file_path)
        {
            // 1. Read the image file into a byte vector
            ifstream file(file_path, ios::binary | ios::ate);
            if (file.is_open())
            {

                streamsize size = file.tellg();
                file.seekg(0, ios::beg);

                vector<unsigned char> file_data(size);

                if (file.read(reinterpret_cast<char *>(file_data.data()), size))
                {
                    file.close();
                    return file_data;
                }
            }
            return {};
        }
    };
};

#endif /* FILE_UTILS_HPP */