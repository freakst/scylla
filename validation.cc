/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2015-present ScyllaDB
 *
 * Modified by ScyllaDB
 */

/*
 * This file is part of Scylla.
 *
 * Scylla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Scylla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scylla.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "validation.hh"
#include "replica/database.hh"
#include "exceptions/exceptions.hh"

namespace validation {

/**
 * Based on org.apache.cassandra.thrift.ThriftValidation#validate_key()
 */
std::optional<sstring> is_cql_key_invalid(const schema& schema, partition_key_view key) {
    // C* validates here that the thrift key is not empty.
    // It can only be empty if it is not composite and its only component in CQL form is empty.
    if (schema.partition_key_size() == 1 && key.begin(schema)->empty()) {
        return sstring("Key may not be empty");
    }

    // check that key can be handled by FBUtilities.writeShortByteArray
    auto b = key.representation();
    if (b.size() > max_key_size) {
        return format("Key length of {:d} is longer than maximum of {:d}", b.size(), max_key_size);
    }

    try {
        key.validate(schema);
    } catch (const marshal_exception& e) {
        return sstring(e.what());
    }

    return std::nullopt;
}

void
validate_cql_key(const schema& schema, partition_key_view key) {
    if (const auto err = is_cql_key_invalid(schema, key); err) {
        throw exceptions::invalid_request_exception(std::move(*err));
    }
}

/**
 * Based on org.apache.cassandra.thrift.ThriftValidation#validateColumnFamily(java.lang.String, java.lang.String)
 */
schema_ptr
validate_column_family(data_dictionary::database db, const sstring& keyspace_name, const sstring& cf_name) {
    validate_keyspace(db, keyspace_name);

    if (cf_name.empty()) {
        throw exceptions::invalid_request_exception("non-empty table is required");
    }

    try {
        return db.find_schema(keyspace_name, cf_name);
    } catch (...) {
        throw exceptions::invalid_request_exception(format("unconfigured table {}", cf_name));
    }
}

void validate_keyspace(data_dictionary::database db, const sstring& keyspace_name) {
    if (keyspace_name.empty()) {
        throw exceptions::invalid_request_exception("Keyspace not set");
    }

    if (!db.has_keyspace(keyspace_name)) {
        throw exceptions::keyspace_not_defined_exception(format("Keyspace {} does not exist", keyspace_name));
    }
}

}
