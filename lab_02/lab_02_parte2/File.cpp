//
// Created by Enrico on 18/04/2021.
//

#include "File.h"

File::File(uintmax_t size, uintmax_t date, std::string name): Base(name) {
    this->size = size;
    this->date = date;
}

uintmax_t File::getSize() const {
    return 0;
}

uintmax_t File::getDate() const {
    return 0;
}

void File::ls(int indent) const{

}

int File::mType() const{
    return 0;
}
