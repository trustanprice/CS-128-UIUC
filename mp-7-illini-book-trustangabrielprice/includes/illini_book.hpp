#ifndef ILLINI_BOOK_HPP
#define ILLINI_BOOK_HPP

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class IlliniBook {
public:
  IlliniBook(const std::string& people_fpath,
             const std::string& relations_fpath);
  IlliniBook(const IlliniBook& rhs) = delete;
  IlliniBook& operator=(const IlliniBook& rhs) = delete;
  ~IlliniBook() = default;
  bool BfsBase(int current_node, int target_node, std::set<int>& visited) const;
  bool BfsString(int current_node,
                 int target_node,
                 const std::string& target_relationship,
                 std::set<int>& visited) const;
  bool AreRelated(int uin_1, int uin_2) const;
  bool IsUINValid(int uin) const;
  bool AreRelated(int uin_1, int uin_2, const std::string& relationship) const;
  int GetRelated(int uin_1, int uin_2) const;
  int GetRelated(int uin_1, int uin_2, const std::string& relationship);
  std::vector<int> GetSteps(int uin, int n) const;
  void Dfs(int uin, std::set<int>& visited) const;
  size_t CountGroups() const;
  size_t CountGroups(const std::string& relationship) const;
  void DfsForRelationship(int uin,
                          const std::string& relationship,
                          std::set<int>& visited) const;
  size_t CountGroups(const std::vector<std::string>& relationships) const;
  void DfsForRelationships(int uin,
                           const std::vector<std::string>& relationships,
                           std::set<int>& visited) const;

private:
  std::string people_path_;
  std::string relations_path_;
  std::map<int, std::set<int>> adjacency_list_;
  std::map<std::pair<int, int>, std::string> relationship_map_;
  std::vector<int> unique_uins_;
};

#endif
