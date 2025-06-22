#include "matcher.h"
#include <queue>

std::map<std::string, std::string> matchApplicants(std::vector<Applicant>& applicants, std::vector<Company>& companies) {
    std::map<std::string, std::string> result; // company -> applicant
    std::map<std::string, Company*> companyMap;
    std::map<std::string, Applicant*> applicantMap;

    for (auto& c : companies) companyMap[c.name] = &c;
    for (auto& a : applicants) applicantMap[a.name] = &a;

    std::queue<Applicant*> freeApplicants;
    for (auto& a : applicants) freeApplicants.push(&a);

    while (!freeApplicants.empty()) {
        auto* app = freeApplicants.front();
        freeApplicants.pop();

        if (app->nextProposalIndex >= app->preferences.size()) continue;
        std::string companyName = app->preferences[app->nextProposalIndex++];
        auto* comp = companyMap[companyName];

        if (comp->currentMatch.empty()) {
            comp->currentMatch = app->name;
            result[comp->name] = app->name;
        } else {
            std::string current = comp->currentMatch;
            if (comp->preferenceRank[app->name] < comp->preferenceRank[current]) {
                comp->currentMatch = app->name;
                result[comp->name] = app->name;
                freeApplicants.push(applicantMap[current]);
            } else {
                freeApplicants.push(app);
            }
        }
    }
    return result;
}
