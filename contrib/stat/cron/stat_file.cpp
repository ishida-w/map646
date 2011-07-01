#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <stdlib.h>

#include "stat_file.h"


stat_file::stat_file(std::string filename0, std::string dirname0 ):filename(filename0), dirname(dirname0){
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

json_object* stat_file::get_jobj(){
   if(open() < 0)
      return NULL;

   std::string stat, buf;

   while(fs && getline(fs, buf)){
      stat += buf;
   }

   json_object* jobj = json_tokener_parse(stat.c_str());

   if(is_error(jobj)){
      std::cout << "parse failed" << std::endl;
      return NULL;
   }

   if(close() < 0){
      std::cout << "close failed" << std::endl;
      exit(1);
   }
   
   return jobj;

}

int stat_file::open(){
   fs.open((dirname+"/"+filename).c_str(), std::ios::out | std::ios::in);
   
   if(fs.fail()){
      return -1;
   }
   return 0;
}

int stat_file::close(){
   fs.close();

   if(fs.fail()){
      return -1;
   }
   return 0;
}

