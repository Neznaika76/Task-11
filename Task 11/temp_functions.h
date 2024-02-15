//
//  Header.h
//  Task 11
//
//  Created by Pavel iPro on 15.02.2024.
//

#pragma once

#include <string>

struct Record {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int temperature;
  Record(int _year, int _month, int _day, int _hour, int _minute, int _temperature) {
    year = _year; month = _month; day = _day; hour = _hour; minute = _minute; temperature = _temperature;
  }
};

void ShowHelp(char* );

bool LoadData(std::string );

void ShowMonthStatistics(int , int );

void ShowYearStatistics(int );
