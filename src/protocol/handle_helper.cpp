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

#include "handle_helper.h"
#include "query_context.h"
#include "store_interact.hpp"

namespace baikaldb {
void HandleHelper::init() {
    _calls[SQL_HANDLE_ABNORMAL_REGIONS] = std::bind(&HandleHelper::_handle_abnormal_regions,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_ADD_PRIVILEGE] = std::bind(&HandleHelper::_handle_add_privilege,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_TABLE_RESOURCE_TAG] = std::bind(&HandleHelper::_handle_table_resource_tag,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_LOAD_BALANCE_SWITCH] = std::bind(&HandleHelper::_handle_load_balance_switch,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_MIGRATE_SWITCH] = std::bind(&HandleHelper::_handle_load_balance_switch,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_DROP_INSTANCE] = std::bind(&HandleHelper::_handle_drop_instance,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_TABLE_MAIN_LOGICAL_ROOM] = std::bind(&HandleHelper::_handle_table_main_logical_room,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_DROP_REGION] = std::bind(&HandleHelper::_handle_drop_region,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_SPLIT_LINES] = std::bind(&HandleHelper::_handle_split_lines,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_TTL_DURATION] = std::bind(&HandleHelper::_handle_ttl_duration,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_SPLIT_REGION] = std::bind(&HandleHelper::_handle_split_region,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_RM_PRIVILEGE] = std::bind(&HandleHelper::_handle_rm_privilege,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_DELETE_DDL] = std::bind(&HandleHelper::_handle_ddlwork,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_RESTART_DDL] = std::bind(&HandleHelper::_handle_ddlwork,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_SUSPEND_DDL] = std::bind(&HandleHelper::_handle_ddlwork,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_UPDATE_DISTS] = std::bind(&HandleHelper::_handle_update_dists,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_STORE_RM_REGION] = std::bind(&HandleHelper::_handle_store_rm_region,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_STORE_ADD_PEER] = std::bind(&HandleHelper::_handle_store_add_peer,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_STORE_RM_PEER] = std::bind(&HandleHelper::_handle_store_rm_peer,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_STORE_SET_PEER] = std::bind(&HandleHelper::_handle_store_set_peer,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_STORE_TRANS_LEADER] = std::bind(&HandleHelper::_handle_store_trans_leader,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_ADD_USER] = std::bind(&HandleHelper::_handle_add_user,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_COPY_DB] = std::bind(&HandleHelper::_handle_copy_db,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_LINK_BINLOG] = std::bind(&HandleHelper::_handle_binlog,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_UNLINK_BINLOG] = std::bind(&HandleHelper::_handle_binlog,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_INSTANCE_PARAM] = std::bind(&HandleHelper::_handle_instance_param,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_SCHEMA_CONF] =  std::bind(&HandleHelper::_handle_schema_conf,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_MIGRATE_INSTANCE] = std::bind(&HandleHelper::_handle_migrate_instance,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_INSTANCE_STATUS] = std::bind(&HandleHelper::_handle_instance_status,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_STORE_COMPACT_REGION] = std::bind(&HandleHelper::_handle_store_compact_region,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_CREATE_NAMESPACE] = std::bind(&HandleHelper::_handle_create_namespace,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_NETWORK_BALANCE] = std::bind(&HandleHelper::_handle_load_balance_switch,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_STORE_RM_TXN] = std::bind(&HandleHelper::_handle_store_rm_txn,
            this, std::placeholders::_1, std::placeholders::_2);
    _calls[SQL_HANDLE_REGION_ADJUSTKEY] = std::bind(&HandleHelper::_handle_region_adjustkey,
            this, std::placeholders::_1, std::placeholders::_2);
    _wrapper = MysqlWrapper::get_instance();
}

bool HandleHelper::execute(const SmartSocket& client) {
    if (client == nullptr || client->query_ctx == nullptr) {
        DB_FATAL("param invalid");
        //client->state = STATE_ERROR;
        return false;
    }

    std::vector<std::string> split_vec;
    boost::split(split_vec, client->query_ctx->sql,
                 boost::is_any_of(" \t\n\r"), boost::token_compress_on);
    if (split_vec.size() < 2) {
        _wrapper->make_simple_ok_packet(client);
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    std::transform(split_vec[1].begin(), split_vec[1].end(), split_vec[1].begin(), ::tolower);
    auto iter = _calls.find(split_vec[1]);
    if (iter == _calls.end() || iter->second == nullptr) {
        _wrapper->make_simple_ok_packet(client);
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    return iter->second(client, split_vec);
}

bool HandleHelper::_handle_abnormal_regions(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    static std::map<std::string, pb::RecoverOpt> opt_map = {
            {"remove_illegal_peer", pb::DO_REMOVE_ILLEGAL_PEER},
            {"remove_error_peer", pb::DO_REMOVE_PEER},
            {"set_peer", pb::DO_SET_PEER},
            {"init_peer", pb::DO_INIT_REGION}
    };

    std::string opt = "";
    std::string resource_tag = "";
    if (split_vec.size() == 4) {
        opt = split_vec[3];
    } else if (split_vec.size() == 5) {
        opt = split_vec[3];
        resource_tag = split_vec[4];
    } else {
        client->state = STATE_ERROR;
        DB_FATAL("param invalid");
        return false;
    }
    auto iter = opt_map.find(opt);
    if (iter == opt_map.end()) {
        DB_WARNING("param invalid opt:%s", opt.c_str());
        client->state = STATE_ERROR;
        return false;
    }
    pb::RecoverOpt recover_opt = iter->second;
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_RECOVERY_ALL_REGION);
    request.set_recover_opt(recover_opt);
    if (resource_tag != "") {
        request.add_resource_tags(resource_tag);
    }
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    std::vector< std::vector<std::string> > rows;
    rows.reserve(10);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if (response.has_recover_response()) {
        auto&  recover_response = response.recover_response();
        std::vector<pb::PeerStateInfo> recover_region_way;
        recover_region_way.reserve(5);
        for (auto& peer_info : recover_response.illegal_regions()) {
            recover_region_way.emplace_back(peer_info);
        }
        for (auto& peer_info : recover_response.set_peer_regions()) {
            recover_region_way.emplace_back(peer_info);
        }
        for (auto& peer_info : recover_response.inited_regions()) {
            recover_region_way.emplace_back(peer_info);
        }
        for (auto& peer_info : recover_region_way) {
            std::vector<std::string> row;
            row.emplace_back(std::to_string(peer_info.table_id()));
            row.emplace_back(std::to_string(peer_info.region_id()));
            row.emplace_back(opt);
            row.emplace_back(peer_info.peer_id() + "@" + pb::PeerStatus_Name(peer_info.peer_status()));
            rows.emplace_back(row);
        }
    }

    std::vector<std::string> names = {"table_id", "region_id", "handle_region_opt", "peer"};
    std::vector<ResultField> fields;
    fields.reserve(4);
    for (auto& name : names) {
        ResultField field;
        field.name = name;
        field.type = MYSQL_TYPE_STRING;
        fields.emplace_back(field);
    }
    // Make mysql packet.
    if (_make_common_resultset_packet(client, fields, rows) != 0) {
        DB_FATAL_CLIENT(client, "Failed to make result packet.");
        _wrapper->make_err_packet(client, ER_MAKE_RESULT_PACKET, "Failed to make result packet.");
        client->state = STATE_ERROR;
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_add_privilege(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if (!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }
    std::string db = "";
    pb::RW rw  = pb::WRITE;
    if (split_vec.size() == 4) {
        db = split_vec[2];
        if (boost::iequals(split_vec[3], "READ")) {
            rw = pb::READ;
        }
    } else {
        client->state = STATE_ERROR;
        DB_FATAL("param invalid");
        return false;
    }
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_ADD_PRIVILEGE);
    auto pri = request.mutable_user_privilege();
    pri->set_username(client->user_info->username);
    pri->set_namespace_name(client->user_info->namespace_);
    auto add_db = pri->add_privilege_database();
    add_db->set_database(db);
    add_db->set_database_rw(rw);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_table_resource_tag(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }
    std::string namespace_name = client->user_info->namespace_;
    std::string db = client->current_db;
    std::string table = "";
    std::string resource_tag = "";
    if (split_vec.size() == 3) {
        table = split_vec[2];
    } else if (split_vec.size() == 4) {
        table = split_vec[2];
        resource_tag = split_vec[3];
    } else {
        client->state = STATE_ERROR;
        DB_FATAL("param invalid");
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_MODIFY_RESOURCE_TAG);
    auto info = request.mutable_table_info();
    info->set_table_name(table);
    info->set_database(db);
    info->set_namespace_name(namespace_name);
    info->set_resource_tag(resource_tag);

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);

    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_load_balance_switch(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }

    static std::map<std::string, pb::OpType> opt_map = {
            {"load_balance_open", pb::OP_OPEN_LOAD_BALANCE},
            {"load_balance_close", pb::OP_CLOSE_LOAD_BALANCE},
            {"migrate_open", pb::OP_OPEN_MIGRATE},
            {"migrate_close", pb::OP_CLOSE_MIGRATE},
            {"network_balance_open", pb::OP_OPEN_NETWORK_SEGMENT_BALANCE},
            {"network_balance_close", pb::OP_CLOSE_NETWORK_SEGMENT_BALANCE},
    };
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    std::string _switch = "";
    if (split_vec.size() == 3) {
        _switch = split_vec[2];
    } else if (split_vec.size() == 4) {
        request.add_resource_tags(split_vec[2]);
        _switch = split_vec[3];
    } else {
        client->state = STATE_ERROR;
        DB_FATAL("param invalid");
        return false;
    }

    _switch = split_vec[1] + "_" + _switch;
    std::transform(_switch.begin(), _switch.end(), _switch.begin(), ::tolower);
    if (opt_map.find(_switch) == opt_map.end()) {
        client->state = STATE_ERROR;
        DB_FATAL("param invalid");
        return false;
    }
    request.set_op_type(opt_map[_switch]);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_create_namespace(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || split_vec.size() != 3) {
        DB_FATAL("param invalid");
        return false;
    }

    std::string namespace_ = split_vec[2];
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_CREATE_NAMESPACE);
    auto info = request.mutable_namespace_info();
    info->set_namespace_name(namespace_);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_drop_instance(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || split_vec.size() != 3) {
        DB_FATAL("param invalid");
        return false;
    }

    std::string address = split_vec[2];
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_DROP_INSTANCE);
    auto info = request.mutable_instance();
    info->set_address(address);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_table_main_logical_room(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }
    std::string namespace_name = client->user_info->namespace_;
    std::string db = client->current_db;
    std::string table_name = "";
    std::string main_logical_room = "";
    if (split_vec.size() == 4) {
        table_name = split_vec[2];
        main_logical_room = split_vec[3];
    } else {
        client->state = STATE_ERROR;
        DB_FATAL("param invalid");
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_UPDATE_MAIN_LOGICAL_ROOM);
    auto info = request.mutable_table_info();
    info->set_table_name(table_name);
    info->set_database(db);
    info->set_namespace_name(namespace_name);
    info->set_main_logical_room(main_logical_room);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_drop_region(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || split_vec.size() < 3) {
        DB_FATAL("param invalid");
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_DROP_REGION);
    auto info = request.mutable_drop_region_ids();
    for(size_t i = 2; i < split_vec.size(); ++i) {
        int64_t region_id = strtoll(split_vec[i].c_str(), NULL, 10);
        info->Add(region_id);
    }
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_split_lines(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || split_vec.size() != 4) {
        DB_FATAL("param invalid");
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_UPDATE_SPLIT_LINES);
    auto info = request.mutable_table_info();
    info->set_table_name(split_vec[2]);
    info->set_database(client->current_db);
    info->set_namespace_name(client->user_info->namespace_);
    int64_t split_lines = strtoll(split_vec[3].c_str(), NULL, 10);
    info->set_region_split_lines(split_lines);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_ttl_duration(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || split_vec.size() != 4) {
        DB_FATAL("param invalid");
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_UPDATE_TTL_DURATION);
    auto info = request.mutable_table_info();
    info->set_table_name(split_vec[2]);
    info->set_database(client->current_db);
    info->set_namespace_name(client->user_info->namespace_);
    int64_t ttl = strtoll(split_vec[3].c_str(), NULL, 10);
    if (ttl <= 0) { 
        return false; 
    }
    info->set_ttl_duration(ttl);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_split_region(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }
    std::string split_key = "not used";
    int64_t region;
    if (split_vec.size() == 3) {
        region = strtoll(split_vec[2].c_str(), NULL, 10);
    } else if (split_vec.size() == 4) {
        region = strtoll(split_vec[2].c_str(), NULL, 10);
        split_key = split_key[3];
    } else {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_SPLIT_REGION);
    auto info = request.mutable_region_split();
    info->set_region_id(region);
    info->set_split_key(split_key);
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_rm_privilege(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }
    std::string db = "";
    std::string table = "";
    if (split_vec.size() == 3) {
        db = split_vec[2];
    } else if (split_vec.size() == 4) {
        db = split_vec[2];
        table = split_vec[3];
    } else {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_DROP_PRIVILEGE);
    auto info = request.mutable_user_privilege();
    info->set_username(client->user_info->username);
    info->set_password(client->user_info->password);
    info->set_namespace_name(client->user_info->namespace_);
    if (table == "") {
        pb::PrivilegeDatabase privilege_database;
        privilege_database.set_database(db);
        auto db_info = info->add_privilege_database();
        *db_info = privilege_database;
    } else {
        pb::PrivilegeTable privilege_table;
        privilege_table.set_database(db);
        privilege_table.set_table_name(table);
        auto table_info = info->add_privilege_table();
        *table_info = privilege_table;
    }
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_ddlwork(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    static std::map<std::string, pb::OpType> opt_map = {
            {"delete_ddl", pb::OP_DELETE_DDLWORK},
            {"suspend_ddl", pb::OP_SUSPEND_DDL_WORK},
            {"restart_ddl", pb::OP_RESTART_DDL_WORK},
    };
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !client->user_info || !factory) {
        DB_FATAL("param invalid");
        return false;
    }

    bool delete_global_ddl = false;
    if (split_vec.size() == 4 && boost::iequals(split_vec[1], "delete_ddl") &&
        boost::iequals(split_vec[3], "global")) {
        delete_global_ddl = true;
    } else if (split_vec.size() != 3) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    const std::string& table_name = split_vec[2];
    std::string full_name = client->user_info->namespace_+ "." + client->current_db + "." + table_name;
    int64_t table_id;
    if (factory->get_table_id(full_name, table_id) != 0) {
        DB_FATAL("param invalid, no such table with table name: %s", full_name.c_str());
        client->state = STATE_ERROR;
        return false;
    }
    DB_WARNING("table_name: %s, table_id: %ld", table_name.c_str(), table_id);
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(opt_map[split_vec[1]]);
    if (boost::iequals(split_vec[1], "delete_ddl")) {
        auto info = request.mutable_ddlwork_info();
        info->set_table_id(table_id);
        info->set_global(delete_global_ddl);
    } else {
        auto info = request.mutable_index_ddl_request();
        info->set_table_id(table_id);
    }
    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_update_dists(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || !client->query_ctx || split_vec.size() < 3) {
        DB_FATAL("param invalid");
        return false;
    }

    std::string json = client->query_ctx->sql;
    auto start = json.find_first_of('{');
    auto end = json.find_last_of('}');
    if(start != std::string::npos && end != std::string::npos) {
        json = json.substr(start, end - start + 1);
    } else {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    pb::SchemaInfo table_info;
    std::string error_info = json2pb(json, &table_info);
    if (error_info != "") {
        DB_FATAL("parse json failed: %s, with error: %s", json.c_str(), error_info.c_str());
        client->state = STATE_ERROR;
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_UPDATE_DISTS);
    auto info = request.mutable_table_info();
    *info = table_info;

    if (table_info.has_main_logical_room()) {
        bool find_main_logical_room = false;
        for(auto dist : table_info.dists()) {
            if (boost::equals(dist.logical_room(), table_info.main_logical_room())) {
                find_main_logical_room = true;
                break;
            }
        }
        if (!find_main_logical_room) {
            DB_WARNING("main_logical_room not match: %s", table_info.main_logical_room().c_str());
            client->state = STATE_ERROR;
            return false;
        }
    }

    if (table_info.has_replica_num()) {
        int replicas = 0;
        for(auto dist : table_info.dists()) {
            replicas += dist.count();
        }
        if (table_info.replica_num() != replicas) {
            DB_WARNING("replica_num not match");
            client->state = STATE_ERROR;
            return false;
        }
    }

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_store_rm_region(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle store_rm_region leader regionID (no_delay)  (force)
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx || split_vec.size() < 4) {
        DB_FATAL("param invalid");
        return false;
    }
    std::string store_addr = split_vec[2];
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);

    bool delay = true, force = false;
    if (client->query_ctx->sql.find("no_delay") != std::string::npos) {
        delay = false;
    }
    if (client->query_ctx->sql.find("force") != std::string::npos) {
        force = true;
    }

    pb::RemoveRegion req;
    req.set_region_id(region_id);
    if (force) {
        req.set_force(force);
    }
    req.set_need_delay_drop(delay);
    pb::StoreRes res;
    StoreInteract interact(store_addr);
    interact.send_request("remove_region", req, res);
    DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), res.ShortDebugString().c_str());
    if(!_make_response_packet(client, res.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_store_add_peer(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle store_add_peer tableID regionID address
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx || split_vec.size() != 5) {
        DB_FATAL("param invalid");
        return false;
    }
    int64_t table_id = strtoll(split_vec[2].c_str(), NULL, 10);
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);
    std::string new_peer = split_vec[4];

    pb::AddPeer req;
    req.set_region_id(region_id);
    pb::RegionInfo info;
    if (factory->get_region_info(table_id, region_id, info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", region_id, table_id);
        client->state = STATE_ERROR;
        return false;
    }
    for(auto peer : info.peers()) {
        req.add_new_peers(peer);
        req.add_old_peers(peer);
    }
    req.add_new_peers(new_peer);

    int retry_time = 3;
    std::string leader = info.leader();
    while(retry_time-- > 0) {
        pb::StoreRes res;
        StoreInteract interact(leader);
        interact.send_request("add_peer", req, res);
        DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), res.ShortDebugString().c_str());
        if (res.errcode() == pb::NOT_LEADER && res.has_leader() &&
            !res.leader().empty() && res.leader() != "0.0.0.0:0") {
            leader = res.leader();
            info.set_leader(leader);
            factory->update_leader(info);
            continue;
        }
        if(!_make_response_packet(client, res.ShortDebugString())) {
            return false;
        }
        break;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_send_store_raft_control_request(const SmartSocket& client, pb::RaftControlRequest& req,
                                                    pb::RegionInfo& info) {
    int retry_time = 3;
    std::string leader = info.leader();
    while(retry_time-- > 0) {
        pb::RaftControlResponse res;
        StoreInteract interact(leader);
        interact.send_request("region_raft_control", req, res);
        DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), res.ShortDebugString().c_str());
        if (res.errcode() == pb::NOT_LEADER && res.has_leader() &&
            !res.leader().empty() && res.leader() != "0.0.0.0:0") {
            leader = res.leader();
            info.set_leader(leader);
            auto factory = SchemaFactory::get_instance();
            if (factory != nullptr) {
                factory->update_leader(info);
            }
            continue;
        }
        if(!_make_response_packet(client, res.ShortDebugString())) {
            return false;
        }
        break;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_store_rm_peer(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle store_rm_peer tableID regionID address (force)
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx) {
        DB_FATAL("param invalid");
        return false;
    }
    bool is_force = false;
    if (split_vec.size() == 6 && boost::iequals(split_vec[5], "force")) {
        is_force = true;
    } else if (split_vec.size() != 5) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }
    int64_t table_id = strtoll(split_vec[2].c_str(), NULL, 10);
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);
    std::string delete_peer = split_vec[4];
    int peers_count = 0;
    bool found_delete_peer = false;

    pb::RaftControlRequest req;
    req.set_op_type(pb::SetPeer);
    req.set_region_id(region_id);
    pb::RegionInfo info;
    if (factory->get_region_info(table_id, region_id, info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", region_id, table_id);
        if(!_make_response_packet(client, "no such regionID")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }

    for(auto peer : info.peers()) {
        req.add_old_peers(peer);
        if (peer != delete_peer) {
            req.add_new_peers(peer);
        } else {
            found_delete_peer = true;
        }
        ++peers_count;
    }
    if (!found_delete_peer || (peers_count <= info.replica_num() && !is_force)) {
        if(!_make_response_packet(client, "not found delete_peer OR peers_count <= replica_num")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    return _send_store_raft_control_request(client, req, info);
}

bool HandleHelper::_handle_store_set_peer(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle store_set_peer tableID regionID address
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx) {
        DB_FATAL("param invalid");
        return false;
    }
    bool is_force = false;
    if (split_vec.size() == 6 && boost::iequals(split_vec[5], "force")) {
        is_force = true;
    } else if (split_vec.size() != 5) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    int64_t table_id = strtoll(split_vec[2].c_str(), NULL, 10);
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);
    const std::string& only_one_peer = split_vec[4];
    bool found_only_one_peer = false;

    pb::RaftControlRequest req;
    req.set_op_type(pb::SetPeer);
    req.set_region_id(region_id);
    req.set_force(is_force);
    pb::RegionInfo info;
    if (factory->get_region_info(table_id, region_id, info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", region_id, table_id);
        if(!_make_response_packet(client, "no such regionID")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }

    for(auto peer : info.peers()) {
        req.add_old_peers(peer);
        if (only_one_peer == peer) {
            found_only_one_peer = true;
        }
    }
    req.add_new_peers(only_one_peer);
    if (!found_only_one_peer) {
        if(!_make_response_packet(client, "not found peer in raft")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    pb::RaftControlResponse res;
    StoreInteract interact(only_one_peer);
    interact.send_request("region_raft_control", req, res);
    DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), res.ShortDebugString().c_str());
    if(!_make_response_packet(client, res.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_store_trans_leader(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle store_trans_leader tableID regionID TransLeaderAddress
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx) {
        DB_FATAL("param invalid");
        return false;
    }
    if (split_vec.size() != 5) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    int64_t table_id = strtoll(split_vec[2].c_str(), NULL, 10);
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);
    const std::string& new_leader = split_vec[4];
    bool found_new_leader = false;

    pb::RaftControlRequest req;
    req.set_op_type(pb::TransLeader);
    req.set_region_id(region_id);
    pb::RegionInfo info;
    if (factory->get_region_info(table_id, region_id, info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", region_id, table_id);
        if(!_make_response_packet(client, "no such regionID")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }

    for(auto peer : info.peers()) {
        req.add_old_peers(peer);
        if (new_leader == peer) {
            found_new_leader = true;
        }
    }
    req.set_new_leader(new_leader);
    if (!found_new_leader) {
        if(!_make_response_packet(client, "not found new leader in raft group")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    return _send_store_raft_control_request(client, req, info);
}

bool HandleHelper::_handle_add_user(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || !client->query_ctx || split_vec.size() < 3) {
        DB_FATAL("param invalid");
        return false;
    }

    std::string json = "";
    json = client->query_ctx->sql;
    auto start = json.find_first_of('{');
    auto end = json.find_last_of('}');
    if(start != std::string::npos && end != std::string::npos) {
        json = json.substr(start, end - start + 1);
    } else {
        DB_FATAL("json param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    pb::UserPrivilege user_info;
    if (json2pb(json, &user_info) != "") {
        DB_FATAL("parse json failed: %s", json.c_str());
        client->state = STATE_ERROR;
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_CREATE_USER);
    auto info = request.mutable_user_privilege();
    *info = user_info;

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_copy_db(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || !client->query_ctx) {
        DB_FATAL("param invalid");
        return false;
    }
    if (split_vec.size() < 4) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    std::string orgin_db = split_vec[2];
    std::string desc_db = split_vec[3];
    std::string ns = client->user_info->namespace_;
    SchemaFactory* factory = SchemaFactory::get_instance();
    int64_t db_id = 0;
    if (factory->get_database_id(ns + "." + orgin_db, db_id) != 0) {
        client->state = STATE_ERROR;
        DB_WARNING("orgin db not exist : %s", orgin_db.c_str());
        return false;
    }
    if (factory->get_database_id(ns + "." + desc_db, db_id) == 0) {
        client->state = STATE_ERROR;
        DB_WARNING("desc db exist : %s", desc_db.c_str());
        return false;
    }

    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_CREATE_DATABASE);
    pb::DataBaseInfo* database = request.mutable_database_info();
    database->set_namespace_name(ns);
    database->set_database(desc_db);
    int ret = MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if (ret != 0 || response.errcode() != pb::SUCCESS) {
        DB_WARNING("send_request fail");
        client->state = STATE_ERROR;
        return false;
    }

    pb::QueryRequest query_request;
    pb::QueryResponse query_response;
    query_request.set_op_type(pb::QUERY_SCHEMA);
    query_request.set_namespace_name(ns);
    query_request.set_database(orgin_db);
    ret = MetaServerInteract::get_instance()->send_request("query", query_request, query_response);
    DB_WARNING("req:%s res:%s", query_request.ShortDebugString().c_str(), query_response.ShortDebugString().c_str());
    if (ret != 0 || query_response.errcode() != pb::SUCCESS) {
        DB_WARNING("send_request fail");
        client->state = STATE_ERROR;
        return false;
    }
    for (auto& schema_info : query_response.schema_infos()) {
        request.Clear();
        response.Clear();
        request.set_op_type(pb::OP_CREATE_TABLE);
        *request.mutable_table_info() = schema_info;
        request.mutable_table_info()->set_database(desc_db);
        request.mutable_table_info()->clear_split_keys();
        for (auto& index : *request.mutable_table_info()->mutable_indexs()) {
            index.clear_field_ids();
        }
        ret = MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
        DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
        if (ret != 0 || response.errcode() != pb::SUCCESS) {
            DB_WARNING("send_request fail");
            client->state = STATE_ERROR;
            return false;
        }
    }
    if (!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_binlog(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if (client == nullptr || client->user_info == nullptr || split_vec.size() < 6) {
        DB_FATAL("param invalid");
        return false;
    }

    const std::string& table_db = split_vec[2];
    const std::string& table_name = split_vec[3];
    const std::string& binlog_table_db = split_vec[4];
    const std::string& binlog_table_name = split_vec[5];
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    if (boost::iequals(split_vec[1], SQL_HANDLE_LINK_BINLOG)) {
        request.set_op_type(pb::OP_LINK_BINLOG);
    } else if (boost::iequals(split_vec[1], SQL_HANDLE_UNLINK_BINLOG)) {
        request.set_op_type(pb::OP_UNLINK_BINLOG);
    } else {
        DB_FATAL("json param invalid");
        client->state = STATE_ERROR;
        return false;
    }
    auto table_info = request.mutable_table_info();
    table_info->set_namespace_name(client->user_info->namespace_);
    table_info->set_database(table_db);
    table_info->set_table_name(table_name);
    if (split_vec.size() > 6) {
        table_info->mutable_link_field()->set_field_name(split_vec[6]);
    }
    auto binlog_info = request.mutable_binlog_info();
    binlog_info->set_namespace_name(client->user_info->namespace_);
    binlog_info->set_database(binlog_table_db);
    binlog_info->set_table_name(binlog_table_name);

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_instance_param(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }

    bool is_meta_param = false;
    if (split_vec.size() == 6) {
        if (split_vec[5] == "true") {
            is_meta_param = true;
        }
    } else if (split_vec.size() != 5) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }
    bool need_delete = false;
    if (split_vec[4] == "delete") {
        need_delete = true;
    }
    std::string key, value, instance = "";
    instance = split_vec[2];
    key = split_vec[3];
    auto trim = [] (std::string str) -> std::string {
        size_t first = str.find_first_not_of(" `\'\"");
        if (first == std::string::npos)
            return "";
        size_t last = str.find_last_not_of(" `\'\"");
        return str.substr(first, (last-first+1));
    };
    value = trim(split_vec[4]);
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_UPDATE_INSTANCE_PARAM);
    auto instance_params = request.mutable_instance_params();
    auto instance_param = instance_params->Add();
    instance_param->set_resource_tag_or_address(instance);
    auto params = instance_param->mutable_params()->Add();
    params->set_key(key);
    params->set_value(value);
    params->set_is_meta_param(is_meta_param);
    params->set_need_delete(need_delete);

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

void handle_signlist(bool is_add, std::set<uint64_t> origin_list, std::string sign, std::string& out_value) {
    uint64_t sign_num = strtoull(sign.c_str(), nullptr, 10);
    if (is_add) {
        origin_list.insert(sign_num);
    } else {
        origin_list.erase(sign_num);
    }

    bool is_first = true;
    for (auto s : origin_list) {
        if (!is_first) {
            out_value += ',';
        }
        out_value += std::to_string(s);
        is_first = false;
    }
}

bool HandleHelper::_handle_schema_conf(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if (!client || !client->user_info) {
        DB_FATAL("param invalid");
        return false;
    }

    if (split_vec.size() != 5) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    std::string key, value, table_name;
    table_name = split_vec[2];
    key = split_vec[3];
    bool is_open = true;
    if (boost::iequals(split_vec[4], "false")) {
        is_open = false;
    }
    // 拿到table_id并检测是否存在改表
    std::string full_name = client->user_info->namespace_+ "." + client->current_db + "." + table_name;
    int64_t table_id;
    SchemaFactory* factory = SchemaFactory::get_instance();
    if (factory->get_table_id(full_name, table_id) != 0) {
        DB_FATAL("param invalid, no such table with table name: %s", full_name.c_str());
        client->state = STATE_ERROR;
        return false;
    }
    
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_UPDATE_SCHEMA_CONF);
    auto table_info = request.mutable_table_info();
    table_info->set_table_name(table_name);
    table_info->set_database(client->current_db);
    table_info->set_namespace_name(client->user_info->namespace_);
    table_info->set_table_id(table_id);
    auto schema_conf = table_info->mutable_schema_conf();

    if (key == "need_merge") {
        schema_conf->set_need_merge(is_open);
    } else if (key == "storage_compute_separate") {
        schema_conf->set_storage_compute_separate(is_open);
    } else if (key == "select_index_by_cost") {
        schema_conf->set_select_index_by_cost(is_open);
    } else if (key == "pk_prefix_balance") {
        int32_t pk_prefix_balance = strtol(split_vec[4].c_str(), NULL, 10);
        schema_conf->set_pk_prefix_balance(pk_prefix_balance);
    } else if (key == "backup_table") {
        int32_t number = pb::BackupTable_descriptor()->FindValueByName(split_vec[4])->number();
        DB_WARNING("backup table enum %s => %d", split_vec[4].c_str(), number);
        schema_conf->set_backup_table(static_cast<pb::BackupTable>(number));
    } else if (key == "in_fast_import") {
        schema_conf->set_in_fast_import(is_open);
        auto table_schema = factory->get_table_info(table_id);
        table_info->set_resource_tag(table_schema.resource_tag);
    } else if (key.find("blacklist") != key.npos || key.find("forcelearner") != key.npos) {
        auto table = factory->get_table_info_ptr(table_id);
        if (table == nullptr) {
            DB_FATAL("table null table name: %s, table_id: %ld", full_name.c_str(), table_id);
            client->state = STATE_ERROR;
            return false;
        }

        bool is_add = key.find("add") != key.npos ? true : false;
        std::string value;
        if (key.find("blacklist") != key.npos) {
            handle_signlist(is_add, table->sign_blacklist, split_vec[4], value);
            schema_conf->set_sign_blacklist(value);
            DB_WARNING("blacklist table_name: %s, table_id: %ld, sings: %s", full_name.c_str(), table_id, value.c_str());
        } else {
            handle_signlist(is_add, table->sign_forcelearner, split_vec[4], value);
            schema_conf->set_sign_forcelearner(value);
            DB_WARNING("forcelearner table_name: %s, table_id: %ld, sings: %s", full_name.c_str(), table_id, value.c_str());
        }
    } else {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_migrate_instance(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if(!client || !client->user_info || split_vec.size() != 3) {
        DB_FATAL("param invalid");
        return false;
    }

    const std::string& instance_address = split_vec[2];
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_SET_INSTANCE_MIGRATE);
    auto instance_info = request.mutable_instance();
    instance_info->set_address(instance_address);

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_instance_status(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    if (!client || !client->user_info || split_vec.size() != 4) {
        DB_FATAL("param invalid");
        return false;
    }

    const std::string& instance_address = split_vec[2];
    pb::Status status = pb::NORMAL;
    if (!pb::Status_Parse(split_vec[3], &status)) {
        DB_FATAL("param invalid, parse status fail");
        return false;
    }
    pb::MetaManagerRequest request;
    pb::MetaManagerResponse response;
    request.set_op_type(pb::OP_SET_INSTANCE_STATUS);
    auto instance_info = request.mutable_instance();
    instance_info->set_address(instance_address);
    instance_info->set_status(status);

    MetaServerInteract::get_instance()->send_request("meta_manager", request, response);
    DB_WARNING("req:%s res:%s", request.ShortDebugString().c_str(), response.ShortDebugString().c_str());
    if(!_make_response_packet(client, response.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_store_compact_region(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle _handle_store_compact_region StoreAddress meta/data/raft_log [regionID]
    std::unordered_map<std::string, int> ops = {
            {"data", 1},  {"meta", 2},  {"raft_log", 3}
    };
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx || split_vec.size() < 4) {
        DB_FATAL("param invalid");
        return false;
    }

    int64_t region_id = -1;
    if (split_vec.size() == 5) {
        region_id = strtoll(split_vec[4].c_str(), NULL, 10);
    } else if (split_vec.size() != 4) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }
    const std::string& store_addr = split_vec[2];
    const std::string& type = split_vec[3];
    pb::RegionIds req;
    if (ops.find(type) != ops.end()) {
        req.set_compact_type(ops[type]);
        if (type == "data" && region_id != -1) {
            req.add_region_ids(region_id);
        }
    } else {
        if(!_make_response_packet(client, "ERROR: compaction type not in [data, meta, raft_log]")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }

    pb::StoreRes res;
    StoreInteract interact(store_addr);
    interact.send_request("compact_region", req, res);
    DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), req.ShortDebugString().c_str());
    if(!_make_response_packet(client, res.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}
    
bool HandleHelper::_handle_store_rm_txn(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle store_rm_txn StoreAddress regionID txnID
    if(!client || !client->query_ctx || split_vec.size() != 5) {
        DB_FATAL("param invalid");
        return false;
    } 
    
    const std::string& store_addr = split_vec[2];
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);
    int64_t txn_id = strtoll(split_vec[4].c_str(), NULL, 10);
    pb::StoreReq req;
    req.set_op_type(pb::OP_TXN_COMPLETE);
    req.set_region_id(region_id);
    req.set_region_version(0);
    req.add_rollback_txn_ids(txn_id);
    req.set_force(true);
    pb::StoreRes res;
    StoreInteract interact(store_addr);
    interact.send_request("query", req, res);
    DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), req.ShortDebugString().c_str());
    if(!_make_response_packet(client, res.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

bool HandleHelper::_handle_region_adjustkey(const SmartSocket& client, const std::vector<std::string>& split_vec) {
    // handle region_adjustkey tableID regionID start_key_region_id end_key_region_id
    SchemaFactory* factory = SchemaFactory::get_instance();
    if(!client || !factory || !client->query_ctx) {
        DB_FATAL("param invalid");
        return false;
    }
    if (split_vec.size() != 6) {
        DB_FATAL("param invalid");
        client->state = STATE_ERROR;
        return false;
    }

    int64_t table_id = strtoll(split_vec[2].c_str(), NULL, 10);
    int64_t region_id = strtoll(split_vec[3].c_str(), NULL, 10);
    int64_t start_region_id = strtoll(split_vec[4].c_str(), NULL, 10);
    int64_t end_region_id = strtoll(split_vec[5].c_str(), NULL, 10);

    pb::RegionInfo info;
    if (factory->get_region_info(table_id, region_id, info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", region_id, table_id);
        if(!_make_response_packet(client, "no such regionid")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    pb::RegionInfo start_info;
    if (factory->get_region_info(table_id, start_region_id, start_info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", start_region_id, table_id);
        if(!_make_response_packet(client, "no such regionid")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }
    pb::RegionInfo end_info;
    if (factory->get_region_info(table_id, end_region_id, end_info) != 0) {
        DB_WARNING("param invalid, no region %ld in table %ld", end_region_id, table_id);
        if(!_make_response_packet(client, "no such regionid")) {
            return false;
        }
        client->state = STATE_READ_QUERY_RESULT;
        return true;
    }

    pb::StoreRes res;
    pb::StoreReq req;
    req.set_op_type(pb::OP_ADJUSTKEY_AND_ADD_VERSION);
    req.set_start_key(start_info.start_key());
    req.set_end_key(end_info.end_key());
    req.set_region_id(region_id);
    req.set_region_version(info.version());
    req.set_force(true);
    StoreInteract interact(info.leader());
    interact.send_request_for_leader("query", req, res);
    DB_WARNING("req:%s res:%s", req.ShortDebugString().c_str(), res.ShortDebugString().c_str());
    if(!_make_response_packet(client, res.ShortDebugString())) {
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

/*
 *  绝大数的handle sql返回的都是与meta交互的response info一个信息
    +--------------------------------------------------------------+
    | result                                                       |
    +--------------------------------------------------------------+
    | errcode: SUCCESS errmsg: "success" op_type: OP_ADD_PRIVILEGE |
    +--------------------------------------------------------------+
*/
bool HandleHelper::_make_response_packet(const SmartSocket& client, const std::string& response) {
    std::vector< std::vector<std::string> > rows;
    std::vector<std::string> row;
    row.emplace_back(response);
    rows.emplace_back(row);
    std::vector<std::string> names = {"result"};
    std::vector<ResultField> fields;
    fields.reserve(1);
    for (auto& name : names) {
        ResultField field;
        field.name = name;
        field.type = MYSQL_TYPE_STRING;
        fields.emplace_back(field);
    }
    // Make mysql packet.
    if (_make_common_resultset_packet(client, fields, rows) != 0) {
        DB_FATAL_CLIENT(client, "Failed to make result packet.");
        _wrapper->make_err_packet(client, ER_MAKE_RESULT_PACKET, "Failed to make result packet.");
        client->state = STATE_ERROR;
        return false;
    }
    client->state = STATE_READ_QUERY_RESULT;
    return true;
}

int HandleHelper::_make_common_resultset_packet(
        const SmartSocket& sock,
        std::vector<ResultField>& fields,
        std::vector< std::vector<std::string> >& rows) {
    if (!sock || !sock->send_buf) {
        DB_FATAL("sock == NULL.");
        return RET_ERROR;
    }
    if (fields.size() == 0) {
        DB_FATAL("Field size is 0.");
        return RET_ERROR;
    }

    //Result Set Header Packet
    int start_pos = sock->send_buf->_size;
    if (!sock->send_buf->byte_array_append_len((const uint8_t *)"\x01\x00\x00\x01", 4)) {
        DB_FATAL("byte_array_append_len failed.");
        return RET_ERROR;
    }
    if (!sock->send_buf->byte_array_append_length_coded_binary(fields.size())) {
        DB_FATAL("byte_array_append_len failed. len:[%lu]", fields.size());
        return RET_ERROR;
    }
    int packet_body_len = sock->send_buf->_size - start_pos - 4;
    sock->send_buf->_data[start_pos] = packet_body_len & 0xFF;
    sock->send_buf->_data[start_pos + 1] = (packet_body_len >> 8) & 0xFF;
    sock->send_buf->_data[start_pos + 2] = (packet_body_len >> 16) & 0xFF;
    sock->send_buf->_data[start_pos + 3] = (++sock->packet_id) & 0xFF;
    // Make field packets
    for (uint32_t cnt = 0; cnt < fields.size(); ++cnt) {
        fields[cnt].catalog = "baikal";
        if (sock->query_ctx != nullptr) {
            fields[cnt].db = sock->query_ctx->cur_db;
        }
        fields[cnt].table.clear();
        fields[cnt].org_table.clear();
        fields[cnt].org_name = fields[cnt].name;
        _wrapper->make_field_packet(sock->send_buf, &fields[cnt], ++sock->packet_id);
    }

    // Make EOF packet
    _wrapper->make_eof_packet(sock->send_buf, ++sock->packet_id);

    // Make row packets
    for (uint32_t cnt = 0; cnt < rows.size(); ++cnt) {
        // Make row data packet
        if (!_wrapper->make_row_packet(sock->send_buf, rows[cnt], ++sock->packet_id)) {
            DB_FATAL("make_row_packet failed");
            return RET_ERROR;
        }
    }
    // Make EOF packet
    _wrapper->make_eof_packet(sock->send_buf, ++sock->packet_id);
    return 0;
}
}
