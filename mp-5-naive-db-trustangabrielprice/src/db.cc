#include "db.hpp"

// Function to create a new table in the database
void Database::CreateTable(const std::string& table_name) {
    auto user_table = tables_.find(table_name);
    if (user_table != tables_.end()) {
        std::cout << "Table @ db.cc line 6: " << table_name << "already exists" << std::endl;
        return;
    } 
    tables_[table_name] = new DbTable();
}

// Function to drop an existing table from the database
void Database::DropTable(const std::string& table_name) {
    // Check if the table exists
    auto user_table = tables_.find(table_name);
    if (user_table == tables_.end()) {
        throw std::runtime_error("This table does not exist: db.cc line 17");
    }

    // Delete the DbTable object pointed to by user_table->second
    delete user_table->second;

    // Erase the entry from the map
    tables_.erase(user_table);
}

// Function to get a reference to a table by name
DbTable& Database::GetTable(const std::string& table_name) {
    auto it = tables_.find(table_name);
    if (it == tables_.end()) {
        // Table does not exist, create a new one
        tables_[table_name] = new DbTable;
    }
    // Return a reference to the DbTable object
    return *(tables_[table_name]);
}


Database::Database(const Database& rhs) {
    // Iterate over each table in rhs and deep-copy its content
    for (const auto& pair : rhs.tables_) {
        std::string name = pair.first;
        DbTable* table = pair.second;

        // Create a new DbTable object and deep-copy its content
        auto* new_table = new DbTable(*table);

        // Insert the new table into the tables_ map
        tables_[name] = new_table;
    }
}


// Copy assignment operator
// Copy assignment operator
Database& Database::operator=(const Database& rhs) {
    if (this == &rhs) {
        return *this;  // Self-assignment, no need to do anything
    }

    // Release memory for existing tables
    for (auto& pair : tables_) {
        delete pair.second;
    }
    tables_.clear();

    // Deep copy tables from rhs
    for (const auto& pair : rhs.tables_) {
        tables_[pair.first] = new DbTable(*pair.second);
    }

    return *this;
}

Database::~Database() {
    // Release memory for every table
    for (auto& pair : tables_) {
        delete pair.second;
    }
    // Clear the tables_ map
    tables_.clear();
}

std::ostream& operator<<(std::ostream& os, const Database& db) {
    // Iterate over each table in the database and print its name
    for (const auto& pair : db.tables_) {
        os << "Table: " << pair.first << "\n";
        os << *(pair.second) << "\n"; // Output the table using its operator<<
    }
    return os;
}


