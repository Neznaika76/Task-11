//
//  main.c
//  Task 11
//
//  Created by Pavel iPro on 15.02.2024.
//

#include "temp_functions.h"
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>

void ShowHelp(char* programName)
{
  std::cerr << "Usage: " << programName << " -f filename.csv" << " <-m month>" << " <-h>\n"
    << "Options:\n"
    << "\t-h Show this help message\n"
    << "\t-f Set the filename for processing\n"
    << "\t-m Show statistics for this month only\n";
}

std::vector<Record> Statistics;

bool LoadData(std::string datafilename)
{
  std::ifstream fs;
  fs.open(datafilename.c_str(), std::ofstream::in);
  if (!fs.is_open() || fs.fail())
  {
    std::cerr << "Could not open file '" << datafilename.c_str() << "'\n";
    fs.close();
    return false;
  }

  std::string line;
  getline(fs, line); // skip the first line

  const char* delimiters = ",; ";
  size_t i = 1;
  while (!fs.eof())
  {
    i++;
    getline(fs, line);
    if (line.empty()) continue; // ignore empty lines
    int rec[6];
    int c = 0;
    // Tokenize the line
    char* token = std::strtok((char*)line.c_str(), delimiters);
    while (token) {
      try { rec[c] = std::stoi(token); }
      catch (...) { rec[c] = -1; }
      token = std::strtok(nullptr, delimiters);
      c++;
    }
    if (c == 6) { //must be 6 columns
      Record record(rec[0], rec[1], rec[2], rec[3], rec[4], rec[5]);
      Statistics.push_back(record);
    }
    else {
      std::cerr << "Read error detected in line #" << i << ": " << line << std::endl;
    }
  }
  fs.close();

  return true;
}

void ShowMonthStatistics(int year, int month)
{
  if (Statistics.empty()) return;
  int minTemperature = +INT_MAX;
  int maxTemperature = -INT_MAX;
  double meanTemperature = 0.0;
  size_t n = 0;
  for (const Record& record : Statistics) {
    if ((record.year == year) && (record.month == month)) {
      meanTemperature += record.temperature;
      if (record.temperature < minTemperature) minTemperature = record.temperature;
      if (record.temperature > maxTemperature) maxTemperature = record.temperature;
      n++;
    }
  }
  std::vector<std::string> monthes = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
  if (n > 0) {
    meanTemperature /= n;
    std::cout << "Statistics for " << monthes[month] << " " << year << std::endl;
    std::cout << "  minimal temperature: " << minTemperature << std::endl;
    std::cout << "  maximal temperature: " << maxTemperature << std::endl;
    std::cout << "  average temperature: " << meanTemperature << std::endl;
    std::cout << std::endl;
  }
}

void ShowYearStatistics(int year)
{
  if (Statistics.empty()) return;
  int minTemperature = +INT_MAX;
  int maxTemperature = -INT_MAX;
  double meanTemperature = 0.0;
  size_t n = 0;
  for (const Record& record : Statistics) {
    if (record.year == year) {
      meanTemperature += record.temperature;
      if (record.temperature < minTemperature) minTemperature = record.temperature;
      if (record.temperature > maxTemperature) maxTemperature = record.temperature;
      n++;
    }
  }
  if (n > 0) {
    meanTemperature /= n;
    std::cout << "Statistics for " << year << " year" << std::endl;
    std::cout << "  minimal temperature: " << minTemperature << std::endl;
    std::cout << "  maximal temperature: " << maxTemperature << std::endl;
    std::cout << "  average temperature: " << meanTemperature << std::endl;
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2) { ShowHelp(argv[0]); return 0; }
  std::string inputfile;
  int month = 0;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (0 == arg.find("-h")) {
      ShowHelp(argv[0]);
      return 0;
    }
    if (0 == arg.find("-f")) {
      inputfile = std::string(argv[i+1]);
    }
    if (0 == arg.find("-m")) {
      if (argv[i+1]) month = atoi(argv[i+1]);
    }
  }
  if (inputfile.empty()) { ShowHelp(argv[0]); return 0; }

  if (!LoadData(inputfile)) return -1;

  int year = Statistics[0].year;
  if (month > 0)
    ShowMonthStatistics(year, month);
  else
    for (month = 1; month <= 12; month++)
      ShowMonthStatistics(year, month);

  ShowYearStatistics(year);

  return 0;
}
