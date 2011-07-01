#include "stat_file_manager.h"

int stat_file_manager::merge(date ctime){
   date lastupdate = get_lastupdate_date();

   if(ctime.s_year() == lastupdate.s_year()){
      if(ctime.s_month() == lastupdate.s_month()){
         if(ctime.s_day() == lastupdate.s_day()){
            if(ctime.s_hour() == lastupdate.s_hour()){
               if(ctime.s_min() == lastupdate.s_min()){
                  std::cout << "entry already exists" << std::endl;
                  exit(1);
               }else{
                  merge();
               }
            }else{
               merge(jhour);
               merge(jday);
            }
         }else{
            merge(jhour);
            merge(jday);
            merge(jmonth);
         }
      }else{
         merge(jhour);
         merge(jday);
         merge(jmonth);
         merge(jyear);
      }
   }


}
