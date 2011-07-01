#ifndef STAT_FILE_MANAGER_H
#define STAT_FILE_MANAGER_H

#include <iostream>
#include <string>
#include <vector>

#include "stat_file.h"

class stat_file_manager{
   private:
      std::vector<stat_file> stat_files;
      std::string dirname;
   public:
      stat_file_manager(std::string dirname0);
      int merge(date ctime);
      date get_lastupdate_date();
      bool empty();
};

#endif
