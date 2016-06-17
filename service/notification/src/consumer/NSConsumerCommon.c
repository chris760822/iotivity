//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include "NSConsumerCommon.h"
#include "NSConstants.h"
#include "NSThread.h"
#include "oic_malloc.h"
#include "oic_string.h"

#define NS_QUERY_CONSUMER_ID "consumerid"

char ** NSGetConsumerId()
{
    static char * g_consumerId = NULL;
    return & g_consumerId;
}

void NSSetConsumerId(char * cId)
{
    NS_VERTIFY_NOT_NULL_V(cId);
    char ** consumerId = NSGetConsumerId();
    NSOICFree(*consumerId);
    *consumerId = (char *)OICMalloc(sizeof(char) * NS_DEVICE_ID_LENGTH);
    NS_VERTIFY_NOT_NULL_V(*consumerId);

    OICStrcpy(*consumerId, sizeof(char) * NS_DEVICE_ID_LENGTH, cId);
}

char * NSGetQuery(const char * uri)
{
    NS_VERTIFY_NOT_NULL(uri, NULL);

    char * consumerId = OICStrdup(*NSGetConsumerId());
    NS_VERTIFY_NOT_NULL(consumerId, NULL);

    size_t uriLen = strlen(uri) + 1;
    size_t qKeyLen = sizeof(NS_QUERY_CONSUMER_ID);
    size_t queryLen = NS_DEVICE_ID_LENGTH + uriLen + qKeyLen + 2;

    char * retQuery = (char *)OICMalloc(sizeof(char) * queryLen);
    NS_VERTIFY_NOT_NULL(retQuery, NULL);

    size_t index = 0;
    OICStrcpy((retQuery + index), uriLen, uri);
    index += uriLen - 1;
    OICStrcpy((retQuery + index), 2, "?");
    index += 1;
    OICStrcpy((retQuery + index), qKeyLen, NS_QUERY_CONSUMER_ID);
    index += qKeyLen - 1;
    OICStrcpy((retQuery + index), 2, "=");
    index += 1;
    OICStrcpy((retQuery + index), NS_DEVICE_ID_LENGTH, consumerId);

    NSOICFree(consumerId);

    return retQuery;
}

bool * NSGetBoneIsStartedConsumer()
{
    static bool g_isStartedConsumer = false;

    return & g_isStartedConsumer;
}

void NSSetIsStartedConsumer(bool setValue)
{
    * NSGetBoneIsStartedConsumer() = setValue;
}

bool NSIsStartedConsumer()
{
    return * NSGetBoneIsStartedConsumer();
}

NSProviderDiscoveredCallback * NSGetBoneDiscoverCb()
{
    static NSProviderDiscoveredCallback g_discoverCb = NULL;

    return & g_discoverCb;
}

void NSSetDiscoverProviderCb(NSProviderDiscoveredCallback cb)
{
    * NSGetBoneDiscoverCb() = cb;
}

NSProviderDiscoveredCallback NSGetDiscoverCb()
{
    return * NSGetBoneDiscoverCb();
}

void * NSDiscoveredProviderFunc(void * provider)
{
    NSGetDiscoverCb()((NSProvider *) provider);

    return NULL;
}

void NSDiscoveredProvider(NSProvider * provider)
{
    NSConsumerThread * thread = NSThreadInit(NSDiscoveredProviderFunc, (void *) provider);
    NS_VERTIFY_NOT_NULL_V(thread);
}

NSSyncCallback * NSGetBoneNotificationSyncCb()
{
    static NSSyncCallback g_syncCb = NULL;

    return & g_syncCb;
}

void NSSetNotificationSyncCb(NSSyncCallback cb)
{
    * NSGetBoneNotificationSyncCb() = cb;
}

typedef struct
{
    NSProvider * provider;
    NSSyncInfo * sync;
} NSSyncData;

void * NSNotificationSyncFunc(void * obj)
{
    NSProvider * provider = ((NSSyncData *) obj)->provider;
    NSSyncInfo * syncData = ((NSSyncData *) obj)->sync;
    (* NSGetBoneNotificationSyncCb())(provider, syncData);
    return NULL;
}

void NSNotificationSync(NSProvider * provider, NSSyncInfo * sync)
{
    NSSyncData * obj = (NSSyncData *)OICMalloc(sizeof(NSSyncData));
    NS_VERTIFY_NOT_NULL_V(obj);
    obj->provider = provider;
    obj->sync = sync;

    NSConsumerThread * thread = NSThreadInit(NSNotificationSyncFunc, (void *) obj);
    NS_VERTIFY_NOT_NULL_V(thread);
}

NSNotificationReceivedCallback  * NSGetBoneMessagePostedCb()
{
    static NSNotificationReceivedCallback  g_postCb = NULL;

    return & g_postCb;
}

void NSSetMessagePostedCb(NSNotificationReceivedCallback  cb)
{
    * NSGetBoneMessagePostedCb() = cb;
}

NSNotificationReceivedCallback  NSGetMessagePostedCb()
{
    return * NSGetBoneMessagePostedCb();
}

typedef struct
{
    NSProvider * provider;
    NSMessage * msg;
} NSMessageData;

void * NSMessagePostFunc(void * obj)
{
    NSMessageData * msgData = (NSMessageData *) obj;

    NSGetMessagePostedCb()((NSProvider *) msgData->provider,
            (NSMessage *) msgData->msg);
    return NULL;
}

void NSMessagePost(NSProvider * provider, NSMessage * msg)
{
    NSMessageData * obj = (NSMessageData *)OICMalloc(sizeof(NSMessageData));
    NS_VERTIFY_NOT_NULL_V(obj);
    obj->provider = provider;
    obj->msg = msg;

    NSConsumerThread * thread = NSThreadInit(NSMessagePostFunc, (void *) obj);
    NS_VERTIFY_NOT_NULL_V(thread);
}

NSTask * NSMakeTask(NSTaskType type, void * data)
{
    NSTask * retTask = OICMalloc(sizeof(NSTask));
    NS_VERTIFY_NOT_NULL(retTask, NULL);

    retTask->taskType = type;
    retTask->taskData = data;
    retTask->nextTask = NULL;

    return retTask;
}

NSMessage_consumer * NSCopyMessage(NSMessage_consumer * msg)
{
    NS_VERTIFY_NOT_NULL(msg, NULL);

    NSMessage_consumer * newMsg = (NSMessage_consumer *)OICMalloc(sizeof(NSMessage_consumer));
    NS_VERTIFY_NOT_NULL(newMsg, NULL);

    newMsg->addr = (OCDevAddr *)OICMalloc(sizeof(OCDevAddr));
    NS_VERTIFY_NOT_NULL_WITH_POST_CLEANING(newMsg, NULL, OICFree(newMsg));
    memcpy(newMsg->addr, msg->addr, sizeof(OCDevAddr));

    newMsg->messageId = msg->messageId;
    newMsg->title = OICStrdup(msg->title);
    newMsg->contentText = OICStrdup(msg->contentText);
    newMsg->sourceName = OICStrdup(msg->sourceName);

    return newMsg;
}
void NSRemoveMessage(NSMessage_consumer * msg)
{
    msg->messageId = 0;
    NSOICFree(msg->title);
    NSOICFree(msg->contentText);
    NSOICFree(msg->sourceName);
    NSOICFree(msg->addr);

    NSOICFree(msg);
}

OCStackResult NSInvokeRequest(OCDoHandle * handle,
        OCMethod method, const OCDevAddr * addr,
        const char * queryUrl, OCPayload * payload,
        void * callbackFunc, void * callbackData)
{
    OCCallbackData cbdata;

    cbdata.cb = callbackFunc;
    cbdata.context = callbackData;
    cbdata.cd = NULL;

    return OCDoResource(handle, method, queryUrl, addr,
            payload, CT_DEFAULT, NS_QOS, &cbdata, NULL, 0);
}