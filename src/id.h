#include "absl/container/flat_hash_map.h"
#include <ostream>

namespace spectator {

class Id {
public:
    Id(absl::string_view name, Tags tags) noexcept
        : name_(name), tags_(std::move(tags)), hash_(0u) {}

    static std::shared_ptr<Id> of(absl::string_view name, Tags tags = {}) {
        return std::make_shared<Id>(name, std::move(tags));
    }

    bool operator==(const Id& rhs) const noexcept {
        return name_ == rhs.name_ && tags_ == rhs.tags_;
    }

    const std::string& Name() const noexcept { return name_; }

    const Tags& GetTags() const noexcept { return tags_; }

    std::unique_ptr<Id> WithTag(const std::string& key,
                                const std::string& value) const {
        // Create a copy
        Tags tags{GetTags()};
        tags.add(key, value);
        return std::make_unique<Id>(Name(), tags);
    }

    std::unique_ptr<Id> WithTags(Tags&& extra_tags) const {
        Tags tags{GetTags()};
        tags.move_all(std::move(extra_tags));
        return std::make_unique<Id>(Name(), tags);
    }

    std::unique_ptr<Id> WithTags(const Tags& extra_tags) const {
        Tags tags{GetTags()};
        for (const auto& t : extra_tags) {
            tags.add(t.first, t.second);
        }
        return std::make_unique<Id>(Name(), tags);
    }

    std::unique_ptr<Id> WithStat(const std::string& stat) const {
        return WithTag("statistic", stat);
    };

    static std::shared_ptr<Id> WithDefaultStat(std::shared_ptr<Id> baseId,
                                               const std::string& stat) {
        if (baseId->GetTags().has("statistic")) {
            return baseId;
        } else {
            return baseId->WithStat(stat);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Id& id) {
        os << fmt::format("{}", id);
        return os;
    }

    friend struct std::hash<Id>;

    friend struct std::hash<std::shared_ptr<Id>>;

private:
    std::string name_;
    Tags tags_;
    mutable size_t hash_;

    size_t Hash() const noexcept {
        if (hash_ == 0) {
            // compute hash code, and reuse it
            hash_ = tags_.hash() ^ std::hash<std::string>()(name_);
        }
        return hash_;
    }
};

}  // namespace spectator