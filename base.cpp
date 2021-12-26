//
// Created by aai2002 on 12/25/21.
//

#include "base.h"

off_t my_std::filesize(filedesc_t file)
{
    struct stat buf{};
    fstat(file, &buf);
    return buf.st_size;
}

void my_std::closefile(filedesc_t file) {
    if(close(file)) {
        logging::WriteLineForcibly("Error while closing file");
    }
}