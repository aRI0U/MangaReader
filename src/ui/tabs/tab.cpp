#include "tab.h"
#include <iostream>

Tab::Tab(QWidget *parent) : QWidget(parent)
{

}

Reader* Tab::getReader() const {
    std::cout << "no reader on that tab!" << std::endl;
    return nullptr;
}
