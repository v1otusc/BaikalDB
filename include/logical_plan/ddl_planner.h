// Copyright (c) 2018-present Baidu, Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Brief:  the class for generating and executing DDL SQL
#pragma once
#include "logical_planner.h"
#include "query_context.h"
#include "parser.h"

namespace baikaldb {

class DDLPlanner : public LogicalPlanner {
public:

    DDLPlanner(QueryContext* ctx) : LogicalPlanner(ctx) {}

    virtual ~DDLPlanner() {}

    virtual int plan();

private:
    int parse_create_table(pb::SchemaInfo& table);
    int parse_drop_table(pb::SchemaInfo& table);
    int parse_restore_table(pb::SchemaInfo& table);
    int parse_create_database(pb::DataBaseInfo& database);
    int parse_drop_database(pb::DataBaseInfo& database);
    int parse_alter_table(pb::MetaManagerRequest& alter_request);

    int add_column_def(pb::SchemaInfo& table, parser::ColumnDef* column);
    int add_constraint_def(pb::SchemaInfo& table, parser::Constraint* constraint,parser::AlterTableSpec* spec);
    bool is_fulltext_type_constraint(pb::StorageType pb_storage_type, bool& has_arrow_type, bool& has_pb_type) const;
    pb::PrimitiveType to_baikal_type(parser::FieldType* field_type);
    int parse_pre_split_keys(std::string split_start_key,
                             std::string split_end_key,
                             std::string global_start_key,
                             std::string global_end_key,
                             int32_t split_region_num, pb::SchemaInfo& table);
    int pre_split_index(const std::string& start_key,
                        const std::string& end_key,
                        int32_t region_num,
                        pb::SchemaInfo& table,
                        const pb::IndexInfo* pk_index,
                        const pb::IndexInfo* index,
                        const std::vector<const pb::FieldInfo*>& pk_fields,
                        const std::vector<const pb::FieldInfo*>& index_fields);
    int parse_modify_column(pb::MetaManagerRequest& alter_request,
                              const parser::TableName* table_name,
                              const parser::AlterTableSpec* alter_spec);
    std::map<std::string, bool> _column_can_null;
};
} //namespace baikal
