//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef OCPAYLOAD_H_
#define OCPAYLOAD_H_

#include <stdbool.h>
#include <inttypes.h>
#include "octypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct OCResource OCResource;

void OCPayloadDestroy(void *payload);

// Representation Payload
OCRepPayload* OCRepPayloadCreate();

OCRepPayload* OCRepPayloadClone(const OCRepPayload* payload);

void OCRepPayloadAppend(OCRepPayload* parent, OCRepPayload* child);

bool OCRepPayloadSetUri(OCRepPayload* payload, const char* uri);

bool OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType);
bool OCRepPayloadAddInterface(OCRepPayload* payload, const char* interface);

bool OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType);
bool OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* interface);

bool OCRepPayloadIsNull(const OCRepPayload* payload, const char* name);
bool OCRepPayloadSetNull(OCRepPayload* payload, const char* name);

bool OCRepPayloadSetPropInt(OCRepPayload* payload, const char* name, int64_t value);
bool OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value);

bool OCRepPayloadSetPropDouble(OCRepPayload* payload, const char* name, double value);
bool OCRepPayloadGetPropDouble(const OCRepPayload* payload, const char* name, double* value);

bool OCRepPayloadSetPropString(OCRepPayload* payload, const char* name, const char* value);
bool OCRepPayloadSetPropStringAsOwner(OCRepPayload* payload, const char* name, char* value);
bool OCRepPayloadGetPropString(const OCRepPayload* payload, const char* name, char** value);

bool OCRepPayloadSetPropBool(OCRepPayload* payload, const char* name, bool value);
bool OCRepPayloadGetPropBool(const OCRepPayload* payload, const char* name, bool* value);

bool OCRepPayloadSetPropObject(OCRepPayload* payload, const char* name, const OCRepPayload* value);
bool OCRepPayloadSetPropObjectAsOwner(OCRepPayload* payload, const char* name,
        const OCRepPayload* value);
bool OCRepPayloadGetPropObject(const OCRepPayload* payload, const char* name, OCRepPayload** value);

OCSecurityPayload* OCSecurityPayloadCreate(const char* securityData);

#ifdef WITH_CLIENT
// Discovery Payload
OCDiscoveryPayload* OCDiscoveryPayloadCreate();

void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
        uint16_t port);
void OCDiscoveryPayloadAddNewResource(OCDiscoveryPayload* payload, OCResourcePayload* res);
bool OCResourcePayloadAddResourceType(OCResourcePayload* payload, const char* resourceType);
bool OCResourcePayloadAddInterface(OCResourcePayload* payload, const char* interface);

size_t OCDiscoveryPayloadGetResourceCount(OCDiscoveryPayload* payload);
OCResourcePayload* OCDiscoveryPayloadGetResource(OCDiscoveryPayload* payload, size_t index);

// Device Payload
OCDevicePayload* OCDevicePayloadCreate(const char* uri, const uint8_t* sid, const char* dname,
        const char* specVer, const char* dmVer);

// Platform Payload
OCPlatformPayload* OCPlatformPayloadCreate(const char* uri, const OCPlatformInfo* platformInfo);
OCPlatformPayload* OCPlatformPayloadCreateAsOwner(char* uri, OCPlatformInfo* platformInfo);

#ifdef WITH_PRESENCE
// Presence Payload
OCPresencePayload* OCPresencePayloadCreate(uint32_t seqNum, uint32_t maxAge,
        OCPresenceTrigger trigger, const char* resourceType);
#endif

// Helper API
OCStringLL* CloneOCStringLL (OCStringLL* ll);
void OCFreeOCStringLL(OCStringLL* ll);

#endif // WITH_CLIENT

#ifdef __cplusplus
}
#endif

#endif
