#include "group.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "volunteer.hpp"

void Group::AddVolunteer(const Volunteer& vol) { volunteers_.push_back(vol); }

double Group::GetAvgBuildingExp() const {
  if (volunteers_.empty()) {
    return 0.0;
  }

  int total_building_exp = 0.0;
  for (const auto& vol : volunteers_) {
    total_building_exp += vol.GetBuildingExperience();
  }
  auto group_size = static_cast<double>(volunteers_.size());
  double avgbuilder = total_building_exp / group_size;
  return avgbuilder;
}

int Group::GetReturningMembers() const {
  int returningmembers = 0;
  if (volunteers_.empty()) {
    return 0.0;
  }

  for (const auto& vol : volunteers_) {
    if (vol.IsReturning()) {
      returningmembers++;
    }
  }
  return returningmembers;
}

double Group::GetAvgStamina() const {
  if (volunteers_.empty()) {
    return 0.0;
  }

  int total_physical_stamina = 0.0;
  for (const auto& vol : volunteers_) {
    total_physical_stamina += vol.GetPhysicalStamina();
  }
  auto group_size = static_cast<double>(volunteers_.size());
  double avgstamina = total_physical_stamina / group_size;
  return avgstamina;
}

Volunteer& Group::GetRandomVolunteer() {
  std::size_t random_index = std::rand() % volunteers_.size();

  return volunteers_[random_index];
}

std::ostream& operator<<(std::ostream& os, const Group& g_in) {
  os << "Group Information :" << std::endl;

  for (size_t i = 0; i < g_in.volunteers_.size(); ++i) {
    os << g_in.volunteers_.at(i).GetName() << std::endl;
  }
  os << std::endl;
  return os;
}
