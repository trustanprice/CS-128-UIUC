#include "volunteer.hpp"

Volunteer::Volunteer(const std::string& name,
                     int building_experience,
                     int physical_stamina,
                     bool returning):
    name_(name), returning_(returning) {
  if (building_experience < kMinBuildingExp) {
    building_experience_ = kMinBuildingExp;
  } else if (building_experience > kMaxBuildingExp) {
    building_experience_ = kMaxBuildingExp;
  } else {
    building_experience_ = building_experience;
  }

  if (physical_stamina < kMinStamina) {
    physical_stamina_ = kMinStamina;
  } else if (physical_stamina > kMaxStamina) {
    physical_stamina_ = kMaxStamina;
  } else {
    physical_stamina_ = physical_stamina;
  }
}

std::string Volunteer::GetName() const { return name_; }

int Volunteer::GetBuildingExperience() const { return building_experience_; }

int Volunteer::GetPhysicalStamina() const { return physical_stamina_; }

bool Volunteer::IsReturning() const { return returning_; }