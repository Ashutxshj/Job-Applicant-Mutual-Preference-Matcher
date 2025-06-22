#include "parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "utils.h"

// Applicant loader (fix skill as vector<string>)
bool loadApplicants(const std::string &filename, std::vector<Applicant> &applicants)
{
    std::ifstream fin(filename);
    if (!fin)
        return false;

    std::string line;
    while (std::getline(fin, line))
    {
        std::stringstream ss(line);
        std::string name, skillsStr;
        float gpa;
        int exp;

        std::getline(ss, name, ',');
        ss >> gpa;
        ss.ignore(1);
        ss >> exp;
        ss.ignore(1);
        std::getline(ss, skillsStr);

        std::vector<std::string> skills = split(trim(skillsStr), '|');

        applicants.push_back({trim(name), gpa, exp, 0.0f, skills, {}, 0});
    }
    return true;
}

// Company loader (just names, no change)
bool loadCompanies(const std::string &filename, std::vector<Company> &companies)
{
    std::ifstream fin(filename);
    if (!fin)
        return false;

    std::string line;
    while (std::getline(fin, line))
    {
        companies.push_back({trim(line)});
    }
    return true;
}

// Load applicant preferences (no change)
bool loadApplicantPreferences(const std::string &filename, std::vector<Applicant> &applicants)
{
    std::ifstream fin(filename);
    if (!fin)
        return false;

    std::map<std::string, std::vector<std::string>> prefMap;
    std::string line;
    while (std::getline(fin, line))
    {
        auto parts = split(line, ':');
        if (parts.size() != 2)
            continue;
        auto name = trim(parts[0]);
        auto prefs = split(parts[1], ',');

        for (auto &pref : prefs)
            pref = trim(pref);
        prefMap[name] = prefs;
    }

    for (auto &app : applicants)
    {
        if (prefMap.count(app.name))
        {
            app.preferences = prefMap[app.name];
        }
    }
    return true;
}

// New: Load company quality preferences from file
bool loadCompanyPreferences(const std::string &filename, std::vector<Company> &companies)
{
    std::ifstream fin(filename);
    if (!fin)
        return false;

    std::map<std::string, std::string> rawPrefs;
    std::string line;
    while (std::getline(fin, line))
    {
        auto parts = split(line, ':');
        if (parts.size() != 2)
            continue;
        auto companyName = trim(parts[0]);
        rawPrefs[companyName] = trim(parts[1]);
    }

    for (auto &company : companies)
    {
        if (rawPrefs.count(company.name))
        {
            std::string prefsStr = rawPrefs[company.name];
            auto prefsParts = split(prefsStr, ';');

            for (auto &part : prefsParts)
            {
                auto kv = split(part, '=');
                if (kv.size() != 2)
                    continue;

                std::string key = trim(kv[0]);
                std::string value = trim(kv[1]);

                if (key == "min_gpa")
                {
                    company.minGPA = std::stof(value);
                }
                else if (key == "min_experience")
                {
                    company.minExperience = std::stoi(value);
                }
                else if (key == "skills")
                {
                    company.requiredSkills = split(value, '|');
                    for (auto &skill : company.requiredSkills)
                        skill = trim(skill);
                }
            }
        }
    }
    return true;
}
void generateCompanyPreferences(std::vector<Company> &companies, const std::vector<Applicant> &applicants)
{
    for (auto &company : companies)
    {
        std::vector<std::pair<std::string, float>> scoredApplicants;

        for (const auto &applicant : applicants)
        {
            float score = 0.0f;
            if (applicant.gpa >= company.minGPA)
                score += 1.0f;
            if (applicant.experience >= company.minExperience)
                score += 1.0f;

            int matchedSkills = 0;
            for (const auto &skill : applicant.skills)
            {
                if (std::find(company.requiredSkills.begin(), company.requiredSkills.end(), skill) != company.requiredSkills.end())
                {
                    matchedSkills++;
                }
            }
            score += matchedSkills;

            scoredApplicants.emplace_back(applicant.name, score);
        }

        std::sort(scoredApplicants.begin(), scoredApplicants.end(),
                  [](const auto &a, const auto &b)
                  { return a.second > b.second; });

        for (size_t i = 0; i < scoredApplicants.size(); ++i)
        {
            company.preferenceList.push_back(scoredApplicants[i].first);
            company.preferenceRank[scoredApplicants[i].first] = static_cast<int>(i);
        }
    }
}
