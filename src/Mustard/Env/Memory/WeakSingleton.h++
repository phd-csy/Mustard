// -*- C++ -*-
//
// Copyright 2020-2024  The Mustard development team
//
// This file is part of Mustard, an offline software framework for HEP experiments.
//
// Mustard is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// Mustard is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Mustard. If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "Mustard/Env/Memory/WeakSingletonified.h++"
#include "Mustard/Env/Memory/internal/WeakSingletonBase.h++"
#include "Mustard/Env/Memory/internal/WeakSingletonPool.h++"
#include "Mustard/Utility/InlineMacro.h++"
#include "Mustard/Utility/PrettyLog.h++"

#include "fmt/format.h"

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace Mustard::Env::Memory {

template<typename ADerived>
class PassiveSingleton;

template<typename ADerived>
class WeakSingleton : public internal::WeakSingletonBase {
    friend class PassiveSingleton<ADerived>;

protected:
    WeakSingleton();
    ~WeakSingleton();

public:
    MUSTARD_ALWAYS_INLINE static auto NotInstantiated() -> bool { return UpdateInstance() == Status::NotInstantiated; }
    MUSTARD_ALWAYS_INLINE static auto Available() -> bool { return UpdateInstance() == Status::Available; }
    MUSTARD_ALWAYS_INLINE static auto Expired() -> bool { return UpdateInstance() == Status::Expired; }
    MUSTARD_ALWAYS_INLINE static auto Instantiated() -> bool { return not NotInstantiated(); }

private:
    enum struct Status {
        NotInstantiated,
        Available,
        Expired
    };

    MUSTARD_ALWAYS_INLINE static auto UpdateInstance() -> Status;

private:
    static std::shared_ptr<void*> fgInstance;
};

} // namespace Mustard::Env::Memory

#include "Mustard/Env/Memory/WeakSingleton.inl"
