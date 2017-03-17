//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __ELASTOS_UTILITY_JAR_ELASTOS_UTILITY_CPACK200_H__
#define __ELASTOS_UTILITY_JAR_ELASTOS_UTILITY_CPACK200_H__

#include "_Elastos_Utility_Jar_CPack200.h"
#include "Singleton.h"

using Elastos::Core::Singleton;
using Elastos::Utility::Jar::IPacker;
using Elastos::Utility::Jar::IUnpacker;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CPack200)
    , public Singleton
    , public IPack200
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI NewPacker(
        /* [out] */ IPacker** packer);

    CARAPI NewUnpacker(
        /* [out] */ IUnpacker** unpacker);

private:
    // TODO: Add your private member variables here.
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_JAR_ELASTOS_UTILITY_CPACK200_H__