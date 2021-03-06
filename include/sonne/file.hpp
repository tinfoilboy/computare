#pragma once

namespace Sonne
{
    
#ifdef _WIN32
    static constexpr const char Separator = '\\';
#else
    static constexpr const char Separator = '/';
#endif

    struct Entry
    {

        bool isValid = true;

        bool findEnd = false; // special flag for signaling the end of a directory find

        bool isDirectory = false;

        bool isSpecialDirectory = false; // for . or .. symlinks

        bool isHidden = false; // this attribute only really matters on windows

        std::string fullPath = "";
        std::string fileName = ""; // only used for GetNextEntry calls

        size_t fileSize = 0;

#ifdef _WIN32
        HANDLE windowsHandle = nullptr;
#else
        DIR* direntHandle = nullptr;
#endif

        std::vector<Entry> children; // a vector of child entries, used for a directory when listing

    };

#ifdef _WIN32
    void SetEntryFromHandle(
        const std::string& path,
        Entry& entry,
        WIN32_FIND_DATA& data,
        bool grabFullPath=true);
#endif

    /**
     Grab the current running path for the executable.

     Returns a blank string if not able to be found.
     */
    std::string GetRunningPath();

    /**
     Grab an entry from the filesystem, with information about that entry.
     */
    Entry GetFSEntry(std::string path, bool shouldClose=true);

    /**
     Grab an entry that is in a sequence from a directory.
     */
    Entry GetNextEntry(const std::string& rootDir, Entry& previous);

    /**
     Gets a vector of each entry in a directory, recursing through subfolders and appending to the entry children.
     */
    std::vector<Entry> WalkDirectory(std::string path);

}