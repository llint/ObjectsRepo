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
            ObjRef(const ObjRef& rhs) : _repo(rhs._repo), _objId(rhs._objId) {
                _repo.incRef(_objId);
            }

            ObjRef(ObjRef&& rhs) : _repo(rhs._repo), _objId(rhs._objId) {
                rhs._objId = 0;
                // ref count not changed
            }

            ~ObjRef() {
                if (_objId != 0) {
                    _repo.decRef(_objId);
                }
            }

            void release() {
                if (_objId != 0) {
                    _repo.decRef(_objId);
                    _objId = 0;
                }
            }

            constexpr const T& operator*() const& {
                return _objId != 0 ? *_repo.getObject(_objId) : *(T*)nullptr;
            }

            constexpr T& operator*() & {
                return _objId != 0 ? *_repo.getObject(_objId) : *(T*)nullptr;
            }

            constexpr const T* operator->() const& {
                return _objId != 0 ? _repo.getObject(_objId) : nullptr;
            }

            constexpr T* operator->() & {
                return _objId != 0 ? _repo.getObject(_objId) : nullptr;
            }

            constexpr const T& operator()() const& {
                return _objId != 0 ? *_repo.getObject(_objId) : *(T*)nullptr;
            }

            constexpr T& operator()() & {
                return _objId != 0 ? *_repo.getObject(_objId) : *(T*)nullptr;
            }

            explicit constexpr operator const T&() const& {
                return _objId != 0 ? *_repo.getObject(_objId) : *(T*)nullptr;
            }

            explicit constexpr operator T&() & {
                return _objId != 0 ? *_repo.getObject(_objId) : *(T*)nullptr;
            }

            explicit constexpr operator bool() const noexcept {
                return _objId != 0 && _repo.hasObject(_objId);
            }

        private:
            explicit ObjRef(ObjectsRepository& repo, uint64_t objId = 0) : _repo(repo), _objId(objId) {}

            ObjectsRepository& _repo;
            uint64_t _objId;
        };

        template<typename... Args>
        ObjRef createObject(Args&&... args) {
            if (auto [it, r] = _repo.try_emplace(GetNextObjectId(), std::piecewise_construct, std::forward_as_tuple(args...), std::make_tuple(1)); r) {
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
                return &it->second.first;
            }
            return nullptr;
        }

        void incRef(uint64_t objId) {
            if (auto it = _repo.find(objId); it != _repo.end()) {
                ++it->second.second;
            }
        }

        void decRef(uint64_t objId) {
            if (auto it = _repo.find(objId); it != _repo.end()) {
                if (--it->second.second <= 0) {
                    _repo.erase(it);
                }
            }
        }

        static uint64_t GetNextObjectId() {
            static uint64_t s_id = 0;
            return ++s_id;
        }

        std::unordered_map<uint64_t, std::pair<T, size_t>> _repo;
    };

}
