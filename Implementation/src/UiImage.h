#pragma once

#include <gui/Image.h>
#include <mu/mu.h>
#include <mu/IAppSettings.h>

#include <filesystem>
#include <string>
#include <vector>

namespace ui
{
    inline std::filesystem::path findImageFile(const char* fileName)
    {
        if (!fileName || !fileName[0])
            return {};

        std::vector<std::filesystem::path> candidates;

        if (auto* settings = mu::getAppSettings())
        {
            const std::filesystem::path resPath = settings->getResPath();
            const std::filesystem::path appPath(settings->getApplicationFolder().c_str());

            candidates.emplace_back(resPath / "images" / fileName);
            candidates.emplace_back(resPath / "res" / "images" / fileName);

            candidates.emplace_back(appPath / "res" / "images" / fileName);
            candidates.emplace_back(appPath / "images" / fileName);

            candidates.emplace_back(appPath / ".." / "Resources" / "images" / fileName);
        }

        const std::filesystem::path cwd = std::filesystem::current_path();
        candidates.emplace_back(cwd / "res" / "images" / fileName);
        candidates.emplace_back(cwd / ".." / "res" / "images" / fileName);

        for (const auto& candidate : candidates)
        {
            std::error_code ec;
            if (std::filesystem::exists(candidate, ec) &&
                !ec &&
                std::filesystem::is_regular_file(candidate, ec) &&
                !ec)
            {
                return candidate.lexically_normal();
            }
        }

        return {};
    }


    inline bool loadImage(gui::Image& image, const char* fileName)
    {
        const auto filePath = findImageFile(fileName);
        if (filePath.empty())
            return false;

        const std::string nativePath = filePath.string();
        return image.load(nativePath.c_str());
    }
}
