#include <iostream>
#include "parser.h"
#include "matcher.h"

int main() {
    std::vector<Applicant> applicants;
    std::vector<Company> companies;

    if (!loadApplicants("data/applicants.txt", applicants)) {
        std::cerr << "Failed to load applicants.\n";
        return 1;
    }

    if (!loadCompanies("data/companies.txt", companies)) {
        std::cerr << "Failed to load companies.\n";
        return 1;
    }

    if (!loadApplicantPreferences("data/preferences_applicants.txt", applicants)) {
        std::cerr << "Failed to load applicant preferences.\n";
        return 1;
    }

    generateCompanyPreferences(companies, applicants); // scoring logic

    auto matches = matchApplicants(applicants, companies);

    std::ofstream fout("output/final_matches.txt");
    for (const auto& [company, applicant] : matches) {
        fout << applicant << " -> " << company << "\n";
        std::cout << applicant << " -> " << company << "\n";
    }
    fout.close();

    return 0;
}
