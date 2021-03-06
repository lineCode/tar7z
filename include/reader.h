/*! \file reader.h

    Describes a basic reader for files
 */
#pragma once
#include "archive.h"
#include "error.h"

namespace tar7z
{

/*! A basic reader, that could read TAR archives into tar7z::Archive
 */
class Reader
{
public:
    inline Reader()
    {

    }
    /*! Reads archive from specified file
        \param[in] filename a name for file
        \param[out] ar resulting archive
        \return error
     */
    virtual tar7z::Error read(const std::string& filename, tar7z::Archive& ar);
    /*! Can be inherited
     */
    virtual ~Reader();
protected:
    /*! Read the archive into header
        \param[in] offset offset of header from beginning
        \param[out] entry the entry
     */
    tar7z::Error readHeader(size_t offset, tar7z::Entry& entry);
    /*! A buffer for reading
     */
    std::vector<char> * m_buffer;
};

}
