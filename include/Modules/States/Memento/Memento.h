#ifndef _MEMENTO_H_
#define _MEMENTO_H_

#include "common.h"
#include <stack>
#include <ctime>

class Memento
{

public:
   virtual ~Memento() {}
   virtual std::string get_name() const = 0;
   virtual std::string date() const = 0;
   virtual std::string state() const = 0;
};


#endif