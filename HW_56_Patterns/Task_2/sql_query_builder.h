#pragma once

#include <string>
#include <vector>
#include <map>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns_;
    std::string from_table_;
    std::map<std::string, std::string> where_conditions_;

public:
    SqlSelectQueryBuilder() = default;

    // ���������� �������� ��� �������
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns_.push_back(column);
        return *this;
    }

    // ���������� ���������� �������� �� ���� �����
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) {
        for (const auto& column : columns) {
            columns_.push_back(column);
        }
        return *this;
    }

    // ��������� ������� ��� ������� (�������������� ���������� ��������)
    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        from_table_ = table;
        return *this;
    }

    // ���������� ������� WHERE
    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_conditions_[column] = value;
        return *this;
    }
    
    // ���������� ���������� ������� WHERE
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept{
        for (const auto& condition : kv) {
            where_conditions_[condition.first] = condition.second;
        }
        return *this;
    }
    
    // ���������� ��������� SQL-�������
    std::string BuildQuery() {
        std::string query = "SELECT ";

        // ��������� ��������
        if (columns_.empty()) {
            query += "*";
        }
        else {
            for (size_t i = 0; i < columns_.size(); ++i) {
                query += columns_[i];
                if (i < columns_.size() - 1) {
                    query += ", ";
                }
            }
        }

        // ���� ������� �� �������, ���������� ������ �������
        query += " FROM ";
        if (from_table_.empty()) {
            query += "table"; // �������� �� ���������
        }
        else {
            query += from_table_;
        }

        // ��������� ������� WHERE
        if (!where_conditions_.empty()) {
            query += " WHERE ";
            bool first_condition = true;

            for (const auto& condition : where_conditions_) {
                if (!first_condition) {
                    query += " AND ";
                }
                query += condition.first + "=" + condition.second;
                first_condition = false;
            }
        }

        query += ";";
        return query;
    }
};
