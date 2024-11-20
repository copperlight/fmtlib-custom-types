#include "absl/container/flat_hash_map.h"
#include <ostream>

namespace spectator {

class Tags {
    using table_t = absl::flat_hash_map<std::string, std::string>;
    table_t entries_;

public:
    Tags() = default;

    Tags(std::initializer_list<std::pair<absl::string_view, absl::string_view>> vs) {
        for (auto& pair : vs) {
            add(pair.first, pair.second);
        }
    }

    template <typename Cont>
    static Tags from(Cont&& cont) {
        Tags tags;
        tags.entries_.reserve(cont.size());
        for (auto&& kv : cont) {
            tags.add(kv.first, kv.second);
        }
        return tags;
    }

    void add(absl::string_view k, absl::string_view v) {
        entries_[k] = std::string(v);
    }

    [[nodiscard]] size_t hash() const {
        using hs = std::hash<std::string>;
        size_t h = 0;
        for (const auto& entry : entries_) {
            h += (hs()(entry.first) << 1U) ^ hs()(entry.second);
        }
        return h;
    }

    void move_all(Tags&& source) {
        entries_.insert(std::make_move_iterator(source.begin()),
                        std::make_move_iterator(source.end()));
    }

    bool operator==(const Tags& that) const { return that.entries_ == entries_; }

    [[nodiscard]] bool has(absl::string_view key) const {
        return entries_.find(key) != entries_.end();
    }

    [[nodiscard]] std::string at(absl::string_view key) const {
        auto entry = entries_.find(key);
        if (entry != entries_.end()) {
            return entry->second;
        }
        return {};
    }

    [[nodiscard]] size_t size() const { return entries_.size(); }

    [[nodiscard]] table_t::const_iterator begin() const {
        return entries_.begin();
    }

    [[nodiscard]] table_t::const_iterator end() const { return entries_.end(); }
};

inline std::ostream& operator<<(std::ostream& os, const Tags& tags) {
    os << fmt::format("{}", tags);
    return os;
}

}  // namespace spectator
