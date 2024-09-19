#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "constants.hpp"  // Assuming you have constants like kMaxIterations
#include "group.hpp"      // Assuming you have the Group class definition

std::vector<Volunteer> ReadVolunteersFromFile(const std::string& inputFile) {
  std::vector<Volunteer> vol;
  std::ifstream file{inputFile};

  // Read each line from the file
  if (file.is_open()) {
    std::string name;
    int building_exp = 0;
    int physical_stamina = 0;
    bool returning = false;

    // Read tokens from the line
    while (file >> name >> building_exp >> physical_stamina >> returning) {
      Volunteer volunteer(name, building_exp, physical_stamina, returning);
      vol.push_back(volunteer);
    }
    file.close();
  }
  return vol;
}

void PerformSwaps(std::vector<Group>& groups) {
  size_t g1 = rand() % groups.size();
  size_t g2 = rand() % groups.size();
  while (g1 == g2) {
    g2 = rand() % groups.size();
  }

  // Get random volunteers from the selected groups
  Volunteer& swap1 = groups[g1].GetRandomVolunteer();
  Volunteer& swap2 = groups[g2].GetRandomVolunteer();

  // Ensure unique names within groups
  while (swap1.GetName() == swap2.GetName()) {
    // If names are the same, get new volunteers
    g1 = rand() % groups.size();
    g2 = rand() % groups.size();
    while (g1 == g2) {
      g2 = rand() % groups.size();
    }
    swap1 = groups[g1].GetRandomVolunteer();
    swap2 = groups[g2].GetRandomVolunteer();
  }

  // Perform the swap
  std::swap(swap1, swap2);
}

bool SatisfyConditions(const std::vector<Group>& kGroup) {
  for (const auto& vol : kGroup) {
    if (vol.GetAvgBuildingExp() < kMinAvgBuildingExp ||
        vol.GetAvgStamina() < kMinAvgStamina ||
        vol.GetReturningMembers() < kMinReturning) {
      return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    return 0;
  }
  std::string input_file = argv[1];
  size_t number = std::stoi(argv[2]);
  std::string output_file = argv[3];
  srand(time(nullptr));
  std::vector<Volunteer> vols = ReadVolunteersFromFile(input_file);
  std::vector<Group> groups;
  unsigned int total_groups = number / kSizeGroups;
  for (unsigned int i = 0; i < total_groups; ++i) {
    Group holder;
    for (unsigned int j = 0; j < kSizeGroups; ++j) {
      holder.AddVolunteer(vols[j + (i * kSizeGroups)]);
    }
    groups.push_back(holder);
  }
  unsigned int loops = 0;
  while (!SatisfyConditions(groups) && loops < kMaxIterations) {
    PerformSwaps(groups);
    loops++;
  }

  if (SatisfyConditions(groups)) {
    std::ofstream output(output_file);
    for (const auto& vol : groups) {
      output << vol;
    }
    output.close();
  }
  std::cout << "not happening" << std::endl;
}
