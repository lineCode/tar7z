/*! \file archive.h

    Defines a main archive list
 */
#pragma once
#include "error.h"
#include "entry.h"
#include "header.h"
#include <vector>
#include <utility>

#ifndef TAR7Z_SADDY
    #include <boost/unordered/unordered_map.hpp>
#else
    #include "../../boost/boost-dist/boost/unordered/unordered_map.hpp"
#endif

namespace tar7z
{

class Entry;

class Archive
{
public:
    inline Archive()
    {

    }
    /*! Returns file by it's name or NULL if not found
        \param[in] name a name of file
        \return link to entry
     */
    tar7z::Entry* file(const std::string& name);
    /*! Adds new file to archive
        \param[in] name name of file
        \param[in] contents list of contents
        \param[in] default_time should we substitute creation time with 0
        \return false if name is invalid
     */
    bool add(const std::string& name, const std::vector<char>& contents, bool default_time = false );
    /*! Adds new file to archive
        \param[in] name name of file
        \param[in] contents list of contents
        \param[in] default_time should we substitute creation time with 0
        \return false if name is invalid
     */
    bool add(const std::string& name, const std::vector<unsigned char>& contents, bool default_time = false);
    /*! Removes a file with specified name from archove
        \param[in] name name of file
     */
    void remove(const std::string& name);
    /*! Adds new entry to list
       \param[in] name a name
       \param[in] e entry
     */
    void addEntry(const std::string& name, const tar7z::Entry& e);
    /*! A byte contents of archive
     */
    std::vector<char> Contents;
protected:
    /*! Returns size with padding
        \param sz size
        \return size
     */
    inline static size_t sizeWithPadding(size_t sz)
    {
        size_t result = sz;
        size_t oddpart = result % TAR7Z_ALIGNMENT_BLOCK;
        if (oddpart)
        {
            result +=  (TAR7Z_ALIGNMENT_BLOCK - oddpart);
        }
        return result;
    }
    /*! Appends header for the entry. The entry must have name of <100 characters
        \param[in,out] contents a changed content
        \param[in] entry an entry to be added
        \param[in] link whether it's link
        \param[in] default_time whether we should default time with zero
     */
    static void appendHeader(std::vector<char>& contents, const tar7z::Entry& entry, bool link, bool default_time);
    /*! Appends contents with padding
        \param destination a destination
        \param begin a beginning
        \param end an ending of file
     */
    template<typename InputIterator>
    static void appendAndPadContents(std::vector<char>& destination, InputIterator begin, InputIterator end) {
        size_t size = std::distance(begin, end);
        destination.insert(destination.end(), begin, end);
        // Pad content block
        size_t oddpart = size  % TAR7Z_ALIGNMENT_BLOCK;
        if (oddpart != 0)
        {
            destination.resize(destination.size() + (TAR7Z_ALIGNMENT_BLOCK - oddpart), TAR7Z_FILL_CHARACTER);
        }
    }
    /*! Compute header's checksum
        \param[in] contents content of checksum
        \return checksum
     */
    static unsigned int headerChecksum(const char* contents);
    /*! Validates file name, returns true if valid
        \param[in] filename name of file
        \return true if valid
     */
    static bool validateFileName(const std::string& filename);
    /*! A list of entries
     */
    std::vector<tar7z::Entry> m_entries;
    /*! A mapping between name of file and corresponding entry
     */
    boost::unordered_map<std::string, size_t> m_name_to_entry;
};

}
