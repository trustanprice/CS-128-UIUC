#include "illini_book.hpp"

// Your code here!
IlliniBook::IlliniBook(const std::string& people_fpath,
                       const std::string& relations_fpath) {
  std::ifstream people_file(people_fpath);
  if (!people_file.is_open()) {
    throw std::runtime_error("Failed to open people CSV file: " + people_fpath);
  }
  int uin = 0;
  while (people_file >> uin) {
    unique_uins_.push_back(uin);
  }
  people_file.close();
  std::ifstream relations_file(relations_fpath);
  if (!relations_file.is_open()) {
    throw std::runtime_error("Failed to open relationships CSV file: " +
                             relations_fpath);
  }
  std::string line;
  while (std::getline(relations_file, line)) {
    size_t pos_a = line.find(',');
    if (pos_a == std::string::npos) {
      throw std::runtime_error("Invalid format in relations CSV file: " +
                               relations_fpath);
    }
    size_t pos_b = line.find(',', pos_a + 1);
    if (pos_b == std::string::npos) {
      throw std::runtime_error("Invalid format in relations CSV file: " +
                               relations_fpath);
    }
    int uin_a = std::stoi(line.substr(0, pos_a));
    int uin_b = std::stoi(line.substr(pos_a + 1, pos_b - pos_a - 1));
    adjacency_list_[uin_a].insert(uin_b);
    adjacency_list_[uin_b].insert(uin_a);
    relationship_map_[{uin_a, uin_b}] = line.substr(pos_b + 1);
    relationship_map_[{uin_b, uin_a}] = line.substr(pos_b + 1);
  }
  relations_file.close();
}

bool IlliniBook::BfsBase(int current_node,
                         int target_node,
                         std::set<int>& visited) const {
  // Ensure current_node is smaller than target_node
  if (current_node == target_node) {
    return false;
  }
  visited.insert(current_node);

  // Check if the current_node exists in the adjacency list
  if (adjacency_list_.find(current_node) == adjacency_list_.end()) {
    // current_node has no neighbors
    return false;
  }

  // Check if there's a direct relationship between current_node and target_node
  const std::set<int>& neighbors = adjacency_list_.at(current_node);
  if (neighbors.find(target_node) != neighbors.end()) {
    return true;
  }

  // Explore bidirectional relationships
  for (int neighbor : neighbors) {
    if (visited.find(neighbor) == visited.end()) {
      if (BfsBase(neighbor, target_node, visited)) {
        return true;
      }
    }
  }

  return false;
}

bool IlliniBook::BfsString(int current_node,
                           int target_node,
                           const std::string& target_relationship,
                           std::set<int>& visited) const {
  visited.insert(current_node);

  // Check if the current_node exists in the adjacency list
  if (adjacency_list_.find(current_node) == adjacency_list_.end()) {
    // Current node has no neighbors
    return false;
  }

  // Check if there's a direct relationship between current_node and target_node
  const std::set<int>& neighbors = adjacency_list_.at(current_node);
  for (int neighbor : neighbors) {
    if (neighbor == target_node &&
        relationship_map_.count({current_node, neighbor}) !=
            0 &&  // Explicit comparison
        relationship_map_.at({current_node, neighbor}) == target_relationship) {
      return true;
    }
  }

  // Explore bidirectional relationships
  for (int neighbor : neighbors) {
    if (visited.find(neighbor) == visited.end() &&
        relationship_map_.count({current_node, neighbor}) != 0 &&
        relationship_map_.at({current_node, neighbor}) == target_relationship) {
      if (BfsString(neighbor, target_node, target_relationship, visited)) {
        return true;
      }
    }
  }

  return false;
}

bool IlliniBook::AreRelated(int uin_1, int uin_2) const {
  // Check if both uin_1 and uin_2 are present in the people CSV
  if (!IsUINValid(uin_1) || !IsUINValid(uin_2)) {
    return false;  // At least one UIN is not in the people CSV
  }

  // Perform BFS to check if there's a path between uin_1 and uin_2
  std::set<int> visited;
  return BfsBase(uin_1, uin_2, visited);
}

bool IlliniBook::IsUINValid(int uin) const {
  // Check if the UIN exists in the unique UINs vector
  for (int u : unique_uins_) {
    if (u == uin) {
      return true;
    }
  }
  return false;
}

bool IlliniBook::AreRelated(int uin_1,
                            int uin_2,
                            const std::string& relationship) const {
  // Check if either UIN is invalid
  if (uin_1 <= 0 || uin_2 <= 0) {
    throw std::invalid_argument("Invalid UINs: " + std::to_string(uin_1) +
                                ", " + std::to_string(uin_2));
  }

  // Check if both UINs are in the vector of UINs from the people CSV
  bool found_uin_1 = false;
  bool found_uin_2 = false;
  for (int uin : unique_uins_) {
    if (uin == uin_1) {
      found_uin_1 = true;
    }
    if (uin == uin_2) {
      found_uin_2 = true;
    }
    if (found_uin_1 && found_uin_2) {
      break;  // Both UINs found, no need to continue searching
    }
  }

  // If either UIN is not found, return false
  if (!found_uin_1 || !found_uin_2) {
    return false;
  }

  // Both UINs are valid and in the people CSV, perform BFS traversal
  std::set<int> visited;
  return BfsString(uin_1, uin_2, relationship, visited);
}

int IlliniBook::GetRelated(int uin_1, int uin_2) const {
  if (!(AreRelated(uin_1, uin_2))) {
    return -1;
  }
  std::set<int> visited;
  std::queue<std::pair<int, int>>
      queue;  // Queue stores node and its distance from uin_1
  queue.push({uin_1, 0});
  while (!queue.empty()) {
    auto [current_node, distance] = queue.front();
    queue.pop();
    if (current_node == uin_2) {
      return distance;
    }
    visited.insert(current_node);
    if (adjacency_list_.find(current_node) == adjacency_list_.end()) {
      // Current node has no neighbors
      continue;
    }
    const std::set<int>& neighbors = adjacency_list_.at(current_node);
    for (int neighbor : neighbors) {
      if (visited.find(neighbor) == visited.end()) {
        queue.push({neighbor, distance + 1});
        visited.insert(
            neighbor);  // Mark neighbor as visited to avoid revisiting
      }
    }
  }
  return -1;
}

int IlliniBook::GetRelated(int uin_1,
                           int uin_2,
                           const std::string& relationship) {
  std::set<int> visited;
  std::queue<std::pair<int, int>>
      queue;  // Queue stores node and its distance from uin_1

  // Enqueue starting node uin_1 with distance 0
  queue.push({uin_1, 0});

  // Perform BFS
  while (!queue.empty()) {
    auto [current_node, distance] = queue.front();
    queue.pop();

    // If current node is the target node uin_2, return the distance
    if (current_node == uin_2) {
      return distance;
    }

    visited.insert(current_node);

    // Enqueue unvisited neighbors of the current node with distance incremented
    // by 1
    for (int neighbor : adjacency_list_[current_node]) {
      if (visited.find(neighbor) == visited.end() &&
          relationship_map_.count({current_node, neighbor}) != 0 &&
          relationship_map_.at({current_node, neighbor}) == relationship) {
        queue.push({neighbor, distance + 1});
        visited.insert(
            neighbor);  // Mark neighbor as visited to avoid revisiting
      }
    }
  }
  return -1;  // No path found
}

std::vector<int> IlliniBook::GetSteps(int uin, int n) const {
  std::vector<int> steps;
  if (!IsUINValid(uin) || n < 1) {
    return steps;  // At least one UIN is not in the people CSV
  }

  // Check if the node has adjacent nodes
  if (adjacency_list_.find(uin) == adjacency_list_.end()) {
    return steps;  // No adjacent nodes, return empty vector
  }
  // Queue for BFS
  std::queue<std::pair<int, int>> queue;  // {node, depth}
  queue.push({uin, 0});

  // Set to mark visited nodes
  std::set<int> visited;
  visited.insert(uin);

  while (!queue.empty()) {
    auto [node, depth] = queue.front();
    queue.pop();

    // Check if the current node is exactly n steps away
    if (depth == n) {  // could change to <= n
      steps.push_back(node);
    } else if (depth > n) {
      break;  // Stop if depth exceeds n
    }

    // Add unvisited neighbors to the queue
    for (int neighbor : adjacency_list_.at(node)) {
      if (visited.find(neighbor) == visited.end()) {
        queue.push({neighbor, depth + 1});
        visited.insert(neighbor);
      }
    }
  }

  return steps;
}

size_t IlliniBook::CountGroups() const {
  // Set to keep track of visited nodes
  std::set<int> visited;
  // Initialize the count of groups
  size_t groups_count = 0;

  // Iterate over all unique UINs
  for (int uin : unique_uins_) {
    // If the current UIN has not been visited yet
    if (visited.find(uin) == visited.end()) {
      // Increment the group count
      groups_count++;
      // Perform DFS starting from the current UIN to explore the connected
      // component
      Dfs(uin, visited);
    }
  }

  return groups_count;
}

void IlliniBook::Dfs(int uin, std::set<int>& visited) const {
  // Mark the current UIN as visited
  visited.insert(uin);

  // Check if the current UIN has adjacent nodes
  if (adjacency_list_.find(uin) == adjacency_list_.end()) {
    // If not, return since there are no more nodes to explore
    return;
  }

  // Iterate over the adjacent nodes of the current UIN
  for (int neighbor : adjacency_list_.at(uin)) {
    // If the neighbor has not been visited yet, recursively explore it
    if (visited.find(neighbor) == visited.end()) {
      Dfs(neighbor, visited);
    }
  }
}

size_t IlliniBook::CountGroups(const std::string& relationship) const {
  // Set to keep track of visited nodes
  std::set<int> visited;
  // Initialize the count of groups
  size_t groups_count = 0;

  // Iterate over all unique UINs
  for (int uin : unique_uins_) {
    // If the current UIN has not been visited yet
    if (visited.find(uin) == visited.end()) {
      // Increment the group count
      groups_count++;
      // Perform DFS starting from the current UIN to explore the connected
      // component
      DfsForRelationship(uin, relationship, visited);
    }
  }

  return groups_count;
}

void IlliniBook::DfsForRelationship(int uin,
                                    const std::string& relationship,
                                    std::set<int>& visited) const {
  // Mark the current UIN as visited
  visited.insert(uin);

  // Check if the current UIN has adjacent nodes
  if (adjacency_list_.find(uin) == adjacency_list_.end()) {
    // If not, return since there are no more nodes to explore
    return;
  }

  // Iterate over the adjacent nodes of the current UIN
  for (int neighbor : adjacency_list_.at(uin)) {
    // Check if the edge between uin and neighbor has the specified relationship
    if (relationship_map_.count({uin, neighbor}) != 0 &&
        relationship_map_.at({uin, neighbor}) == relationship) {
      // If the edge has the specified relationship and the neighbor has not
      // been visited yet
      if (visited.find(neighbor) == visited.end()) {
        // Recursively explore the neighbor
        DfsForRelationship(neighbor, relationship, visited);
      }
    }
  }
}

size_t IlliniBook::CountGroups(
    const std::vector<std::string>& relationships) const {
  // Set to keep track of visited nodes
  std::set<int> visited;
  // Initialize the count of groups
  size_t groups_count = 0;

  // Iterate over all unique UINs
  for (int uin : unique_uins_) {
    // If the current UIN has not been visited yet
    if (visited.find(uin) == visited.end()) {
      // Increment the group count
      groups_count++;
      // Perform DFS starting from the current UIN to explore the connected
      // component
      DfsForRelationships(uin, relationships, visited);
    }
  }

  return groups_count;
}

void IlliniBook::DfsForRelationships(
    int uin,
    const std::vector<std::string>& relationships,
    std::set<int>& visited) const {
  // Mark the current UIN as visited
  visited.insert(uin);

  // Check if the current UIN has adjacent nodes
  if (adjacency_list_.find(uin) == adjacency_list_.end()) {
    // If not, return since there are no more nodes to explore
    return;
  }

  // Iterate over the adjacent nodes of the current UIN
  for (int neighbor : adjacency_list_.at(uin)) {
    // Check if the edge between uin and neighbor is in any of the specified
    // relationships
    for (const std::string& relationship : relationships) {
      if (relationship_map_.count({uin, neighbor}) != 0 &&
          relationship_map_.at({uin, neighbor}) == relationship) {
        // If the edge is in the specified relationship and the neighbor has not
        // been visited yet
        if (visited.find(neighbor) == visited.end()) {
          // Recursively explore the neighbor
          DfsForRelationships(neighbor, relationships, visited);
        }
        // Break out of the loop since the edge is found in one of the specified
        // relationships
        break;
      }
    }
  }
}
