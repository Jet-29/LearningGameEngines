#pragma once

namespace Engine {
    class UUID {
    public:
        UUID();
        UUID(uint64_t uuid);

        operator uint64_t() const { return m_UUID; }

    private:
        uint64_t m_UUID;
    };
}

namespace std {
    template<>
    struct hash<Engine::UUID> {
        std::size_t operator()(const Engine::UUID& uuid) const {
            return (uint64_t) uuid;
        }
    };
}