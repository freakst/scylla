/*
 * Copyright (C) 2020-present ScyllaDB
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

#pragma once

#include <seastar/util/noncopyable_function.hh>

#include "feed_writers.hh"

namespace mutation_writer {

// Given a producer that may contain data for all shards, consume it in a per-shard
// manner. This is useful, for instance, in the resharding process where a user changes
// the amount of CPU assigned to Scylla and we have to rewrite the SSTables to their new
// owners.
future<> segregate_by_shard(flat_mutation_reader_v2 producer, reader_consumer_v2 consumer);

} // namespace mutation_writer
