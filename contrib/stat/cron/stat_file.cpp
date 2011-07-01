#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "stat_file.h"

stat_file::stat_file(std::string filename0):filename(filename0){
   std::string sdate = filename.substr(7, 12);
   filedate.set_time(sdate);
}

bool stat_file::operator==(stat_file& rhs){
   return filedate == rhs.filedate;
}
bool stat_file::operator<(stat_file& rhs){
   return filedate < rhs.filedate;
}
bool stat_file::operator>(stat_file& rhs){
   return filedate > rhs.filedate;
}

std::string stat_file::get_filename(){
   return filename;
}
date stat_file::get_date(){
   return filedate;
}

int stat_file::open(){
   std::fstream fs(filename.c_str(), std::ios::out | std::ios::in);
   if(fs.fail()){
      return -1;
   }
   return 0;
}
