#include "flash.hpp"

Flash_mem::Flash_mem(){
    _size = (*((uint16_t *)(FLASHSIZE_BASE)));
    _pages = _size / (FLASH_PAGE_SIZE / 1024);
}

int Flash_mem::Size(){
    return _size;
}

int Flash_mem::Page_size(){
    return FLASH_PAGE_SIZE/1024;
}

int Flash_mem::Pages(){
    return _pages;
}
