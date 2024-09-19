#include <iostream>

#include "db.hpp"
#include "db_table.hpp"

#include "db_table.hpp"
#include <iostream>

int main() {
    // Define the number of rows and columns
    const unsigned int m = 3; // Number of rows
    const unsigned int n = 4; // Number of columns

    // Create a DbTable object with m rows and n columns
    DbTable table;

    // Add n columns to the table
    for (unsigned int i = 0; i < n; ++i) {
        std::pair<std::string, DataType> col_desc("Column" + std::to_string(i), DataType::kInt);
        table.AddColumn(col_desc);
    }

    // Add m rows to the table
    for (unsigned int i = 0; i < m; ++i) {
        std::initializer_list<std::string> row_data = {"1", "2", "3", "4"}; // Sample data for each column
        table.AddRow(row_data);
    }

    // Print the table
    std::cout << "Constructed Table:\n" << table << std::endl;

    // Verify properties
    std::cout << "Number of rows: " << m << std::endl;
    std::cout << "Number of columns: " << n << std::endl;

    return 0;
}
