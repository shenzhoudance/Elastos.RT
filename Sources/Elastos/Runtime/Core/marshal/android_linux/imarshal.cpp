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

#include <binder/Parcel.h>
#include <pthread.h>
#include <prxstub.h>
#include <rot.h>

_ELASTOS_NAMESPACE_USING

EXTERN_C const InterfaceID EIID_IProxy;

#define ASSERT_PROXY(pProxy) \
    do {assert(((CObjectProxy*)pProxy)->mInterfaceNum > 0);} while (0);

namespace Elastos {
namespace IPC {

extern pthread_mutex_t g_marshalLock;

ECode StdMarshalInterface(
    /* [in] */ IInterface* object,
    /* [out] */ InterfacePack* interfacePack)
{
    ExportObject ex;

    pthread_mutex_lock(&g_marshalLock);
    ECode ec = FindExportObject(object, &ex);
    if (SUCCEEDED(ec)) {
        interfacePack->mBinder = ex.mBinder;
        ex.mIStub->Release();
        ex.mIStub->GetClassID(&interfacePack->mClsid);
        ec = ex.mIStub->GetInterfaceIndex(object, &interfacePack->mIndex);
        if (ec == E_NO_INTERFACE) {
            ec = E_CONFLICT_WITH_LOCAL_KEYWORD;
#if defined(_DEBUG) || defined(_MARSHAL_DEBUG)
            String info;
            if (IObject::Probe(object) != NULL) {
                IObject::Probe(object)->ToString(&info);
            }
            ALOGE("Object %s with local constructor or interface cannot be created in diff process.",
                info.string());
#endif
        }
        pthread_mutex_unlock(&g_marshalLock);
    }
    else {
        IProxy* proxy = (IProxy*)object->Probe(EIID_IProxy);
        if (proxy) {
            pthread_mutex_unlock(&g_marshalLock);
            interfacePack->mBinder = ((CObjectProxy*)proxy)->mBinder;
            ASSERT_PROXY(proxy);
            proxy->GetClassID(&interfacePack->mClsid);
            ec = proxy->GetInterfaceIndex(object, &(interfacePack->mIndex));
        }
        else {
            IStub* stub;
            ec = CObjectStub::S_CreateObject(object, &stub);
            pthread_mutex_unlock(&g_marshalLock);
            if (SUCCEEDED(ec)) {
                interfacePack->mBinder = (android::IBinder*)(CObjectStub*)stub;
                stub->Release();
                stub->GetClassID(&(interfacePack->mClsid));
                ec = stub->GetInterfaceIndex(object, &(interfacePack->mIndex));
                if (ec == E_NO_INTERFACE) {
                    ec = E_CONFLICT_WITH_LOCAL_KEYWORD;
#if defined(_DEBUG) || defined(_MARSHAL_DEBUG)
                    String info;
                    if (IObject::Probe(object) != NULL) {
                        IObject::Probe(object)->ToString(&info);
                    }
                    ALOGE("Object %s with local constructor or interface cannot be created in diff process.",
                        info.string());
#endif
                }
            }
        }
    }

    return ec;
}

static ECode StdUnmarshalCustomInterface(
    /* [in] */ ICustomMarshal* customMarshal,
    /* [in] */ InterfacePack* interfacePack,
    /* [out] */ IInterface** object)
{
    ClassID proxyClsid;
    char uunmbuf[80];
    proxyClsid.mUunm = uunmbuf;

    ECode ec = customMarshal->GetClsid(&proxyClsid);
    if (SUCCEEDED(ec)) {
        AutoPtr<IInterface> obj;
        ec = _CObject_CreateInstance(proxyClsid, RGM_SAME_DOMAIN,
                EIID_ICustomMarshal, (IInterface**)&obj);
        if (SUCCEEDED(ec)) {
            ICustomMarshal* tempCustMsh = ICustomMarshal::Probe(obj);
            ec = tempCustMsh->CreateObject(customMarshal, object);
        }
    }

    return ec;
}

ECode StdUnmarshalInterface(
    /* [in] */ UnmarshalFlag flag,
    /* [in] */ InterfacePack* interfacePack,
    /* [out] */ IInterface** object)
{
    ExportObject ex;
    ICustomMarshal* customMarshal;
    *object = NULL;

    pthread_mutex_lock(&g_marshalLock);
    ECode ec = FindExportObject(interfacePack->mBinder.get(), &ex);
    if (SUCCEEDED(ec) && (flag == UnmarshalFlag_Noncoexisting)) {
        pthread_mutex_unlock(&g_marshalLock);
        customMarshal = ICustomMarshal::Probe(ex.mObject);
        if (!customMarshal) {
            CObjectStub* stub = (CObjectStub*)ex.mIStub;
            *object = stub->mInterfaces[interfacePack->mIndex].mObject;
            (*object)->AddRef();
            ex.mIStub->Release();

            return NOERROR;
        }
        else {
            ec = StdUnmarshalCustomInterface(customMarshal, interfacePack, object);
            return ec;
        }
    }

    ImportObject im;
    ec = FindImportObject(interfacePack->mBinder.get(), &im);
    if (SUCCEEDED(ec)) {
        pthread_mutex_unlock(&g_marshalLock);
        customMarshal = ICustomMarshal::Probe(im.mIProxy);
        if (!customMarshal) {
            ec = im.mIProxy->GetInterface(interfacePack->mIndex, object);
        }
        else {
            ec = StdUnmarshalCustomInterface(customMarshal, interfacePack, object);
        }
        im.mIProxy->Release();
    }
    else {
        AutoPtr<IProxy> proxy;
        ec = CObjectProxy::S_CreateObject(
            interfacePack->mClsid,
            interfacePack->mBinder,
            (IProxy**)&proxy);
        pthread_mutex_unlock(&g_marshalLock);
        if (SUCCEEDED(ec)) {
            customMarshal = ICustomMarshal::Probe(proxy);
            if (!customMarshal) {
                ec = proxy->GetInterface(interfacePack->mIndex, object);
            }
            else {
                ec = StdUnmarshalCustomInterface(customMarshal, interfacePack, object);
            }
        }
    }

    return ec;
}

} // namespace IPC
} // namespace Elastos
