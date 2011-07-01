#include <iostream>
#include <string>
#include <vector>

#include "stat_file.h"

class stat_file_manager{
   private:
      std::vector<stat_file> stat_files;
      std::string dirname;
   public:
      stat_file_manager(std::string dirname0):dirname{
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

      int merge(date ctime);

      date get_lastupdate_date(){

         std::vector<stat_file>::iterator max_it = std::max_element(stat_files.begin(), stat_files.end());

         return max_it->get_date(); 
      }
      bool empty(){
         return stat_files.empty();
      }
}
