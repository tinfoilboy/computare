#pragma once

class Config;

struct DirectoryInfo
{
    std::map<std::string, FileInfo> languageTotals;
    FileInfo totals;
};

enum class CounterQueueType : uint8_t
{
    FILE,
    DIRECTORY
};

struct CounterQueue
{
    CounterQueueType type;
    std::string      path;
};

inline void PrintDirectoryInfo(const DirectoryInfo& info)
{
    // the largest cell thus far is the comment lines cell, which is 15 chars
    // including the end space
    size_t largestWidth = 14;
    size_t totalWidth   = 7;

    // add one to each width for the trailing space
    for (auto& itr : info.languageTotals)
    {
        largestWidth = std::max({
            largestWidth,
            totalWidth,
            itr.second.language.size(),
            std::to_string(itr.second.files).size(),
            std::to_string(itr.second.emptyLines).size(),
            std::to_string(itr.second.codeLines).size(),
            std::to_string(itr.second.commentLines).size(),
            std::to_string(itr.second.totalLines).size()
        });
    }

    largestWidth = std::max({
        largestWidth,
        totalWidth,
        std::to_string(info.totals.files).size(),
        std::to_string(info.totals.emptyLines).size(),
        std::to_string(info.totals.codeLines).size(),
        std::to_string(info.totals.commentLines).size(),
        std::to_string(info.totals.totalLines).size()
    });

    fmt::print(
        "| {}{}| {}{}| {}{}| {}{}| {}{}| {}{}|\n",
        "Language",
        std::string(largestWidth - std::string("Language").size(), ' '),
        "Total Files",
        std::string(largestWidth - std::string("Total Files").size(), ' '),
        "Empty Lines",
        std::string(largestWidth - std::string("Empty Lines").size(), ' '),
        "Code Lines",
        std::string(largestWidth - std::string("Code Lines").size(), ' '),
        "Comment Lines",
        std::string(largestWidth - std::string("Comment Lines").size(), ' '),
        "Total Lines",
        std::string(largestWidth - std::string("Total Lines").size(), ' ')
    );

    for (auto& itr : info.languageTotals)
    {
        fmt::print(
            "| {}{}| {}{}| {}{}| {}{}| {}{}| {}{}|\n",
            itr.second.language,
            std::string(largestWidth - itr.second.language.size(), ' '),
            itr.second.files,
            std::string(largestWidth - std::to_string(itr.second.files).size(), ' '),
            itr.second.emptyLines,
            std::string(largestWidth - std::to_string(itr.second.emptyLines).size(), ' '),
            itr.second.codeLines,
            std::string(largestWidth - std::to_string(itr.second.codeLines).size(), ' '),
            itr.second.commentLines,
            std::string(largestWidth - std::to_string(itr.second.commentLines).size(), ' '),
            itr.second.totalLines,
            std::string(largestWidth - std::to_string(itr.second.totalLines).size(), ' ')
        );
    }

    fmt::print(
        "| {}{}| {}{}| {}{}| {}{}| {}{}| {}{}|\n",
        "Total",
        std::string(largestWidth - std::string("Total").size(), ' '),
        info.totals.files,
        std::string(largestWidth - std::to_string(info.totals.files).size(), ' '),
        info.totals.emptyLines,
        std::string(largestWidth - std::to_string(info.totals.emptyLines).size(), ' '),
        info.totals.codeLines,
        std::string(largestWidth - std::to_string(info.totals.codeLines).size(), ' '),
        info.totals.commentLines,
        std::string(largestWidth - std::to_string(info.totals.commentLines).size(), ' '),
        info.totals.totalLines,
        std::string(largestWidth - std::to_string(info.totals.totalLines).size(), ' ')
    );
}

/**
 * Takes in a path to a directory and tries to walk that directory, counting
 * each file that it finds for lines.
 */
class DirectoryCounter
{
public:
    DirectoryCounter(const std::string& path);

    DirectoryInfo Run(Config& config);

private:
    std::string m_path;

    bool IsHidden(const fs::path& path);

    void AddDirectoryContentToQueue(
        Config& config,
        const std::string& path,
        size_t* ignoredFiles,
        size_t* newConfigs
    );

};