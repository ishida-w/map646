#include "stat_file_manager.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <dirent.h>
#include <sys/types.h>

stat_file_manager::stat_file_manager(std::string dirname0):dirname(dirname0){
   DIR *dp = opendir(dirname.c_str());

   if(dp != NULL){
      dirent* dent;
      do{
         dent = readdir(dp);
         if(dent != NULL){
            std::string filename(dent->d_name);
            if(filename.length() == 24 
                  && filename.find("map646_") != std::string::npos 
                  && filename.find(".stat") != std::string::npos )
               stat_files.push_back(filename);
         }
      }while(dent != NULL);

      closedir(dp);
   }
}


date stat_file_manager::get_lastupdate_date(){
   std::vector<stat_file>::iterator max_it = std::max_element(stat_files.begin(), stat_files.end());

   return max_it->get_date(); 
}

bool stat_file_manager::empty(){
   return stat_files.empty();
}

int stat_file_manager::merge(date ctime){
   date lastupdate = get_lastupdate_date();

   if(ctime.s_year() == lastupdate.s_year()){
      if(ctime.s_month() == lastupdate.s_month()){
         if(ctime.s_day() == lastupdate.s_day()){
            if(ctime.s_hour() == lastupdate.s_hour()){
               if(ctime.s_min() == lastupdate.s_min()){
                  std::cout << "entry already exists" << std::endl;
                  return -1;
               }else{
               }
            }else{
            }
         }else{
         }
      }else{
      }
   }
   
   return 0;

}
