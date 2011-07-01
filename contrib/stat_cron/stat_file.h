#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "date.h"

class stat_file{
   private:
      std::string filename;
      date filedate;
   public:
      
      stat_file(std::string filename0):filename(filename0){
         std::string sdate = filename.substr(7, 12);
         filedate.set_time(sdate);
      }
      
      bool operator==(stat_file& rhs){
         return filedate == rhs.filedate;
      }
      bool operator<(stat_file& rhs){
         return filedate < rhs.filedate;
      }
      bool operator>(stat_file& rhs){
         return filedate > rhs.filedate;
      }
      
      std::string get_filename(){
         return filename;
      }
      date get_date(){
         return filedate;
      }
      int open(){
         std::fstream fs(filename.c_str(), std::ios::out | std::ios::in);
         if(fs.fail()){
            return -1;
         }
         return 0;
      }
};
