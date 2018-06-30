//
//  ObjectsRepository.h
//  ObjectsRepo
//
//  Created by Lin Luo on 6/29/18.
//  Copyright © 2018 Linfinity. All rights reserved.
//

#pragma once

#include <unordered_map>

namespace pi {

    template<typename T>
    class ObjectsRepository {
    public:
        static ObjectsRepository& Singleton() {
            static ObjectsRepository s_singleton;
            return s_singleton;
        }

        class ObjRef {
            friend class ObjectsRepository;

        public:
            ObjRef(const ObjRef&) = default;
            ObjRef(ObjRef&&) = default;

            constexpr const T& operator*() const& {
                return *_repo.getObject(_objId);
            }

            constexpr T& operator*() & {
                return *_repo.getObject(_objId);
            }

            constexpr const T* operator->() const& {
                return _repo.getObject(_objId);
            }

            constexpr T* operator->() & {
                return _repo.getObject(_objId);
            }

            constexpr const T& operator()() const& {
                return *_repo.getObject(_objId);
            }

            constexpr T& operator()() & {
                return *_repo.getObject(_objId);
            }

            explicit constexpr operator const T&() const& {
                return *_repo.getObject(_objId);
            }

            explicit constexpr operator T&() & {
                return *_repo.getObject(_objId);
            }

            void destroy() {
                _repo.destroyObject(_objId);
            }

            explicit constexpr operator bool() const noexcept {
                return _repo.hasObject(_objId);
            }

        private:
            explicit ObjRef(ObjectsRepository& repo, uint64_t objId = 0) : _repo(repo), _objId(objId) {}

            ObjectsRepository& _repo;
            uint64_t _objId;
        };

        template<typename... Args>
        ObjRef createObject(Args&&... args) {
            if (auto [it, r] = _repo.try_emplace(GetNextObjectId(), std::forward<Args>(args)...); r) {
                return ObjRef{*this, it->first};
            }
            return ObjRef{*this};
        }

        constexpr const ObjRef& getInvalid() const {
            return _invalid;
        }

    private:
        ObjRef _invalid{*this};

        bool hasObject(uint64_t objId) {
            return _repo.count(objId) > 0;
        }

        T* getObject(uint64_t objId) {
            if (auto it = _repo.find(objId); it != _repo.end()) {
                return &it->second;
            }
            return nullptr;
        }

        void destroyObject(uint64_t objId) {
            _repo.erase(objId);
        }

        static uint64_t GetNextObjectId() {
            static uint64_t s_id = 0;
            return ++s_id;
        }

        std::unordered_map<uint64_t, T> _repo;
    };

}
