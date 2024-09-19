#include "illini_book.hpp"

int main() {
  // Initialize the IlliniBook instance with appropriate file paths and
  //    adjacency list
  std::string people_path =
      "/workspaces/mp-7-illini-book-trustangabrielprice/example/"
      "persons.csv";
  std::string relations_path =
      "/workspaces/mp-7-illini-book-trustangabrielprice/example/"
      "relations(1,2,string).csv";
  IlliniBook illiniBook(people_path, relations_path);

  std::cout << "Testing CountGroups: " << std::endl;
  std::cout << "AreRelated(1,2): "
            << (illiniBook.AreRelated(1, 2) ? "true" : "false") << std::endl;
  std::cout << "AreRelated(4,1): "
            << (illiniBook.AreRelated(4, 1) ? "true" : "false") << std::endl;
  std::cout << "CountGroups(Multiple invalid relationships): "
            << illiniBook.CountGroups(
                   std::vector<std::string>{"173", "Basketball", "White Sox"})
            << std::endl;

  return 0;
}