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
                  && filename.find(".stat") != std::string::npos ){
                 stat_file file(filename, dirname);
                 stat_files.push_back(file);
            }
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

json_object* stat_file_manager::merge(date time){
   std::vector<stat_file>::iterator it = stat_files.begin();
   json_object* jobj = json_new_object();
   
   while(it != stat_files.end()){
      std::string key;
      if((key = it->is_included(lastupdate))){
         json_object* jelement = it->get_jobj();
         if(jelement != NULL){
            json_object_object_add(jobj, key.c_str(), jelement);
            it->remove();
            std::erase(it);
         }
      }
      it++;
   }

   json_merge(jobj);

   stat_file newfile(time, dirname, jobj);

   
   return jobj;
}

int stat_file_manager::update(date ctime){
   date lastupdate = get_lastupdate_date();

   if(ctime.s_year() == lastupdate.s_year()){
      if(ctime.s_month() == lastupdate.s_month()){
         if(ctime.s_day() == lastupdate.s_day()){
            if(ctime.s_hour() == lastupdate.s_hour()){
               if(ctime.s_min() == lastupdate.s_min()){
                  std::cout << "entry already exists" << std::endl;
                  return -1;
               }
            }else{
               lastupdate.get_time().tm_min = -1;
               merge(lastupdate);
            }
         }else{
            lastupdate.get_time().tm_min = -1;
            merge(lastupdate);
            lastupdate.get_time().tm_hour = -1;
            merge(lastupdate);
         }
      }else{
         lastupdate.get_time().tm_min = -1;
         merge(lastupdate);
         lastupdate.get_time().tm_hour = -1;
         merge(lastupdate);
         lastupdate.get_time().tm_day = -1;
         merge(lastupdate);
      }
   }else{
      lastupdate.get_time().tm_min = -1;
      merge(lastupdate);
      lastupdate.get_time().tm_hour = -1;
      merge(lastupdate);
      lastupdate.get_time().tm_day = -1;
      merge(lastupdate);
      lastupdate.get_time().tm_month = -1;
      merge(lastupdate);
   }
   
   return 0;
}
