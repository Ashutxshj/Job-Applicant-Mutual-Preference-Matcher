#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "utils.h"

struct Applicant
{
    std::string name;
    float gpa;
    int experience;
    float skillMatch;
    std::vector<std::string> skills;
    std::vector<std::string> preferences;
    int nextProposalIndex = 0;
};
struct Company
{
    std::string name;

    float minGPA = 0.0f;
    int minExperience = 0;
    std::vector<std::string> requiredSkills;

    std::vector<std::string> preferenceList;
    std::map<std::string, int> preferenceRank;
    std::string currentMatch = "";

    Company(const std::string &n) : name(n) {}
};

bool loadApplicants(const std::string &filename, std::vector<Applicant> &applicants);
bool loadCompanies(const std::string &filename, std::vector<Company> &companies);
bool loadApplicantPreferences(const std::string &filename, std::vector<Applicant> &applicants);
void generateCompanyPreferences(std::vector<Company> &companies, const std::vector<Applicant> &applicants);
