#include <iostream>
#include <string>
#include <sstream>

#include <time.h>

class date{
   tm ts;
   bool debug;
public:
   date(tm ts0):ts(ts0){
      debug = true;
   }
   date(){
      time_t timer = time(NULL);
      ts = *(localtime(&timer));
      debug = false;
   }
   void set_time(tm ts0){
      ts = ts0;
   }
   int year(){
      return ts.tm_year + 1900;
   }
   int month(){
      return ts.tm_mon+1;
   }
   int day(){
      return ts.tm_mday;
   }
   int hour(){
      return ts.tm_hour;
   }
   int min(){
      return ts.tm_min;
   }
   int sec(){
      return ts.tm_sec;
   }
   std::string s_year(int offset = 0){
      std::stringstream ss;
      ss << (year() + offset);
      return ss.str();
   }
   std::string s_month(int offset = 0){
      std::stringstream ss;
      ss << (month() + offset);
      return ss.str();
   }
   std::string s_day(int offset = 0){
      std::stringstream ss;
      ss << (day() + offset);
      return ss.str();
   }
   std::string s_hour(int offset = 0){
      std::stringstream ss;
      ss << (hour() + offset);
      return ss.str();
   }
   std::string s_min(int offset = 0){
      std::stringstream ss;
      ss << (min() + offset);
      return ss.str();
   }
   std::string s_sec(int offset = 0){
      std::stringstream ss;
      ss << (sec() + offset);
      return ss.str();
   }
};

