#include "db_table.hpp"

// AddColumn function definition
void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
    if (col_descs_.size() == row_col_capacity_) {
        for (auto& row : rows_) {
            void** new_row_data = new void*[static_cast<unsigned long>(row_col_capacity_) * 2];
            for (unsigned int i = 0; i < row_col_capacity_; ++i) {
                new_row_data[i] = row.second[i];
            }
            delete[] row.second;
            row.second = new_row_data;
        }
        row_col_capacity_ = row_col_capacity_ * 2;
    }
    col_descs_.push_back(col_desc);
    for (auto& row : rows_) {
        unsigned int new_col_index = col_descs_.size() - 1;
        switch (col_desc.second) {
            case DataType::kString:
                row.second[new_col_index] = static_cast<void*>(new std::string(""));
                break;
            case DataType::kInt:
                row.second[new_col_index] = static_cast<void*>(new int(0));
                break;
            case DataType::kDouble:
                row.second[new_col_index] = static_cast<void*>(new double(0.0));
                break;
            default:
                throw std::invalid_argument("Unsupported data type");
        }
    }
}

// DeleteColumnByIdx function definition
void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
    if (col_idx >= col_descs_.size()) {
        throw std::out_of_range("col_idx is out of range");
    } 
    if (col_descs_.size() == 1 && !rows_.empty()) {
        throw std::runtime_error("any table with at least one row must have at least one column.");
    }

    for (auto& row : rows_) {
        // Get the pointer to the dynamically allocated object
        void* obj_to_delete = row.second[col_idx];

        // Determine the appropriate type based on the column's DataType
        switch (col_descs_[col_idx].second) {
            case DataType::kString:
                delete static_cast<std::string*>(obj_to_delete);
                break;
            case DataType::kInt:
                delete static_cast<int*>(obj_to_delete);
                break;
            case DataType::kDouble:
                delete static_cast<double*>(obj_to_delete);
                break;
            default:
                throw std::runtime_error("Unknown data type");
        }

        for (unsigned int i = col_idx + 1; i < col_descs_.size(); ++i) {
            row.second[i - 1] = row.second[i];
        }
    }
     col_descs_.erase(col_descs_.begin() + col_idx);
} 

// AddRow function definition
void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
    if (col_data.size() != col_descs_.size()) {
        throw std::invalid_argument("Not equal");
    }

    // Create a new entry into rows_ with the next_unique_id_ mapped to a dynamically allocated array of row_col_capacity_ pointers to void objects
    void** new_row_data = new void*[row_col_capacity_];
    rows_[next_unique_id_] = new_row_data;

    // Insert the dynamically allocated objects storing the col_data into the new row
    unsigned int col_index = 0;
    for (const std::string& data : col_data) {
        switch (col_descs_[col_index].second) {
            case DataType::kString:
                new_row_data[col_index] = static_cast<void*>(new std::string(data));
                break;
            case DataType::kInt:
            // use stoi to convert
                new_row_data[col_index] = static_cast<void*>(new int(std::stoi(data)));
                break;
            case DataType::kDouble:
            // use double to convert
                new_row_data[col_index] = static_cast<void*>(new double(std::stod(data)));
                break;
            default:
                throw std::runtime_error("Unknown data type");
        }
        ++col_index;
    }

    // Increment next_unique_id_
    ++next_unique_id_;
}

//DeleteRowById function definition
void DbTable::DeleteRowById(unsigned int id) {
    auto row_with_id= rows_.find(id);
    if (row_with_id == rows_.end()) {
        throw std::invalid_argument("Row with the given id does not exist");
    }

    // Iterate through the row and release the memory allocated to each object
    void** row_data = row_with_id->second;
    for (unsigned int i = 0; i < col_descs_.size(); ++i) {
        switch (col_descs_[i].second) {
            case DataType::kString:
                delete static_cast<std::string*>(row_data[i]);
                break;
            case DataType::kInt:
                delete static_cast<int*>(row_data[i]);
                break;
            case DataType::kDouble:
                delete static_cast<double*>(row_data[i]);
                break;
            default:
                throw std::runtime_error("Unknown data type");
        }
    }

    // Release memory allocated for the row
    delete[] row_data;

    // Remove the row from the rows_ map
    rows_.erase(id);
}
DbTable::DbTable(const DbTable& rhs) {
    next_unique_id_ = rhs.next_unique_id_;
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;

    for (const auto& pair : rhs.rows_) {
        unsigned int id = pair.first;
        void** row_data = pair.second;

        // Allocate new memory for the row data
        void** new_row_data = new void*[row_col_capacity_]();

        for (unsigned int i = 0; i < col_descs_.size(); ++i) {
                switch (col_descs_[i].second) {
                    case DataType::kString:
                        new_row_data[i] = new std::string(*static_cast<std::string*>(row_data[i]));
                        break;
                    case DataType::kInt:
                        new_row_data[i] = new int(*static_cast<int*>(row_data[i]));
                        break;
                    case DataType::kDouble:
                        new_row_data[i] = new double(*static_cast<double*>(row_data[i]));
                        break;
                    default:
                        throw std::runtime_error("Unknown data type");
            }
        }
        rows_[id] = new_row_data;
    }
}

/*Copy constructor definition
DbTable::DbTable(const DbTable& rhs) {
    next_unique_id_ = rhs.next_unique_id_;
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;

    // Deep copy rows_
    for (const auto& pair : rhs.rows_) {
        unsigned int id = pair.first;
        void** row_data = pair.second;

        // Allocate new memory for the row data
        void** new_row_data = new void*[row_col_capacity_];

        // Copy data from rhs to newRowData
        for (unsigned int i = 0; i < row_col_capacity_; ++i) {
            switch (col_descs_[i].second) {
                case DataType::kString:
                    new_row_data[i] = static_cast<void*>(new std::string(*static_cast<std::string*>(row_data[i])));
                    break;
                case DataType::kInt:
                    new_row_data[i] = static_cast<void*>(new int(*static_cast<int*>(row_data[i])));
                    break;
                case DataType::kDouble:
                    new_row_data[i] = static_cast<void*>(new double(*static_cast<double*>(row_data[i])));
                    break;
                default:
                    throw std::runtime_error("Unknown data type");
            }
        }

        // Insert the deep copied row into the new DbTable object
        rows_[id] = new_row_data;
    }
    // Table: copy constructor and table with (m rows; n columns)
}*/

//Assignment operator definition
DbTable& DbTable::operator=(const DbTable& rhs) {
    if (this == &rhs) return *this;
    for (auto& pair : rows_) {
        void** row_data = pair.second;
        for (unsigned int i = 0; i < col_descs_.size(); ++i) {
            switch (col_descs_[i].second) {
                case DataType::kString: delete static_cast<std::string*>(row_data[i]); break;
                case DataType::kInt: delete static_cast<int*>(row_data[i]); break;
                case DataType::kDouble: delete static_cast<double*>(row_data[i]); break;
                default: throw std::runtime_error("Unknown data type");
            }
        }
        delete[] row_data;
    }
    rows_.clear();
    next_unique_id_ = rhs.next_unique_id_; // cannot update before deleting original data, would cause issues
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;
    for (const auto& pair : rhs.rows_) {
        unsigned int id = pair.first;
        void** row_data = pair.second;
        void** new_row_data = new void*[row_col_capacity_];
        for (unsigned int i = 0; i < col_descs_.size(); ++i) {
            switch (col_descs_[i].second) {
                case DataType::kString: new_row_data[i] = new std::string(*static_cast<std::string*>(row_data[i])); break;
                case DataType::kInt: new_row_data[i] = new int(*static_cast<int*>(row_data[i])); break;
                case DataType::kDouble: new_row_data[i] = new double(*static_cast<double*>(row_data[i])); break;
                default: throw std::runtime_error("Unknown data type");
            }
        }
        rows_[id] = new_row_data;
    }
    return *this;
}


// Destructor definition
DbTable::~DbTable() noexcept {
    // Release memory for every row
    for (auto& pair : rows_) {
        void** row_data = pair.second;

        // Deallocate memory for each column in the row
        for (unsigned int i = 0; i < col_descs_.size(); ++i) {
            switch (col_descs_[i].second) {
                case DataType::kString:
                    delete static_cast<std::string*>(row_data[i]);
                    break;
                case DataType::kInt:
                    delete static_cast<int*>(row_data[i]);
                    break;
                case DataType::kDouble:
                    delete static_cast<double*>(row_data[i]);
                    break;
                default:
                    throw std::runtime_error("Unknown data type");
            }
        }

        // Deallocate memory for the row itself
        delete[] row_data;
    }

    // Clear the rows_ map
    rows_.clear();

     std::cout << "Table: copy constructor and table with (" << rows_.size() << " rows; " << col_descs_.size() << " columns)\n";
}


// Friend function definition
std::ostream& operator<<(std::ostream& os, const DbTable& table) {
    for (size_t i = 0; i < table.col_descs_.size(); ++i) {
        os << table.col_descs_[i].first << "(";
        switch (table.col_descs_[i].second) {
            case DataType::kString: os << "std::string"; break;
            case DataType::kInt: os << "int"; break;
            case DataType::kDouble: os << "double"; break;
            default: os << "unknown"; break;
        }
        os << ")" << (i < table.col_descs_.size() - 1 ? ", " : "\n");
    }
    for (const auto& pair : table.rows_) {
        for (size_t i = 0; i < table.col_descs_.size(); ++i) {
            void* cell = pair.second[i];
            switch (table.col_descs_[i].second) {
                case DataType::kString: os << *(static_cast<std::string*>(cell)); break;
                case DataType::kInt: os << *(static_cast<int*>(cell)); break;
                case DataType::kDouble: os << *(static_cast<double*>(cell)); break;
                default: os << "unknown"; break;
            }
            os << (i < table.col_descs_.size() - 1 ? ", " : "\n");
        }
    }
    return os;
}




// const unsigned int kRowGrowthRate = 2;*/
