#ifndef STAT_FILE_H
#define STAT_FILE_H

#include <string>
#include "date.h"

class stat_file{
   private:
      std::string filename;
      date filedate;
   public:
      
      stat_file(std::string filename0);
      
      bool operator==(stat_file& rhs);
      bool operator<(stat_file& rhs);
      bool operator>(stat_file& rhs);
      
      std::string get_filename();
      date get_date();
      int open();
};

#endif
