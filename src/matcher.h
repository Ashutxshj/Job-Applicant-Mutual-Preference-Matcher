#pragma once
#include <map>
#include <vector>
#include <string>
#include "parser.h"

std::map<std::string, std::string> matchApplicants(std::vector<Applicant>& applicants, std::vector<Company>& companies);
