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

#ifndef __ORG_APACHE_HARMONY_SECURITY_ASN1_CASN1INTEGERHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_ASN1_CASN1INTEGERHELPER_H__

#include "_Org_Apache_Harmony_Security_Asn1_CASN1IntegerHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;
using Elastos::Math::IBigInteger;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1IntegerHelper)
    , public Singleton
    , public IASN1IntegerHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetInstance(
        /* [out] */ IASN1Integer** instance);

    CARAPI ToIntValue(
        /* [in] */ IInterface* decoded,
        /* [out] */ Int32* intValue);

    CARAPI ToBigIntegerValue(
        /* [in] */ IInterface *decoded,
        /* [out] */ IBigInteger** bigInteger);

    CARAPI FromIntValue(
        /* [in] */ Int32 value,
        /* [out] */ IInterface** fromIntValue);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_ASN1_CASN1INTEGERHELPER_H__