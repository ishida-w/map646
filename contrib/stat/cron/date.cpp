
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

#include "date.h"

date::date(){
   time_t timer = time(NULL);
   ts = *(localtime(&timer));
}

date::date(tm ts0):ts(ts0){
}

date::date(std::string ts0){
   set_time(ts0);
}

void date::set_time(tm ts0){
   ts = ts0;
}

void date::set_time(std::string ts0){

   int precision = ts0.length();

   if(precision != 12 
         && precision != 10 
         && precision != 8
         && precision != 6
         && precision != 4){
      std::cout << "invalid lastupdate time" << std::endl;
      return;
   }

   std::stringstream ss;
   int year = -1, month = -1, day = -1, hour = -1, min = -1;

   ss << ts0.substr(0, 4) << std::endl;
   ss >> year;
   ss.str("");

   if(precision > 4){
      ss << ts0.substr(4, 2) << std::endl;
      ss >> month;
      ss.str("");
      if(precision > 6){
         ss << ts0.substr(6, 2) << std::endl;
         ss >> day;
         ss.str("");
         if(precision > 8){
            ss << ts0.substr(8, 2) << std::endl;
            ss >> hour;
            if(precision > 10){
               ss << ts0.substr(10, 2) << std::endl;
               ss >> min;
               ss.str("");
            }
         }
      }
   }

   ts.tm_year = year - 1900;
   ts.tm_mon = month - 1;
   ts.tm_mday = day;
   ts.tm_hour = hour;
   ts.tm_min = min;

}

tm date::get_time(){
   return ts;
}

std::string date::get_stime(){
   return s_year() + s_month() + s_day() + s_hour() + s_min();
}

int date::year(){
   return ts.tm_year + 1900;
}
int date::month(){
   return ts.tm_mon+1;
}
int date::day(){
   return ts.tm_mday;
}
int date::hour(){
   return ts.tm_hour;
}
int date::min(){
   return ts.tm_min;
}

std::string date::s_year(int offset){
   std::stringstream ss;
   ss << (year() + offset);
   return ss.str();
}
std::string date::s_month(int offset){
   std::stringstream ss;
   ss << (month() + offset);
   return ss.str();
}
std::string date::s_day(int offset){
   std::stringstream ss;
   ss << (day() + offset);
   return ss.str();
}
std::string date::s_hour(int offset){
   std::stringstream ss;
   ss << (hour() + offset);
   return ss.str();
}
std::string date::s_min(int offset){
   std::stringstream ss;
   ss << (min() + offset);
   return ss.str();
}

bool date::operator==(date& rhs){
   return year() == rhs.year() &&
      month() == rhs.month() &&
      day() == rhs.day() &&
      hour() == rhs.hour() &&
      min() == rhs.min();
}

bool date::operator<(date& rhs){
   if(year() != rhs.year()){
      return year() < rhs.year();
   }else if(month() != rhs.month()){
      return month() < rhs.month();
   }else if(day() != rhs.day()){
      return day() < rhs.day();
   }else if(hour() != rhs.hour()){
      return hour() < rhs.hour();
   }else if(min() != rhs.min()){
      return min() < rhs.min();
   }else{
      return false;
   }
}

bool date::operator>(date& rhs){
   if(year() != rhs.year()){
      return year() > rhs.year();
   }else if(month() != rhs.month()){
      return month() > rhs.month();
   }else if(day() != rhs.day()){
      return day() > rhs.day();
   }else if(hour() != rhs.hour()){
      return hour() > rhs.hour();
   }else if(min() != rhs.min()){
      return min() > rhs.min();
   }else{
      return false;
   }
}


